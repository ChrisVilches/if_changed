#pragma once

#include <optional>
#include <string>

inline constexpr const char* program_name = "if_changed";
enum {
  SAME = 1,
  DIFFERENT = 0,
};

// TODO: compare trimmed strings (for now just let the user do it himself/herself)
bool get_changed(const std::optional<std::string>& prev_content,
                 const std::string& new_content);

int get_exit_code(const bool changed);

void print_usage();

// TODO: use a permanent folder to store (should persist after a reboot).
std::string get_full_file_path(const std::string& key);
