# newrepo
# Lab-13: Fix My Build — Translation Units & the C++ Build Pipeline

## What this lab is about

Every lab you have done this semester followed the same pattern: you received a locked `.h` file, implemented a `.cpp` file, and ran `make test_partN`. The build system was handed to you. You never had to think about *why* the code compiles, *how* multiple `.cpp` files become one executable, or *what* the linker actually does.

That changes today.

You are receiving a small multi-file project — an inventory management system with a few classes spread across header and source files. The code is logically correct. The *program* is correct. But the *project* is broken. It will not compile. And even after you fix the build errors, it will crash at runtime with memory bugs.

Your job is to diagnose and fix every issue, working through the same pipeline the compiler does:

```
Source files → Preprocessor → Compiler → Linker → Executable → Runtime
```

Each part of this lab isolates one phase of that pipeline. You will read real compiler and linker error messages, figure out what they mean, and fix the code. The automated test suite will verify your fixes — when you see all `PASS` for a part, move on.

---

## Why this matters

When you work on any project larger than a single file — which is every real project — you will hit these errors. They look terrifying the first time. Linker errors in particular are infamous for producing long, mangled messages that seem to say nothing useful.

But every single one of these errors has a mechanical explanation. The compiler is not being cryptic — it is telling you exactly what went wrong, in its own language. After today, you will speak enough of that language to debug any build failure you encounter.

---

## Files

```
lab-13/
├── Item.h               ← buggy — you fix this
├── Item.cpp             ← buggy — you fix this
├── Warehouse.h          ← buggy — you fix this
├── Warehouse.cpp        ← buggy — you fix this
├── PriceUtils.h         ← buggy — you fix this
├── PriceUtils.cpp       ← buggy — you fix this
├── Report.h             ← clean — do not modify
├── Report.cpp           ← clean — do not modify
├── main.cpp             ← clean — do not modify
├── Makefile             ← buggy — you fix this
├── expected_output.txt  ← what the program should print when everything is fixed
└── README.md            
```

Every file you touch should be marked with a `// FIX:` comment explaining what you changed and why.


## How to work through this lab

**Do not** try to fix everything at once. Work through one part at a time. Each part has its own make target:

```bash
make test_part1      # Preprocessor fixes only
make test_part2      # Compilation fixes only
make test_part3      # Linker fixes only
make test_part4      # Runtime fixes (AddressSanitizer)
make test_all        # Full build + all tests
```

Each target compiles only the subset of code relevant to that part. When you see `PASS` for every test in a part, move on to the next.

Read the error messages carefully. The compiler is telling you the file, the line number, and the nature of the problem. Your first instinct should be to *read the message*, not to guess.


## Part 1 — Preprocessor (Header Guards & Includes)

**What you are fixing**: problems that occur *before* compilation — during the preprocessor's textual inclusion step.

When you write `#include "Item.h"`, the preprocessor literally copies the entire content of `Item.h` into your `.cpp` file. If two files both include the same header, and that header has no include guard, the compiler sees the same type definition twice and rejects it.

**Bugs planted in this part:**

- A header file is missing its include guard entirely. Any `.cpp` that includes it through two different paths will trigger a redefinition error.
- A header is missing a necessary `#include`, so a type it uses is unknown when that header is included on its own.
- Two headers include each other (circular dependency). The include guard on one of them causes the other's type definition to be skipped, leaving the type incomplete exactly where it is needed.

**What to look for**: error messages mentioning "redefinition of", "was not declared in this scope", or "incomplete type".

**Hint for the circular dependency**: if you add a missing `#include` to fix the "unknown type" error and the *same* error persists, a cycle is the reason — the guard fires before the definition is reached.

Run:
```bash
make test_part1
```


## Part 2 — Compilation (Forward Declarations vs Full Definitions)

**What you are fixing**: problems within a single translation unit — one `.cpp` file and everything it includes.

A forward declaration (`class Item;`) tells the compiler "this class exists" but nothing else. You can declare a *pointer* or *reference* to a forward-declared class. You cannot construct it, call its methods, or ask for its size — the compiler does not have enough information.

**Bugs planted in this part:**

- Code tries to call a method on a type that was only forward-declared, not fully included.
- Code tries to use `sizeof` on an incomplete type.

**What to look for**: error messages mentioning "incomplete type", "member access into incomplete type", or "invalid application of sizeof".

Run:
```bash
make test_part2
```


## Part 3 — Linking (Across Translation Units)

**What you are fixing**: problems that only appear when the linker tries to combine multiple `.o` files into one executable.

Each `.cpp` file is compiled independently into an object file (`.o`). The linker's job is to connect them: when `main.o` calls `Item::getPrice()`, the linker finds the actual compiled code for that function in `Item.o`. If it cannot find it, you get an "undefined reference." If it finds it in *two* object files, you get a "multiple definition."

**Bugs planted in this part:**

- A non-inline function is defined in a `.h` file. Every `.cpp` that includes it gets its own copy of that definition. The linker sees duplicate symbols and rejects them. Multiple definition error.
- A `.cpp` file is missing from the Makefile's object list. Its functions are never compiled into any `.o`, so the linker cannot find them. Undefined reference error.
- A template function has its implementation in a `.cpp` file instead of the header. The compiler cannot instantiate a template it cannot see at the call site. Undefined reference at link time.

**What to look for**: error messages mentioning "undefined reference to" or "multiple definition of". Pay attention to *which symbols* are reported — the symbol name tells you which function or method is affected.

**Important**: one of these bugs is in the `Makefile`, not in the C++ source files.

Run:
```bash
make test_part3
```


## Part 4 — Runtime (Memory Bugs with AddressSanitizer)

**What you are fixing**: the code compiles and links cleanly, but it has undefined behavior that corrupts memory at runtime.

Until now, you have been using `-fsanitize=address` in every lab without thinking about it much. Today you will see exactly what it catches and how to read its output.

AddressSanitizer (ASan) instruments your program at compile time to detect memory errors at runtime. When it catches one, it prints a detailed report: what kind of error, where it happened (stack trace), and where the memory was originally allocated or freed.

**Bugs planted in this part:**

1. **Stack buffer overflow.** An off-by-one error writes past the end of a stack-allocated C-style array. This one is the easiest to spot in ASan output — use it as your warm-up.

2. **Double free.** A class manages a dynamically allocated resource with `new`/`delete`, but its copy constructor was never written. The compiler-generated copy does a shallow copy (copies the pointer, not the data). When both the original and the copy are destroyed, they both call `delete` on the same address.

3. **Use-after-free via vector reallocation.** A pointer is taken to an element inside a `std::vector`. Later, the vector grows (via `push_back`), which may reallocate its internal buffer to a new address. The old pointer now points to freed memory. This is the most common real-world C++ memory bug you will encounter. It looks perfectly fine locally — the bug is in the *interaction* between the pointer and the container's lifecycle.

**How to work through this part:**

First, compile and run normally (without ASan) and observe that the program may *appear* to work. This is the danger of undefined behavior — it can seem correct by accident.

Then compile with ASan enabled and run again:

```bash
make test_part4
```

Fix each bug. The test suite will verify that the memory errors are resolved.


## How to read an AddressSanitizer report

ASan output looks intimidating the first time. It is not. Every report follows the same structure, and once you know how to read one, you can read all of them.

Here is a real example of what you will see:

```
=================================================================
==12345==ERROR: AddressSanitizer: heap-use-after-free on address 0x60200000ef10
    at pc 0x00010a3b READ of size 8
    #0 0x00010a3a in Warehouse::printFirstItem() Warehouse.cpp:42
    #1 0x00010b7c in main main.cpp:18
    #2 0x7fff2039 in _start

0x60200000ef10 is located 0 bytes inside of 32-byte region [0x60200000ef10, 0x60200000ef30)
freed by thread T0 here:
    #0 0x000109a2 in operator delete(void*)
    #1 0x00010a10 in std::vector<Item>::_M_realloc_insert(...) vector:1234

previously allocated by thread T0 here:
    #0 0x00010982 in operator new(unsigned long)
    #1 0x000109f0 in std::vector<Item>::push_back(...) vector:1100
```

Read it in three blocks:

**Block 1 — What happened.** The first line after `ERROR:` tells you the type of bug. In this case, `heap-use-after-free` — the program read memory that was already freed. The stack trace below it (`#0`, `#1`, `#2`) tells you *where* the bad access happened. Read `#0` first — it gives you the file and line number (`Warehouse.cpp:42`). That is where your code touched dead memory.

**Block 2 — When was it freed.** The `freed by thread T0 here:` section tells you *when and where* the memory was deallocated. In this example, `std::vector::_M_realloc_insert` freed it — meaning the vector reallocated its internal buffer. This is your "why": the vector grew, moved its data to a new buffer, and freed the old one. Any pointer you saved before the growth is now dangling.

**Block 3 — When was it allocated.** The `previously allocated by thread T0 here:` section tells you where the memory was originally created. This helps you trace the full lifecycle: allocated here, freed here, accessed (illegally) here.

For **double-free** errors, the structure is similar but the first line will say `attempting double-free on 0x...` and the report will show you both the first `free` and the second (illegal) `free`.

For **stack-buffer-overflow** errors, the report will say `stack-buffer-overflow on address 0x...` and point you directly to the array and the out-of-bounds index. These are the most straightforward to fix.

**The key habit**: always start at `#0` in the first stack trace. That is your file and line number. Then read the `freed by` or `allocated by` sections to understand the lifecycle. Do not try to interpret the hex addresses — they are not useful to you. Focus on the file names and line numbers.


## Build reference

The full compilation pipeline for this project (when everything is fixed):

```bash
# Preprocess (you will not run this, but this is what happens first)
g++ -E Item.cpp           # outputs the fully expanded translation unit

# Compile each TU to an object file
g++ -std=c++17 -Wall -Wextra -fsanitize=address -c Item.cpp -o Item.o
g++ -std=c++17 -Wall -Wextra -fsanitize=address -c Warehouse.cpp -o Warehouse.o
g++ -std=c++17 -Wall -Wextra -fsanitize=address -c PriceUtils.cpp -o PriceUtils.o
g++ -std=c++17 -Wall -Wextra -fsanitize=address -c Report.cpp -o Report.o
g++ -std=c++17 -Wall -Wextra -fsanitize=address -c main.cpp -o main.o

# Link all object files into one executable
g++ -fsanitize=address -o inventory main.o Item.o Warehouse.o PriceUtils.o Report.o

# Run
MallocNanoZone=0 ./inventory
```

You can run `g++ -E <file>.cpp` on any source file to see the preprocessor output. This is useful for debugging Part 1 — you can see exactly what the compiler sees after all `#include` directives are resolved.


## Submission

Submit the entire `lab-13/` directory with all your fixes.

