#pragma once

#include <optional>
#include <string>

inline constexpr const char* program_name = "if_changed";

enum DIR_TYPE { TMP, LOCAL_STATE };

bool get_changed(const std::optional<std::string>& prev_content,
                 const std::string& new_content);

int get_exit_code(const bool changed);

void print_usage();

std::string get_full_file_path(const DIR_TYPE dir_type, const std::string& key);

std::optional<std::string> parse_key(const std::string& input);
