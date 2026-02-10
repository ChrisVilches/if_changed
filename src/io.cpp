#include "io.hpp"

#include <fstream>
#include <iostream>

enum class Operation { Read, Write, Execute };

const char* operation_to_str(const Operation op) {
  switch (op) {
    case Operation::Read:
      return "read";
    case Operation::Write:
      return "write";
    case Operation::Execute:
      return "execute";
  }
}

[[noreturn]] void handle_filesystem_errno(const std::string& path, const Operation op) {
  std::string msg;

  const char* op_str = operation_to_str(op);

  if (errno == ENOENT) {
    msg = format("File or directory does not exist ({}): {}", op_str, path);
  } else if (errno == EACCES) {
    msg = format("Permission denied ({}): {}", op_str, path);
  } else {
    msg = format("Unexpected open error ({}): {}", op_str, path);
  }

  throw std::runtime_error(msg);
}

std::optional<std::string> read_file(const std::string& path) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    if (errno == ENOENT) return std::nullopt;
    handle_filesystem_errno(path, Operation::Read);
  }
  return std::string((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
}

// NOTE: Reading from stdin rarely fails (and an error is hard to reproduce).
std::string read_stdin() {
  return std::string(std::istreambuf_iterator<char>(std::cin),
                     std::istreambuf_iterator<char>());
}

void write_file(const std::string& path, const std::string& data) {
  std::ofstream f(path, std::ios::binary);
  if (!f) {
    handle_filesystem_errno(path, Operation::Write);
  }
  f << data;
}
