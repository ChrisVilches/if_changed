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