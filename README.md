# if_changed

A minimal command-line utility that tracks changes in content identified by a key.
Returns exit code 0 when content changed, 1 when unchanged.
Useful in shell pipelines to conditionally run commands when content changes.

## Build

```bash
# Configure with CMake (generated files contain settings for LSP, which will be read automatically by clangd)
cmake -B build -S . -DCMAKE_CXX_COMPILER=g++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build everything
cmake --build build --target all -j4

# Or build specific targets
cmake --build build --target if_changed      # main program
cmake --build build --target if_changed_tests # tests
```

## Test

```bash
# Run tests with ctest
cd build && ctest --verbose

# Or run test executable directly
./build/if_changed_tests
```

## Usage

```bash
# First run (no previous content) → exit code 0 (changed)
echo "hello" | ./build/if_changed testkey; echo $?

# Same content → exit code 1 (same)
echo "hello" | ./build/if_changed testkey; echo $?

# Different content → exit code 0 (changed)
echo "world" | ./build/if_changed testkey; echo $?

# Use in pipelines
echo "new data" | ./build/if_changed mykey && echo "changed, run next command"
```

## Clean

```bash
rm -rf build  # Remove CMake build directory
```
