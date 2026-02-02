#include <format>
#include <iostream>

#include "io.hpp"
#include "lib.hpp"

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << std::format(
        "needs a unique key to identify the content (got {} arguments)\n\n", argc - 1);
    print_usage();
    return 2;
  }

  const std::string key(argv[1]);

  const std::string file_path = get_full_file_path(key);
  const std::optional<std::string> prev_content = read_file(file_path);
  const std::string new_content = read_stdin();

  const bool changed = get_changed(prev_content, new_content);

  write_file(file_path, new_content);

  return get_exit_code(changed);
}
