# bfc
bfc is an educational Brainfuck compiler split into two projects. The non-optimizing compiler demonstrates how simple 
it is to translate brainfuck to a higher level language like C; and the optimizing compiler serves as a learning 
sandbox to experiment with compiler optimizations. bfc reads Brainfuck code from `stdin` and outputs the compiled form
to `stdout`. Currently the only compiled output is a translation from Brainfuck to C; assembly and compiled binaries
are planned for the future.

### building bfc
bfc has only been tested with clang-3.5 on x86_64 Ubuntu

Building both versions of the compiler:

```
$ cd bf-compiler
$ make
```

Building the optimizing version of the compiler:

```
$ cd bf-compiler/optimizing
$ make
```

Building the non-optimzing version of the compiler:

```
$ cd bf-compiler/non-optimizing
$ make
```


### usage
```
$ ./bfc < input-file.b > output-file.c
$ cc -O0 output-file.c -o output-file
```


### optimizations implemented
- Arithmetic/constant folding
