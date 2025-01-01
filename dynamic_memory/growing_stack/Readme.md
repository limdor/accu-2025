# Example to illustrate how the stack is growing

Build the application with

```bash
bazel build //dynamic_memory/growing_stack:main -c dbg
```

then start it in one console

```bash
./bazel-bin/dynamic_memory/growing_stack/main
```

and in the other start gdb and attach to the started process.

Check the size of the memory reserved for the stack with `info proc mappings`.
Introduce a number that is high enough like `20000` and after that check again `info proc mappings` and see how the size of the memory reserved for the stack has grown.
After that, we can see that the stack pointer gets decremented but the memory reserved for the stack does not shrink.

Breakpoint can be set on a specific line with condition with `break main.cpp:5 if n == 0`.
