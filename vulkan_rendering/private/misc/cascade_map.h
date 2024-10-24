//
// Created by Tereza on 10/24/2024.
//

#pragma once
#include <map>

template<typename K1, typename K2, typename V>
class cascade_map {
  std::map<K1, std::map<K2, V>> map{};
};
