#include "io.hpp"

#include <fstream>
#include <iostream>

// TODO: remove this
std::optional<std::string> read_file(const std::string& path, const int x) {
  std::cout << "remove this " << x << std::endl;
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    return std::nullopt;
  }
  return std::string((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
}

// TODO: it's possible that reading from stdin fails. So how do I handle that?
std::string read_stdin() {
  return std::string(std::istreambuf_iterator<char>(std::cin),
                     std::istreambuf_iterator<char>());
}

// TODO: It's possible that this fails. Handle
void write_file(const std::string& path, const std::string& data) {
  std::ofstream f(path, std::ios::binary);
  f << data;
}
