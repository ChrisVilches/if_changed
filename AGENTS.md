# AGENTS.md - Development Guide for if_changed

This file provides instructions for agentic coding agents working on the `if_changed` repository.
It contains build commands, test instructions, and code style guidelines.

## Build System

The project uses **CMake** with C++23.

### Prerequisites
- CMake ≥ 3.16
- C++ compiler with C++23 support (g++ or clang++)
- Google Test (for testing)

### Configure and Build

```bash
# Configure with g++ (default)
cmake -B build -S . -DCMAKE_CXX_COMPILER=g++

# Build all targets
cmake --build build --target all -j$(nproc)

# Build only the main executable
cmake --build build --target if_changed

# Build only the test executable
cmake --build build --target if_changed_tests
```

### Clean

```bash
rm -rf build   # Remove the build directory
```

## Testing

The project uses **Google Test** (GTest). Tests are located in `tests/`.

### Run All Tests

```bash
cd build && ctest --verbose
# or run the test executable directly
./build/if_changed_tests
```

### Run a Single Test

Use the `--gtest_filter` flag to run specific test cases:

```bash
./build/if_changed_tests --gtest_filter="GetChangedTest.*"
./build/if_changed_tests --gtest_filter="GetChangedTest.NoPreviousContentReturnsTrue"
```

### Add New Tests

- Place test files in the `tests/` directory.
- Follow the existing naming pattern: `*_test.cpp`.
- Use `TEST(SuiteName, TestName)` macros.
- Include the relevant headers (`lib.hpp`, `io.hpp`).
- Link against GTest as shown in `CMakeLists.txt`.

## Linting and Formatting

### Clang‑Format

The code style is defined in `.clang-format` (based on **Google style** with a 90‑column limit and left pointer alignment).

To format all source files:

```bash
# Format everything in src/ and tests/
find src tests -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

You can also run a dry‑run to see what would change:

```bash
find src tests -name "*.cpp" -o -name "*.hpp" | xargs clang-format --dry-run -Werror
```

### Compiler Warnings

The project enforces strict warnings (see `.clangd`). The main flags are:

```
-std=c++23 -Wall -Wextra -Werror -pedantic -Wconversion -Wshadow
-Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused
-Woverloaded-virtual -Wpedantic -Wnull-dereference -Wdouble-promotion -Wformat=2
```

All warnings are treated as errors. Ensure your changes compile without warnings.

## Code Style Guidelines

### General Principles
- **KISS** – Keep it simple and straightforward.
- **Explicit over implicit** – Prefer clear, readable code.
- **Fail fast** – Validate inputs early and throw/return errors promptly.
- **Use standard C++23** – Leverage modern C++ features where appropriate.

### Naming Conventions
- **Functions and variables**: `snake_case`
  Example: `get_changed`, `read_stdin`, `prev_content`
- **Constants and enumerators**: `UPPER_SNAKE_CASE`
  Example: `program_name` (global constant), `SAME`, `DIFFERENT`, `DIR_TYPE::LOCAL_STATE`
- **Types (classes, enums, structs)**: `PascalCase`
  Example: `DIR_TYPE`
- **Files**: `snake_case` with `.cpp`/`.hpp` extensions

### Header Files
- Use `#pragma once` as the include guard.
- Place header comments that describe the module’s purpose.
- Keep headers minimal; forward declare when possible.
- Include order:
  1. Corresponding `.hpp` (if implementing)
  2. System/standard library headers (`<iostream>`, `<format>`, etc.)
  3. Other project headers (`"lib.hpp"`, `"io.hpp"`)

### Source Files
- Implement functions declared in the matching `.hpp`.
- Use `const` and `constexpr` liberally.
- Prefer `std::optional` for functions that may fail or return “no value”.
- Throw `std::runtime_error` for unrecoverable errors (e.g., missing environment variable).
- Use `std::format` for string formatting; avoid `printf`‑style functions.

### Error Handling
- For functions that can fail, return `std::optional<T>` or a `Result`‑like type (currently `std::optional`).
- Use exceptions only for truly exceptional conditions (e.g., missing `HOME` environment variable).
- Validate user input early and print helpful error messages to `std::cerr`.
- Exit codes:
  - `0` (DIFFERENT) – content changed
  - `1` (SAME) – content unchanged
  - `2` – usage error (invalid arguments)

### Memory and Resource Management
- Rely on RAII; avoid manual `new`/`delete`.
- Use standard containers (`std::string`, `std::vector`, etc.).
- Pass large parameters by `const std::string&` or `std::string_view` where appropriate.
- Use `std::filesystem` for file‑system operations.

### Comments and Documentation
- Use `// TODO:` comments for planned improvements (see existing TODOs in the code).
- Write concise, meaningful comments that explain **why**, not what.
- Avoid comment clutter; let the code speak for itself.
- Update the `README.md` if you change user‑facing behavior.

### Example Code Snippet

```cpp
#include "lib.hpp"

#include <filesystem>
#include <format>
#include <iostream>

bool get_changed(const std::optional<std::string>& prev_content,
                 const std::string& new_content) {
  if (!prev_content.has_value()) return true;
  return prev_content.value() != new_content;
}

int get_exit_code(const bool changed) { return changed ? DIFFERENT : SAME; }

// Throws std::runtime_error on missing HOME
std::string get_local_state_dir(const std::string& key) {
  const char* home = std::getenv("HOME");
  if (!home) {
    throw std::runtime_error("HOME environment variable is not set");
  }
  const std::string dir = std::format("{}/.local/state/{}", home, program_name);
  std::filesystem::create_directory(dir);
  return std::format("{}/{}_{}", dir, program_name, key);
}
```

## Development Workflow

1. **Before making changes** run the existing tests to ensure they pass.
2. **Implement** your feature or fix, adhering to the style guide.
3. **Format** your code with `clang‑format`.
4. **Compile** with the full warning set (CMake builds already enforce `-Werror`).
5. **Run the tests** (both the whole suite and any new tests you added).
6. **Update documentation** if needed (especially `README.md` and `TODO.md`).

## Repository Notes

- No Cursor or Copilot‑specific rules are present.
- The `.gitignore` excludes the `build/` directory and editor temporary files.
- The project is small and focused; keep changes minimal and aligned with the original purpose.

## Quick Reference

| Command | Purpose |
|---------|---------|
| `cmake -B build -S .` | Configure the project |
| `cmake --build build --target if_changed` | Build the main program |
| `./build/if_changed_tests` | Run all unit tests |
| `./build/if_changed_tests --gtest_filter="Suite.*"` | Run a subset of tests |
| `find src tests -name "*.cpp" -o -name "*.hpp" \| xargs clang-format -i` | Format all source files |
| `rm -rf build` | Clean build artifacts |

---

*This guide is intended for automated agents working on the `if_changed` repository. Please follow it to maintain consistency and quality.*
