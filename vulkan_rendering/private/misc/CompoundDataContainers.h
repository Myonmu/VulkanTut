//
// Created by Tereza on 10/24/2024.
//

#pragma once
#include <expected>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <ranges>

#define CASCADE_KV_DECL(KvType, name)\
template <typename TKey, typename TVal>\
using cascade_##name##_base = KvType<TKey, TVal>;\
\
template <typename TKey, typename... TRest>\
struct cascade_##name##_helper {\
    using type = KvType<TKey, typename cascade_##name##_helper<TRest...>::type>;\
};\
\
template <typename TKey, typename TVal>\
struct cascade_##name##_helper<TKey, TVal> {\
    using type = cascade_##name##_base<TKey, TVal>;\
};\
\
template <typename... T>\
using cascade_##name## = typename cascade_##name##_helper<T...>::type;


CASCADE_KV_DECL(std::unordered_map, unordered_map)
CASCADE_KV_DECL(std::map, map)



/* A double-key unordered map, with one key being "Strong" and the other "Weak".
 * Each strong key is mapped to a value,
 * and each weak key is mapped to a strong key.
 * This means multiple weak keys may be mapped to the same strong key, and is mapped to the same value.
 * Simply put, weak keys are "aliases" for the strong key.
 */
template<typename StrongKey, typename WeakKey, typename Val,
    template <typename...> class StrongMap,
    template <typename...> class WeakMap>
class aliased_t_map {
    // Maps weak keys to strong keys (many-to-one relationship)
    WeakMap<WeakKey, StrongKey> weakToStrong{};
    // Maps strong keys to values (one-to-one relationship)
    StrongMap<StrongKey, Val> strongToVal{};

public:
    // Access value by StrongKey
    Val &operator[](const StrongKey &strongKey) {
        return strongToVal[strongKey];
    }

    Val &operator[](const WeakKey &weakKey) {
        return strongToVal[getStrongKey(weakKey).value()];
    }

    // Retrieve StrongKey for a given WeakKey
    std::expected<StrongKey, std::string> getStrongKey(const WeakKey &weakKey) const {
        if (auto it = weakToStrong.find(weakKey); it != weakToStrong.end()) {
            return it->second;
        }
        return std::unexpected{"WeakKey not found"};
    }

    // Retrieve all WeakKeys associated with a StrongKey
    auto getWeakKeys(const StrongKey &strongKey) const {
        return weakToStrong | std::views::filter([&](const auto &pair) {
            return pair.second == strongKey;
        }) | std::views::keys;
    }

    // Emplace a new StrongKey-WeakKey-Value association
    Val &emplace(const StrongKey &strongKey, const WeakKey &weakKey, Val &&value) {
        // Insert into the strong-to-value map
        auto [it, inserted] = strongToVal.emplace(strongKey, std::forward<Val>(value));
        if (!inserted && it->second != value) {
            throw std::runtime_error("StrongKey already exists with a different value");
        }

        // Associate the weak key with the strong key
        weakToStrong[weakKey] = strongKey;

        return it->second;
    }

    // Erase by StrongKey
    void eraseByStrongKey(const StrongKey &strongKey) {
        if (strongToVal.erase(strongKey) == 0) {
            throw std::runtime_error("StrongKey not found");
        }

        // Remove all associated weak keys
        auto keysToRemove = getWeakKeys(strongKey);
        for (const auto &weakKey: keysToRemove) {
            weakToStrong.erase(weakKey);
        }
    }

    // Erase by WeakKey
    void eraseByWeakKey(const WeakKey &weakKey) {
        weakToStrong.erase(weakKey);
    }

    // Check if a StrongKey exists
    bool containsStrongKey(const StrongKey &strongKey) const {
        return strongToVal.contains(strongKey);
    }

    // Check if a WeakKey exists
    bool containsWeakKey(const WeakKey &weakKey) const {
        return weakToStrong.contains(weakKey);
    }

    template <typename Func>
    requires std::invocable<Func, const StrongKey&, const Val&>
    void forEachValue(Func&& func) const {
        for (const auto& [key, value] : strongToVal) {
            func(key, value);
        }
    }
};

template<typename StrongKey, typename WeakKey, typename Val>
using aliased_unordered_map = aliased_t_map<StrongKey, WeakKey, Val, std::unordered_map, std::unordered_map>;

template<typename StrongKey, typename WeakKey, typename Val>
using aliased_map = aliased_t_map<StrongKey, WeakKey, Val, std::map, std::map>;