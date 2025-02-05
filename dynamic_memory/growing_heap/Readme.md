# Example to illustrate how the stack is growing

Run the application with

```bash
bazel run //dynamic_memory/growing_stack:main
```

and choos which option do you want:

- a: Dynamically llocate a given amount of bytes of memory
- i: Initialize memory allocated until now
- p: Print the pointer addresses of the memory chunks dynamically allocated until now
- d: Deallocate a bunch of memory (Not yet implemented)
