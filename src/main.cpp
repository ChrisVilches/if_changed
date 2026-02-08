#include <iostream>

#include "cli.hpp"
#include "io.hpp"
#include "lib.hpp"

inline constexpr const DIR_TYPE default_dir_type = DIR_TYPE::LOCAL_STATE;

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
