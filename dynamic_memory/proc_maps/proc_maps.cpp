#include "dynamic_memory/proc_maps/proc_maps.h"

#include <sstream>
#include <format>

std::expected<std::ifstream, std::string> get_proc_mappings_file(pid_t pid)
{
    std::ifstream proc_mappings{std::format("/proc/{}/maps", pid)};
    if (proc_mappings.is_open())
    {
        return proc_mappings;
    }
    else
    {
        return std::unexpected{"Could not open file"};
    }
}

std::optional<memory_mapped_region_information> extract_memory_mapped_region_information(std::string_view memory_mapped_region_line)
{
#if defined(__cpp_lib_sstream_from_string_view) && __cpp_lib_sstream_from_string_view >= 202306L
    std::istringstream memory_region_information{memory_mapped_region_line};
#else
    std::istringstream memory_region_information{std::string{memory_mapped_region_line}};
#endif
    std::string address_column;
    memory_region_information >> address_column;
    const auto start_end_address_separator = address_column.find_first_of('-');
    if (start_end_address_separator == std::string::npos)
    {
        // If we do not find the '-' it means that it is another line in the file that does not contain region information
        return std::nullopt;
    }
    const auto hex_start_address = address_column.substr(0, start_end_address_separator);
    const auto hex_end_address = address_column.substr(start_end_address_separator + 1);
    const auto dec_start_address = std::stoul(hex_start_address, nullptr, 16);
    const auto dec_end_address = std::stoul(hex_end_address, nullptr, 16);

    std::string permissions_column;
    memory_region_information >> permissions_column;
    std::string offset_column;
    memory_region_information >> offset_column;
    std::string device_column;
    memory_region_information >> device_column;
    std::string inode_column;
    memory_region_information >> inode_column;
    std::string pathname_column;
    memory_region_information >> pathname_column;
    return memory_mapped_region_information{
        .start_address = dec_start_address,
        .end_address = dec_end_address,
        .permissions = permissions_column,
        .offset = offset_column,
        .device = device_column,
        .inode = inode_column,
        .pathname = pathname_column,
    };
}

std::expected<memory_mapped_region_information, std::string> get_region_information_by_address(std::ifstream &&proc_mappings, std::uintptr_t decimal_address)
{
    while (!proc_mappings.eof())
    {
        std::string line;
        std::getline(proc_mappings, line);
        const auto region_information = extract_memory_mapped_region_information(line);
        if (region_information.has_value())
        {
            if (region_information->start_address <= decimal_address && decimal_address <= region_information->end_address)
            {
                return {region_information.value()};
            }
        }
    }
    return std::unexpected<std::string>{"Region not found"};
}

std::expected<memory_mapped_region_information, std::string> get_region_information_by_name(std::ifstream &&proc_mappings, std::string_view region_name)
{
    while (!proc_mappings.eof())
    {
        std::string line;
        std::getline(proc_mappings, line);
        const auto region_information = extract_memory_mapped_region_information(line);
        if (region_information.has_value())
        {
            if (region_information->pathname == region_name)
            {
                return {region_information.value()};
            }
        }
    }
    return std::unexpected<std::string>{"Region not found"};
}
