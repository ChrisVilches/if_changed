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

  const std::optional<std::string> key = parse_key(argv[1]);
  if (!key.has_value()) {
    std::cerr << std::format(
        "key should be non-empty alphanumeric (can contain dash and underscore)\n\n");
    print_usage();
    return 2;
  }
  const std::string file_path = get_full_file_path(key.value());
  const std::optional<std::string> prev_content = read_file(file_path);
  const std::string new_content = read_stdin();

  const bool changed = get_changed(prev_content, new_content);

  write_file(file_path, new_content);

  return get_exit_code(changed);
}
