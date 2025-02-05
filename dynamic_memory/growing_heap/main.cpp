#include <iostream>
#include <memory>
#include <deque>
#include <format>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <functional>
#include <expected>

#include "dynamic_memory/proc_maps/proc_maps.h"

int get_integer_from_cin()
{
    int value;
    std::cin >> value;
    return value;
}

char get_char_from_cin()
{
    char value;
    std::cin >> value;
    return value;
}

auto print_error(const std::string &error)
{
    std::cerr << error << "\n";
    return std::expected<void, std::string>{};
}

auto print_region_information(memory_mapped_region_information region_information)
{
    std::cout << "Pathname: " << region_information.pathname << "\n";
    std::cout << "Start address: " << region_information.start_address << " (0x" << std::format("{:x}", region_information.start_address) << ")\n";
    std::cout << "End address: " << region_information.end_address << " (0x" << std::format("{:x}", region_information.end_address) << ")\n";
    return std::expected<void, std::string>{};
}

void get_memory_region_by_decimal_address(pid_t pid, std::uintptr_t decimal_address)
{
    get_proc_mappings_file(pid)
        .and_then(std::bind(get_region_information_by_address, std::placeholders::_1, decimal_address))
        .and_then(print_region_information)
        .or_else(print_error);
}

void get_memory_region_by_name(pid_t pid, std::string_view region_name)
{
    get_proc_mappings_file(pid)
        .and_then(std::bind(get_region_information_by_name, std::placeholders::_1, region_name))
        .and_then(print_region_information)
        .or_else(print_error);
}

void print_heap_region_information(unsigned int pid)
{
    std::cout << "--- Heap memory region info for pid " << pid << " (start) ---\n";
    get_memory_region_by_name(pid, "[heap]");
    std::cout << "--- Heap memory region info (end) ---\n";
}

void print_stack_region_information(unsigned int pid)
{
    std::cout << "--- Stack memory region info for pid " << pid << " (start) ---\n";
    get_memory_region_by_name(pid, "[stack]");
    std::cout << "--- Stack memory region info (end) ---\n";
}

using array_t = int[];
using element_t = std::remove_pointer_t<std::decay_t<array_t>>;
constexpr auto element_size = sizeof(element_t);
using pointer_t = std::unique_ptr<array_t>;

std::pair<pointer_t, unsigned long> do_allocation()
{
    std::cout << "How many bytes would you like to allocate? ";
    const auto bytes_to_allocate = get_integer_from_cin();
    const auto array_size = bytes_to_allocate / element_size;
    const auto allocated_memory = array_size * element_size;
    std::cout << "Allocating an array of " << array_size << " elements with a total size of " << allocated_memory << " bytes\n";
    auto pointer = std::make_unique<array_t>(array_size);

    return std::make_pair(std::move(pointer), allocated_memory);
}

int main()
{
    const auto pid = getpid();
    std::cout << "Process started with pid " << pid << "\n";
    print_heap_region_information(pid);
    std::cout << "\n";

    std::deque<std::pair<pointer_t, unsigned long>> allocation_queue{};
    unsigned long total_allocated = 0;
    while (true)
    {
        std::cout << "What would you like to do?\n";
        std::cout << " a - Allocate\n";
        std::cout << " i - Random initialize memory\n";
        std::cout << " p - Print pointer addresses\n";
        std::cout << " d - Deallocate\n";
        std::cout << " q - Quit\n";
        const auto action = get_char_from_cin();
        switch (action)
        {
        case 'a':
            allocation_queue.push_back(do_allocation());
            total_allocated += allocation_queue.back().second;
            break;
        case 'i':
            for (auto &element : allocation_queue)
            {
                const auto elements_in_array = element.second / element_size;
                for (unsigned long i = 0; i < elements_in_array; i++)
                {
                    element.first[i] = std::rand();
                }
            }
            break;
        case 'p':
            // TODO: potentially talk about https://sourceware.org/glibc/wiki/MallocInternals and multiple memory regions for the heap
            for (auto &element : allocation_queue)
            {
                const auto dec_address = reinterpret_cast<unsigned long>(&element.first[0]);
                std::cout << element.second << " bytes at 0x" << std::format("{:x}", dec_address) << "\n";
                get_memory_region_by_decimal_address(pid, dec_address);
            }
            break;
        case 'd':
            std::cout << "deallocate\n";
            break;
        case 'q':
            std::exit(EXIT_SUCCESS);
            break;
        default:
            std::cout << "Invalid option, please choose again\n";
            continue;
            break;
        }
        std::cout << "Total amount allocated until now: " << total_allocated << " bytes\n";
        print_heap_region_information(pid);
        std::cout << "\n";
    }
}
