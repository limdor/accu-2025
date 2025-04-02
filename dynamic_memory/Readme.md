# Dynamic memory allocation playground

## Background

In C++ an object can be allocated on the stack or on the heap. When the object is allocated on the heap, we say that dynamic memory allocation is happening.

## About the code

This code is intended to see in which situations dynamic allocation is happening and in which ones is not. In several situation this is not specified by the standard and it is implementation defined.

## How to run it

Once you have Bazel in your system, you just need to run

```bash
bazel run //dynamic_memory:main
```

that will produce the following output

```bash
--------------------------------------------------------------------------------------
--------- Examples to understand when dynamic memory allocation is performed ---------
--------------------------------------------------------------------------------------
Dynamic builtin array of 42 integers
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 168
nothrow new allocation of int
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 4
Dynamic builtin array of 2 integers with extended alignment
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 8
Unique pointer of dynamic array of 2 integers
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 8
Unique pointer of struct with extended alignment
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 64
Unique pointer to int
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 4
Shared pointer to int
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 24
std::function void(int)
  - Allocation type: static
std::function void(int) calling a member function
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 24
std::string that fits for the small string optimization
  - Allocation type: static
std::string that does not fit for the small string optimization
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 62
std::string_view that does not fit for the small string optimization
  - Allocation type: static
std::vector of ints
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 32
std::vector default ctor
  - Allocation type: static
std::array of ints
  - Allocation type: static
std::thread passing void(int) function
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 24
std::jthread passing void(int) function
  - Allocation type: dynamic
  - Number of allocations: 2
  - Allocated bytes: 48
std::promise default ctor
  - Allocation type: dynamic
  - Number of allocations: 2
  - Allocated bytes: 64
promise.get_future
  - Allocation type: static
promise.set_value
  - Allocation type: static
std::future default ctor
  - Allocation type: static
std::any of 'small' object
  - Allocation type: static
std::any of 'large' object
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 32
throwing a std::runtime_error with small string literal as message
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 37
throwing a std::bad_alloc
  - Allocation type: static
Getting the current std::stacktrace
  - Allocation type: dynamic
  - Number of allocations: 1
  - Allocated bytes: 512
--------------------------------------------------------------------------------------
--------- End of the examples, feel free to add more cases ---------------------------
--------------------------------------------------------------------------------------
```
