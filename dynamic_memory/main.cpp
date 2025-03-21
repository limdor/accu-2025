#include <string>
#include <new>
#include <atomic>
#include <iostream>
#include <functional>
#include <vector>
#include <array>
#include <thread>
#include <future>
#include <string_view>
#include <any>
#include <stacktrace>
#include <source_location>

namespace
{
    std::atomic<bool> g_tracking{false};
    std::atomic<std::size_t> g_allocated_bytes{0};
    std::atomic<std::size_t> g_number_of_allocations{0};
}

void *operator new(std::size_t count)
{
    // std::cout << std::source_location::current().function_name() << "\n";
    if (g_tracking)
    {
        g_allocated_bytes += count;
        g_number_of_allocations++;
    }
    void *pointer_to_storage = malloc(count);
    if (!pointer_to_storage)
    {
        // TODO: Handle new_handler
        throw std::bad_alloc();
    }
    return pointer_to_storage;
}

void *operator new(std::size_t count, std::align_val_t alignment)
{
    // std::cout << std::source_location::current().function_name() << "\n";
    if (g_tracking)
    {
        g_allocated_bytes += count;
        g_number_of_allocations++;
    }
    void *pointer_to_storage = aligned_alloc(count, static_cast<std::size_t>(alignment));
    if (!pointer_to_storage)
    {
        // TODO: Handle new_handler
        throw std::bad_alloc();
    }
    return pointer_to_storage;
}

void *operator new(std::size_t size, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    try
    {
        return ::operator new(size);
    }
    catch (...)
    {
        return nullptr;
    }
}

void *operator new(std::size_t size, std::align_val_t alignment, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    try
    {
        return ::operator new(size, alignment);
    }
    catch (...)
    {
        return nullptr;
    }
}

void *operator new[](std::size_t size)
{
    // std::cout << std::source_location::current().function_name() << "\n";
    return ::operator new(size);
}

void *operator new[](std::size_t size, std::align_val_t alignment)
{
    // std::cout << std::source_location::current().function_name() << "\n";
    return ::operator new(size, alignment);
}

void *operator new[](std::size_t size, const std::nothrow_t &nothrow) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    return ::operator new(size, nothrow);
}

void *operator new[](std::size_t size, std::align_val_t alignment, const std::nothrow_t &nothrow) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    return ::operator new(size, alignment, nothrow);
}

void operator delete(void *ptr) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    free(ptr);
}

void operator delete(void *ptr, std::size_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    free(ptr);
}

void operator delete(void *ptr, std::align_val_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    free(ptr);
}

void operator delete(void *ptr, std::size_t, std::align_val_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    free(ptr);
}

void operator delete(void *ptr, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete(void *ptr, std::align_val_t alignment, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr, std::size_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr, std::align_val_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr, std::size_t, std::align_val_t) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void operator delete[](void *ptr, std::align_val_t alignment, const std::nothrow_t &) noexcept
{
    // std::cout << std::source_location::current().function_name() << "\n";
    ::operator delete(ptr);
}

void reset_allocation_statistics()
{
    g_allocated_bytes = 0;
    g_number_of_allocations = 0;
}

void answer_to_live_or_abort(int value)
{
    if (value != 42)
    {
        std::terminate();
    }
}

struct universe_t
{
    void answer_to_live_or_abort(int value)
    {
        ::answer_to_live_or_abort(value);
    }
};

class AllocationTracker
{
public:
    AllocationTracker(std::string_view message) : m_message{message}
    {
        reset_allocation_statistics();
        if (g_tracking)
        {
            std::cerr << "Nested tracking not supported\n";
            std::terminate();
        }
        g_tracking = true;
    };
    ~AllocationTracker()
    {
        // First thing we do is to get a copy allocation statistics, we want to make sure that if during
        // reporting we allocate memroy it is not considered
        const auto number_of_allocations = g_number_of_allocations.load();
        const auto allocated_bytes = g_allocated_bytes.load();

        std::cout << m_message << "\n";
        const auto allocation_type = number_of_allocations > 0 ? "dynamic" : "static";
        std::cout << "  - Allocation type: " << allocation_type << "\n";
        if (number_of_allocations > 0)
        {
            std::cout << "  - Number of allocations: " << number_of_allocations << "\n";
            std::cout << "  - Allocated bytes: " << allocated_bytes << "\n";
        }

        reset_allocation_statistics();
        g_tracking = false;
    };

private:
    std::string_view m_message;
};

int main()
{
    using namespace std::placeholders;

    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "--------- Examples to understand when dynamic memory allocation is performed ---------\n";
    std::cout << "--------------------------------------------------------------------------------------\n";

    {
        AllocationTracker tracker{"Dynamic builtin array of 2 integers with extended alignment"};
        [[maybe_unused]] int *pointer = new alignas(64) int[2]();
        delete[] (pointer);
    }

    {
        AllocationTracker tracker{"Unique pointer of dynamic array of 2 integers with extended alignment"};
        [[maybe_unused]] const auto pointer = std::make_unique<alignas(64) int[]>(2);
    }

    {
        AllocationTracker tracker{"Unique pointer of struct with extended alignment"};
        struct alignas(64) Foo
        {
            int bar{42};
        };

        const auto pointer = std::make_unique<Foo>();
    }

    {
        AllocationTracker tracker{"Dynamic builtin array of 42 integers"};
        auto *pointer = new int[42];
        delete[] (pointer);
    }

    {
        AllocationTracker tracker{"nothrow new allocation of int"};
        auto *pointer = new (std::nothrow) int;
        delete (pointer);
    }

    {
        AllocationTracker tracker{"std::function void(int)"};
        std::function<void(int)> f_answer_to_live_or_abort = answer_to_live_or_abort;
    }

    {
        AllocationTracker tracker{"std::function void(int) calling a member function"};
        universe_t universe;
        std::function<void(int)> f_struct_answer_to_live = std::bind(&universe_t::answer_to_live_or_abort, &universe, _1);
    }

    {
        AllocationTracker tracker{"std::string that fits for the small string optimization"};
        const std::string small_string = "my_small_string";
    }

    {
        AllocationTracker tracker{"std::string that does not fit for the small string optimization"};
        const std::string big_string = "my_big_string_that_does_not_fit_for_small_string_optimization";
    }

    {
        AllocationTracker tracker{"std::vector of ints"};
        const std::vector<int> vector_of_ints = {4, 3, 5, 6, 7, 8, 9, 10};
    }

    {
        AllocationTracker tracker{"std::vector default ctor"};
        const std::vector<std::string> default_constructed_vector;
    }

    {
        AllocationTracker tracker{"std::array of ints"};
        const std::array array_of_ints = {4, 3, 5, 6, 7, 8, 9, 10};

        if (array_of_ints.size() != 8)
        {
            std::terminate();
        }
    }

    {
        AllocationTracker tracker{"std::thread passing void(int) function"};
        std::thread thread{answer_to_live_or_abort, 42};
        thread.join();
    }

    {
        AllocationTracker tracker{"std::promise default ctor"};
        std::promise<void> promise;
    }

    {
        std::promise<void> promise;
        {
            AllocationTracker tracker{"promise.get_future"};
            auto future = promise.get_future();
        }
    }

    {
        std::promise<void> promise;
        auto future = promise.get_future();
        {
            AllocationTracker tracker{"promise.set_value"};
            promise.set_value();
        }
        future.get();
    }

    {
        AllocationTracker tracker{"std::future default ctor"};
        std::future<int> future2;
    }

    {
        AllocationTracker tracker{"std::any of 'small' object"};
        std::any small_any = 42;
    }

    {
        const std::string small_string = "my_small_string";
        {
            AllocationTracker tracker{"std::any of 'large' object"};
            std::any large_any = small_string;
        }
    }

    {
        AllocationTracker tracker{"throwing a std::runtime_error with small string literal as message"};
        try
        {
            throw std::runtime_error("my exception");
        }
        catch (std::exception const &ex)
        {
        }
    }

    {
        AllocationTracker tracker{"throwing a std::bad_alloc"};
        try
        {
            throw std::bad_alloc{};
        }
        catch (std::exception const &ex)
        {
        }
    }

    {
        AllocationTracker tracker{"Getting the current std::stacktrace"};
        std::stacktrace::current();
    }

    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "--------- End of the examples, feel free to add more cases ---------------------------\n";
    std::cout << "--------------------------------------------------------------------------------------\n";

    return 0;
}
