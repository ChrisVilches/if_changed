# Agent Guidelines for if_changed

This document provides guidelines for AI agents working on the `if_changed` C++ project.
It covers build commands, code style, testing, and repository conventions.

## Project Overview

`if_changed` is a small command-line utility that tracks changes in piped input.
It stores previous content in `/tmp/if_changed_<key>` and returns exit code 0 when content
changed, 1 when unchanged (or vice versa). Used in shell pipelines to conditionally run
commands when input changes.

- **Language**: C++20 (C++23 in clangd configuration)
- **Build system**: GNU Make
- **Dependencies**: Standard library only
- **Platform**: Unix-like (uses `/tmp`)

## Build Commands

### Basic Build
```bash
make          # Builds the executable to build/if_changed
make all      # Same as default target
```

### Clean Build
```bash
make clean    # Removes the build directory
```

### Manual Compilation (for reference)
```bash
g++ -std=c++20 -Wall -Wextra -I./src -c src/main.cpp -o build/main.o
g++ -std=c++20 -Wall -Wextra -I./src -c src/io.cpp -o build/io.o
g++ -std=c++20 -Wall -Wextra -I./src -c src/lib.cpp -o build/lib.o
g++ build/*.o -o build/if_changed
```

**Note:** The Makefile uses C++20, while `.clangd` uses C++23. For consistency, new code should compile with both standards.

### Running the Program
```bash
# Example usage
echo "new content" | ./build/if_changed mykey && echo "changed"
```

## Linting and Formatting

### Code Formatting with clang-format
The project uses a `.clang-format` file based on Google style with modifications:

- **BasedOnStyle**: Google
- **ColumnLimit**: 90
- **PointerAlignment**: Left
- **DerivePointerAlignment**: false

Format all source files:
```bash
clang-format -i src/*.cpp src/*.hpp
```

### Linting with clang-tidy
No `.clang-tidy` configuration exists yet, but the `.clangd` file enforces strict warnings.
Manual linting can be done with:
```bash
clang-tidy src/*.cpp -- -std=c++20 -I./src
```

### Compiler Warnings
The `.clangd` configuration enables these flags (should be mirrored in any new compilation):
```
-std=c++23 -Wall -Wextra -Werror -pedantic -Wconversion -Wshadow -Wnon-virtual-dtor
-Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic
-Wnull-dereference -Wdouble-promotion -Wformat=2
```

Always ensure new code compiles without warnings under these flags.

## Testing

**No unit test suite is currently implemented.** The TODO list includes "add unit tests".
When adding tests, consider using a lightweight framework like Catch2 or Google Test.

### Manual Testing
Test the program manually with different inputs and keys:
```bash
# First run (no previous content) should indicate change
echo "hello" | ./build/if_changed testkey; echo $?
# Should output 0 (changed)

# Same content should indicate no change
echo "hello" | ./build/if_changed testkey; echo $?
# Should output 1 (same)

# Different content should indicate change
echo "world" | ./build/if_changed testkey; echo $?
# Should output 0 (changed)
```

## Code Style Guidelines

### File Organization
- Header files: `.hpp` extension, source files: `.cpp` extension
- Header guards: Use `#pragma once` (preferred over `#ifndef`)
- Place public interface in headers, implementation in source files
- Keep functions small and focused

### Includes Order
1. Standard library headers
2. Project headers
3. Third-party headers (none currently)

Example from `main.cpp`:
```cpp
#include <format>
#include <iostream>

#include "io.hpp"
#include "lib.hpp"
```

### Naming Conventions
- **Functions**: `snake_case` (e.g., `get_changed`, `read_file`)
- **Variables**: `snake_case` (e.g., `prev_content`, `file_path`)
- **Constants**: `snake_case` for `constexpr` variables (e.g., `program_name`)
- **Enums**: UPPER_CASE for enumerators (e.g., `SAME`, `DIFFERENT`)
- **Types**: `snake_case` (no custom types yet, but follow STL style)

### Error Handling
- Use `std::optional` for functions that may fail (e.g., `read_file`)
- Return `std::nullopt` on failure rather than throwing exceptions
- Print error messages to `std::cerr` with descriptive text
- Exit codes: 0 for success (content changed), 1 for success (unchanged), 2 for usage error

### Const Correctness
- Use `const` for parameters that aren't modified
- Use `const` for local variables that don't change after initialization
- Mark member functions as `const` when they don't modify object state

### Modern C++ Features
- Prefer `std::format` over string concatenation or printf
- Use `std::optional` for optional values
- Use `constexpr` where appropriate
- Use range-based for loops when iterating
- Use `auto` when the type is obvious or verbose

### Memory Management
- No dynamic memory allocation needed currently (uses STL containers)
- If allocation is needed, prefer RAII types (`std::unique_ptr`, `std::vector`)

### Comments
- Use `//` for single-line comments
- Place comments above the code they describe
- Avoid obvious comments; explain "why" not "what"
- Mark TODOs with `// TODO: description` (existing TODOs are in code)

## Git Conventions

### Commit Messages
- Short, descriptive present-tense messages (e.g., "add makefile", "fix formatting issue")
- No need for lengthy descriptions in this small project
- Reference issues if they exist

### Branching
- `main` is the primary branch
- Feature branches can be used for larger changes

### Ignored Files
- `build/` directory is in `.gitignore`
- No other ignored patterns currently

## Cursor and Copilot Rules

No `.cursorrules` or `.github/copilot-instructions.md` files exist. However, agents should:

1. Follow the code style outlined in this document
2. Use `#pragma once` in header files
3. Keep functions under 90 columns as per clang-format
4. Add tests when implementing new features (once test framework is added)

## Additional Notes

### Project State
- The project is minimal and functional
- Several TODOs exist in the code (check `lib.hpp`, `io.hpp`, `io.cpp`)
- Error handling for file I/O and stdin reading is incomplete
- Key validation is not implemented (should restrict to alphanumeric)

### Development Workflow
1. Make changes to source files in `src/`
2. Run `make` to rebuild
3. Test manually with sample commands
4. Format code with `clang-format`
5. Commit changes

### Future Considerations
- Add unit tests with Catch2
- Implement proper error handling for file operations
- Add key validation
- Consider persistent storage location (not just `/tmp`)
- Add command-line options (e.g., `--help`, `--version`)

## Quick Reference

| Command | Purpose |
|---------|---------|
| `make` | Build executable |
| `make clean` | Remove build artifacts |
| `clang-format -i src/*.cpp src/*.hpp` | Format code |
| `echo "content" \| ./build/if_changed key` | Test program |

