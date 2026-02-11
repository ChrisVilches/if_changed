#include "lib.hpp"

#include <filesystem>
#include <format>
#include <iostream>

enum {
  SAME = 1,
  DIFFERENT = 0,
};

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

std::string get_local_state_dir(const std::string& key) {
  const char* home = std::getenv("HOME");
  if (!home) {
    throw std::runtime_error("HOME environment variable is not set");
  }
  const std::string dir = std::format("{}/.local/state/{}", home, program_name);
  std::filesystem::create_directory(dir);
  return std::format("{}/{}_{}", dir, program_name, key);
}

std::string get_full_file_path(const DIR_TYPE dir_type, const std::string& key) {
  switch (dir_type) {
    case LOCAL_STATE:
      return get_local_state_dir(key);
    default:
      return std::format("/tmp/{}_{}", program_name, key);
  }
}

std::optional<std::string> parse_key(const std::string& input) {
  if (input.empty()) {
    return std::nullopt;
  }
  for (char c : input) {
    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_') {
      return std::nullopt;
    }
  }
  return input;
}
