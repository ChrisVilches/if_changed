# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I./src
LDFLAGS =

# Target executable
TARGET = build/if_changed

# Object files
OBJS = build/main.o build/io.o build/lib.o

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS) | build
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files to object files
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
build:
	mkdir -p build

# Clean build artifacts
clean:
	rm -rf build

# Phony targets
.PHONY: all clean