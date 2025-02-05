#include <string>
#include <string_view>
#include <expected>
#include <fstream>
#include <optional>
#include <unistd.h>

struct memory_mapped_region_information
{
    std::uintptr_t start_address;
    std::uintptr_t end_address;
    std::string permissions;
    std::string offset;
    std::string device;
    std::string inode;
    std::string pathname;
};

std::expected<std::ifstream, std::string>
get_proc_mappings_file(pid_t pid);

std::optional<memory_mapped_region_information>
extract_memory_mapped_region_information(std::string_view memory_mapped_region_line);

std::expected<memory_mapped_region_information, std::string>
get_region_information_by_address(std::ifstream &&proc_mappings, std::uintptr_t decimal_address);

std::expected<memory_mapped_region_information, std::string>
get_region_information_by_name(std::ifstream &&proc_mappings, std::string_view region_name);
