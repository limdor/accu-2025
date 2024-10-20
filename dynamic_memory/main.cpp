#include <string>
#include <new>
#include <atomic>
#include <iostream>
#include <functional>
#include <vector>
#include <array>
#include <thread>
#include <future>
#include <any>

#include <boost/stacktrace.hpp>

namespace{
    std::atomic<bool> dynamic_memory_allocated{false};
}
void *operator new(std::size_t count)
{
    //std::cout << "  -> void *operator new(" << count << ")\n";
    dynamic_memory_allocated = true;
    return malloc(count);
}

void *operator new(std::size_t count, std::align_val_t al)
{
    //std::cout << "  -> void *operator new(" << count << ", " << static_cast<std::size_t>(al) << ")\n";
    std::cerr << "Still not implemented the forwarding of new with alignment constraints to malloc\n";
    std::terminate();
}

void allocation_type(){
    if(dynamic_memory_allocated){
        std::cout << "(dynamic):";
        std::cout << "    " << boost::stacktrace::stacktrace();
    } else{
        std::cout << "( static):";
    }
    dynamic_memory_allocated = false;
}

void answer_to_live_or_abort(int value){
    if(value != 42){
        std::terminate();
    }
}

struct universe_t{
    void answer_to_live_or_abort(int value){
        ::answer_to_live_or_abort(value);
    }
};

int main()
{
    using namespace std::placeholders;

    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "--------- Examples to understand when dynamic memory allocation is performed ---------\n";
    std::cout << "--------------------------------------------------------------------------------------\n";

    std::function<void(int)> f_answer_to_live_or_abort = answer_to_live_or_abort;
    allocation_type();
    std::cout << "std::function void(int)\n";

    universe_t universe;
    std::function<void(int)> f_struct_answer_to_live = std::bind(&universe_t::answer_to_live_or_abort, &universe, _1);
    allocation_type();
    std::cout << "std::function void(int) calling a member function\n";

    const std::string small_string = "my_small_string";
    allocation_type();
    std::cout << "std::string that fits for the small string optimization\n";
    
    const std::string big_string = "my_big_string_that_does_not_fit_for_small_string_optimization";
    allocation_type();
    std::cout << "std::string that does not fit for the small string optimization\n";

    const std::vector<int> vector_of_ints = {4,3,5,6,7,8,9,10};
    allocation_type();
    std::cout << "std::vector of ints\n";

    const std::vector<std::string> default_constructed_vector;
    allocation_type();
    std::cout << "std::vector default ctor\n";

    const std::array array_of_ints = {4,3,5,6,7,8,9,10};
    allocation_type();
    if(array_of_ints.size() != 8){
        std::terminate();
    }
    std::cout << "std::array of ints\n";

    std::thread thread{answer_to_live_or_abort, 42};
    thread.join();
    allocation_type();
    std::cout << "std::thread passing void(int) function\n";

    std::promise<void> promise;
    allocation_type();
    std::cout << "std::promise default ctor\n";
    auto future = promise.get_future();
    allocation_type();
    std::cout << "promise.get_future\n";
    promise.set_value();
    allocation_type();
    std::cout << "promise.set_value\n";
    future.get();

    std::future<int> future2;
    allocation_type();
    std::cout << "std::future default ctor\n";

    std::any small_any = 42;
    allocation_type();
    std::cout << "std::any of 'small' object\n";

    std::any large_any = small_string;
    allocation_type();
    std::cout << "std::any of 'large' object\n";

    try{
        throw std::runtime_error("my exception");
    }catch(std::exception const& ex){
        allocation_type();
        std::cout << "throwing a std::runtime_error\n";
    }

    try{
        throw std::bad_alloc{};
    }catch(std::exception const& ex){
        allocation_type();
        std::cout << "throwing a std::bad_alloc\n";
    }

    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "--------- End of the examples, feel free to add more cases ---------------------------\n";
    std::cout << "--------------------------------------------------------------------------------------\n";

    return 0;
}
