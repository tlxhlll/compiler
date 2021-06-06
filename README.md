## Introduction

An SPL(Simple Pascal Language) compiler for compiler course.

## Timeline

### 1 Before the release of syntax rule of SPL from teacher(5.1-5.31)

1.1 Learn how to colaborate on GitHub

1.2 Learn Google's coding specification instruction

1.3 Learn LLVM IR API

### 2 After downloading the syntax rule doc(6.1-6.3)

2.1 Write lex and yacc(scanner and parser)

2.2 Review knowledge of symbol table and type checking

2.3 Learn syntax of Pascal

### 3 Last 4 days(6.4-6.7)

3.1 Write the 3 desired test programs

3.2 Modify the 3 test programs to adapt to our compiler

3.3 Write ast and codeGen

3.4 Debug

3.4.1 incomplete global function

## Build
### Requirements
Environment: Linux (Ubuntu 20.04)
CMake: >3 (3.16.3)
LLVM: 9 or 10
Flex: 2.6.4
Bison: 3.5.1
Clang: 10

### Run
mkdir build
cd build
cmake ..
make
cd ../test
./clean.sh
./gen.sh

