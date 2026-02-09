#include "lib.hpp"

#include <iostream>

bool get_changed(const std::optional<std::string>& prev_content,
                 const std::string& new_content) {
  if (!prev_content.has_value()) return true;
  return prev_content.value() != new_content;
}

int get_exit_code(const bool changed) { return changed ? DIFFERENT : SAME; }

void print_usage() {
  std::cerr << "Usage:" << std::endl;
  std::cerr << std::format("echo \"new content\" | {} <key> && next_program",
                           program_name)
            << std::endl;
  std::cerr << "(`next_program` will be executed when the content changes)" << std::endl;
}

std::string get_full_file_path(const std::string& key) {
  return format("/tmp/if_changed_{}", key);
}
