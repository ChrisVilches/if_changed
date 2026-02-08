#pragma once

#include <optional>
#include <string>

std::optional<std::string> read_file(const std::string& path);

std::string read_stdin();

void write_file(const std::string& path, const std::string& data);
