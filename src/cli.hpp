#pragma once
#include <exception>
#include <string>

class CLIUsageError : public std::exception {
 public:
  explicit CLIUsageError(const std::string& msg);
  const char* what() const noexcept override;

 private:
  std::string message;
};

class CLIArgs {
 public:
  CLIArgs(const int argc, const char* const argv[]);
  const std::string& get_key() const;

 private:
  std::string key;
};
