#include "io.hpp"

#include <fstream>
#include <iostream>

// TODO: this isn't very good, since we don't know what was the problem (read permission?
// write permission? execution?). It's necessary to tell the user in what situation the
// error happened.
[[noreturn]] void handle_filesystem_errno(const std::string& path) {
  if (errno == ENOENT) {
    throw std::runtime_error(format("File or directory does not exist ({})", path));
  } else if (errno == EACCES) {
    throw std::runtime_error(format("Permission denied ({})", path));
  } else {
    throw std::runtime_error(format("Unexpected open error ({})", path));
  }
}

std::optional<std::string> read_file(const std::string& path) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    if (errno == ENOENT) return std::nullopt;
    handle_filesystem_errno(path);
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
    handle_filesystem_errno(path);
  }
  f << data;
}
