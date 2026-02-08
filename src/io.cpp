#include "io.hpp"

#include <fstream>
#include <iostream>

[[noreturn]] void handle_filesystem_errno(const std::string& path) {
  if (errno == ENOENT)
    throw std::runtime_error(format("File or directory does not exist ({})", path));
  if (errno == EACCES) throw std::runtime_error(format("Permission denied ({})", path));
  throw std::runtime_error(format("Unexpected open error ({})", path));
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

std::string read_stdin() {
  std::string result;

  std::istreambuf_iterator<char> it(std::cin), end;
  result.assign(it, end);

  // TODO: how to reproduce this???
  if (std::cin.bad()) {
    throw std::runtime_error("Failed reading stdin");
  }

  return result;
}

void write_file(const std::string& path, const std::string& data) {
  std::ofstream f(path, std::ios::binary);
  if (!f) {
    // TODO: not yet tested.
    handle_filesystem_errno(path);
  }
  f << data;
}
