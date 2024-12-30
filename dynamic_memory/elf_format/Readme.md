# Example to illustrate variable location in ELF format

Build the application with

```bash
bazel build //dynamic_memory/elf_format:main -c dbg --copt=-Wno-error
```

and then, with the following command line the binary can be introspected

```bash
readelf bazel-bin/dynamic_memory/elf_format/main --all
```

In the symbol table `.symtab` it can be seen for each symbol in which section number the syombol is located.
This information is located in the column `Ndx`.
Then in the `Section Headers`, for each section there is the name, the type, and the flags.
