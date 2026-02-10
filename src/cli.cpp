#include "cli.hpp"

#include <format>

#include "lib.hpp"

CLIUsageError::CLIUsageError(const std::string& msg) : message(std::move(msg)) {}

const char* CLIUsageError::what() const noexcept { return message.c_str(); }

CLIArgs::CLIArgs(const int argc, const char* const argv[]) {
  if (argc != 2) {
    throw CLIUsageError(std::format(
        "needs a unique key to identify the content (got {} arguments)", argc - 1));
  }

  const std::optional<std::string> key_opt = parse_key(argv[1]);
  if (!key_opt.has_value()) {
    throw CLIUsageError(
        "key should be non-empty alphanumeric (can contain dash and underscore)");
  }

  this->key = key_opt.value();
}

const std::string& CLIArgs::get_key() const { return key; }
