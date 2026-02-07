#include <format>
#include <iostream>

#include "io.hpp"
#include "lib.hpp"

inline constexpr const DIR_TYPE default_dir_type = DIR_TYPE::LOCAL_STATE;

class CLIUsageError : public std::exception {
 public:
  explicit CLIUsageError(const std::string& msg) : message(std::move(msg)) {}

  const char* what() const noexcept override { return message.c_str(); }

 private:
  std::string message;
};

class CLIArgs {
 public:
  CLIArgs(const int argc, const char* const argv[]) {
    if (argc != 2) {
      throw CLIUsageError(std::format(
          "needs a unique key to identify the content (got {} arguments)", argc - 1));
    }

    key = parse_key(argv[1]);
    if (!key.has_value()) {
      throw CLIUsageError(std::format(
          "key should be non-empty alphanumeric (can contain dash and underscore)"));
    }
  }

  const std::string& get_key() const { return key.value(); }

 private:
  std::optional<std::string> key;
};

int main(const int argc, const char* const argv[]) {
  try {
    CLIArgs args(argc, argv);

    const std::string file_path = get_full_file_path(default_dir_type, args.get_key());
    const std::optional<std::string> prev_content = read_file(file_path);
    const std::string new_content = read_stdin();

    const bool changed = get_changed(prev_content, new_content);

    write_file(file_path, new_content);
    return get_exit_code(changed);
  } catch (const std::runtime_error& e) {
    std::cerr << "Unexpected error:" << std::endl;
    std::cerr << e.what() << std::endl;
    return 2;
  } catch (const CLIUsageError& e) {
    std::cerr << e.what() << "\n\n";
    print_usage();
    return 2;
  }
}
