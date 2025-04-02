#include <vector>
#include <source_location>
#include <iostream>
#include <memory_resource>

int main()
{
    std::array<std::byte, 50> storage;

    std::pmr::monotonic_buffer_resource memory_resource{
        storage.data(), storage.size(),
        std::pmr::null_memory_resource()};

    std::pmr::vector<int> vector{
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        &memory_resource};

    return 0;
}