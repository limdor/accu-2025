#include <vector>
#include <source_location>
#include <iostream>

template <class T>
class MonotonicAllocator
{

public:
    using value_type = T;
    using propagate_on_container_move_assignment = std::false_type;

    MonotonicAllocator()
    {
        static int i = 0;
        id = i++;
    }

    [[nodiscard]] T *allocate(std::size_t size)
    {
        std::cout << std::source_location::current().function_name() << "(id " << id << ", size " << size << ")\n";
        auto p = std::malloc(sizeof(T) * size);
        return static_cast<T *>(p);
    }
    void deallocate(T *pointer, std::size_t size) noexcept
    {
        std::cout << std::source_location::current().function_name() << "(id " << id << ", size " << size << ")\n";
        std::free(pointer);
    }
    bool operator<=>(const MonotonicAllocator<T> &) const = default;

private:
    int id;
};

int main()
{
    std::vector<int, MonotonicAllocator<int>> foo{2, 3, 4};
    std::vector<int, MonotonicAllocator<int>> bar{1, 2};
    bar = std::move(foo);
    bar.push_back(3);
    return 0;
}