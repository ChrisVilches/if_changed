#include "lib.hpp"

#include <gtest/gtest.h>

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

TEST(GetExitCodeTest, ChangedReturnsDifferent) {
  EXPECT_EQ(get_exit_code(true), DIFFERENT);
}

TEST(GetExitCodeTest, NotChangedReturnsSame) { EXPECT_EQ(get_exit_code(false), SAME); }

// Optional: test get_full_file_path (simple)
TEST(GetFullFilePathTest, ReturnsCorrectFormat) {
  std::string key = "mykey";
  std::string path = get_full_file_path(key);
  EXPECT_EQ(path, "/tmp/if_changed_mykey");
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