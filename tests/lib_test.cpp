#include "lib.hpp"

#include <gtest/gtest.h>

#include <format>

TEST(GetChangedTest, NoPreviousContentReturnsTrue) {
  std::optional<std::string> prev = std::nullopt;
  std::string new_content = "hello";
  EXPECT_TRUE(get_changed(prev, new_content));
}

TEST(GetChangedTest, SameContentReturnsFalse) {
  std::optional<std::string> prev = std::string("hello");
  std::string new_content = "hello";
  EXPECT_FALSE(get_changed(prev, new_content));
}

TEST(GetChangedTest, DifferentContentReturnsTrue) {
  std::optional<std::string> prev = std::string("hello");
  std::string new_content = "world";
  EXPECT_TRUE(get_changed(prev, new_content));
}

TEST(GetChangedTest, EmptyStringSame) {
  std::optional<std::string> prev = std::string("");
  std::string new_content = "";
  EXPECT_FALSE(get_changed(prev, new_content));
}

TEST(GetChangedTest, EmptyStringDifferent) {
  std::optional<std::string> prev = std::string("");
  std::string new_content = "not empty";
  EXPECT_TRUE(get_changed(prev, new_content));
}

TEST(GetExitCodeTest, ChangedExitCode) { EXPECT_EQ(get_exit_code(true), 0); }

TEST(GetExitCodeTest, SameExitCode) { EXPECT_EQ(get_exit_code(false), 1); }

TEST(GetFullFilePathTest, ReturnsCorrectFormatTemp) {
  std::string key = "mykey";
  std::string path = get_full_file_path(DIR_TYPE::TMP, key);
  EXPECT_EQ(path, "/tmp/if_changed_mykey");
}

TEST(GetFullFilePathTest, ReturnsCorrectFormatLocal) {
  std::string key = "some_key";
  std::string path = get_full_file_path(DIR_TYPE::LOCAL_STATE, key);
  const char* home = std::getenv("HOME");
  EXPECT_EQ(path, std::format("{}/.local/state/if_changed/if_changed_some_key", home));
}

TEST(ParseKeyTest, ValidAlphanumericReturnsKey) {
  std::optional<std::string> result = parse_key("abc123");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "abc123");
}

TEST(ParseKeyTest, ValidWithDashAndUnderscoreReturnsKey) {
  std::optional<std::string> result = parse_key("abc-123_def");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "abc-123_def");
}

TEST(ParseKeyTest, EmptyStringReturnsNullopt) {
  std::optional<std::string> result = parse_key("");
  EXPECT_FALSE(result.has_value());
}

TEST(ParseKeyTest, InvalidSymbolReturnsNullopt) {
  std::optional<std::string> result = parse_key("abc@123");
  EXPECT_FALSE(result.has_value());
}

TEST(ParseKeyTest, InvalidSpaceReturnsNullopt) {
  std::optional<std::string> result = parse_key("abc 123");
  EXPECT_FALSE(result.has_value());
}

TEST(ParseKeyTest, InvalidDotReturnsNullopt) {
  std::optional<std::string> result = parse_key("abc.123");
  EXPECT_FALSE(result.has_value());
}

TEST(ParseKeyTest, InvalidSlashReturnsNullopt) {
  std::optional<std::string> result = parse_key("abc/123");
  EXPECT_FALSE(result.has_value());
}

TEST(ParseKeyTest, OnlyDashReturnsKey) {
  std::optional<std::string> result = parse_key("-");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "-");
}

TEST(ParseKeyTest, OnlyUnderscoreReturnsKey) {
  std::optional<std::string> result = parse_key("_");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), "_");
}
