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
( static):std::function void(int)
(dynamic):std::function void(int) calling a member function
( static):std::string that fits for the small string optimization
(dynamic):std::string that does not fit for the small string optimization
(dynamic):std::vector of ints
( static):std::array of ints
(dynamic):std::thread passing void(int) function
(dynamic):throwing and std::runtime_error
--------------------------------------------------------------------------------------
--------- End of the examples, feel free to add more cases ---------------------------
--------------------------------------------------------------------------------------
```

There is a line of each of the examples with a prefix of static or dynamic.

* static: no dynamic memory allocation has been performed
* dynamic: a dynamic memory allocation has been performed
