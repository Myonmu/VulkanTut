import re
import os
import sys

def find_vulkan_sdk():
    """Find Vulkan SDK from environment variable or default paths."""
    vulkan_sdk = os.getenv("VULKAN_SDK")

    if vulkan_sdk:
        header_path = os.path.join(vulkan_sdk, "Include", "vulkan", "vulkan_core.h")
        if os.path.exists(header_path):
            return header_path
        else:
            raise FileNotFoundError(f"vulkan_core.h not found in {vulkan_sdk}")

    raise EnvironmentError("VULKAN_SDK environment variable is not set.")

def parse_vulkan_formats(header_file):
    """Parses Vulkan formats from vulkan_core.h."""
    with open(header_file, 'r') as file:
        content = file.read()

    # Match typedef enum VkFormat { ... } pattern
    enum_pattern = re.compile(r"typedef enum VkFormat \{([^}]*)} VkFormat;", re.DOTALL)
    match = enum_pattern.search(content)
    if not match:
        raise ValueError("Could not find VkFormat enum in the header file.")

    enum_block = match.group(1)

    # Extract individual format entries (ignore = values if present)
    format_pattern = re.compile(r"VK_FORMAT_\w+")
    formats = format_pattern.findall(enum_block)

    # Resolve aliases (if format has an alias, use the canonical format)
    alias_pattern = re.compile(r"VK_FORMAT_(\w+)_EXT\s*=\s*VK_FORMAT_(\w+)")
    aliases = dict(alias_pattern.findall(content))

    # Deduplicate formats by using aliases
    canonical_formats = set()
    resolved_formats = []

    for fmt in formats:
        if fmt in aliases:
            canonical_formats.add(aliases[fmt])  # Use the canonical format if it's an alias
        else:
            canonical_formats.add(fmt)

    # Collect resolved formats without duplicates
    for fmt in canonical_formats:
        resolved_formats.append(fmt)

    return resolved_formats

def get_format_properties(format_name):
    """Estimate properties of the Vulkan format."""
    # Initialize default properties
    properties = {
        "channels": 0,
        "channelMask": 0,
        "bitsCount": 0,

    }

    # Heuristic-based approach to extract channel info based on the format name
    if "R" in format_name or "G" in format_name or "B" in format_name or "A" in format_name:
        if "R8" in format_name:
            properties["channels"] = 1
            properties["channelMask"] = 0b0001  # R channel
            properties["bitsCount"] = 8
        elif "RG8" in format_name or "R8G8" in format_name:
            properties["channels"] = 2
            properties["channelMask"] = 0b0011  # RG channels
            properties["bitsCount"] = 16
        elif "RGB8" in format_name or "R8G8B8" in format_name:
            properties["channels"] = 3
            properties["channelMask"] = 0b0111  # RGB channels
            properties["bitsCount"] = 24
        elif "RGBA8" in format_name or "R8G8B8A8" in format_name:
            properties["channels"] = 4
            properties["channelMask"] = 0b1111  # RGBA channels
            properties["bitsCount"] = 32
        # Additional cases can be added for more formats

    # Heuristic for depth/stencil formats (VK_FORMAT_D16_UNORM, etc.)
    elif "D" in format_name and "S" in format_name:
        properties["channels"] = 1
        properties["channelMask"] = 0b0001  # Depth channel
        properties["bitsCount"] = 16 if "16" in format_name else 24

    return properties

def generate_lookup_table(formats):
    """Generates the lookup table from Vulkan formats."""
    # Create the lookup table header
    lookup_table = """#ifndef FORMAT_UTILS_LOOKUP_TABLE_H
#define FORMAT_UTILS_LOOKUP_TABLE_H

#include <unordered_map>
#include "vulkan/vulkan.h"  // Assuming Vulkan header is included

struct FormatProperties {
    uint32_t channels = 0;
    uint32_t channelMask = 0; // r-g-b-a
    uint32_t bitsCount = 0;
};

static const std::unordered_map<VkFormat, FormatProperties> formatProperties = {
"""

    # Iterate over formats and generate the properties lookup
    for fmt in formats:
        properties = get_format_properties(fmt)
        lookup_table += f"    {{{fmt}, {{ {properties['channels']}, {properties['channelMask']}, {properties['bitsCount']} }} }},\n"

    lookup_table += """};

#endif // FORMAT_UTILS_LOOKUP_TABLE_H
"""

    return lookup_table

def main():
    try:
        # Try to find Vulkan SDK path
        header_file = find_vulkan_sdk()
        print(f"Found Vulkan SDK, using header file: {header_file}")

        formats = parse_vulkan_formats(header_file)
        lookup_table = generate_lookup_table(formats)

        # Output file path (same directory as the script)
        output_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), "FormatUtilsLookupTable.h")

        # Write to output file
        with open(output_file, 'w') as file:
            file.write(lookup_table)

        print(f"Lookup table has been written to {output_file}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()