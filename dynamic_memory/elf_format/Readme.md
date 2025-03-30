# Example to illustrate variable location in ELF format and when program starts

## Building and introspecting the ELF file

Build the application with

```bash
bazel build //dynamic_memory/elf_format:main -c dbg
```

and then, with the following command line the binary can be introspected

```bash
readelf bazel-bin/dynamic_memory/elf_format/main --all
```

In the symbol table `.symtab` it can be seen for each symbol in which section number the syombol is located.
This information is located in the column `Ndx`.
Then in the `Section Headers`, for each section there is the name, the type, and the flags.

## Process loading into memory

If we load the program in gdb and we start breaking at the first instruction with `starti`, we can then see where the variables are located into memory.
With `info proc mappings` we can see the different memory regions that are mapped. And with `info address <variable_name>` we can see in which address variables are located into memory.
