#include <array>
#include <source_location>
#include <iostream>
#include <vector>

std::array<std::byte, 4000> storage;
int free_slot = 0;

class Point
{
public:
    static void *operator new(std::size_t size)
    {
        std::cout << std::source_location::current().function_name() << " with size " << size << "\n";
        if (free_slot + size >= storage.size())
        {
            throw std::bad_alloc();
        }
        void *pointer = &storage[free_slot];
        free_slot += size;
        return pointer;
    }
    static void operator delete(void *) {}

private:
    float x;
    float y;
    float z;
};

int main()
{
    [[maybe_unused]] auto p = new Point();
    return 0;
}
