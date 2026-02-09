#include <optional>
#include <string>

// TODO: also add the permission check (must print error if the file isn't accessible).
std::optional<std::string> read_file(const std::string& path);

// TODO: it's possible that reading from stdin fails. So how do I handle that?
std::string read_stdin();

// TODO: It's possible that this fails. Handle
void write_file(const std::string& path, const std::string& data);
