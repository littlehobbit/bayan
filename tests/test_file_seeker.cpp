#include <optional>
#include <regex>
#include <system_error>

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>

#include <gtest/gtest.h>

#include "utils.h"

#if not defined(TEST_DIR_PATH)
#define TEST_DIR_PATH "tests"
#endif

namespace fs = boost::filesystem;

using namespace ::testing;

class FilesSeekerTest : public ::testing::Test {
 public:
  void SetUp() override { ASSERT_TRUE(fs::exists(test_dir_path)); }

  fs::path test_dir_path = fs::path{TEST_DIR_PATH} / "for_tests";
};

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, RecursiveFind_NoDepth_NoMask) {
  auto res = utils::recursive_find(test_dir_path, 0, std::nullopt);

  std::vector<fs::path> expected = {
      test_dir_path / "file1.txt",
      test_dir_path / "file2.bat",
      test_dir_path / "file3.sh",
  };
  ASSERT_EQ(expected, res);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, RecursiveFind_With1Depth_NoMask) {
  auto res = utils::recursive_find(test_dir_path, 1, std::nullopt);

  std::vector<fs::path> expected = {
      test_dir_path / "file1.txt",
      test_dir_path / "file2.bat",
      test_dir_path / "file3.sh",
      test_dir_path / "subdirectory" / "file4.sh",
      test_dir_path / "subdirectory" / "file5.data",
  };
  ASSERT_EQ(expected, res);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, RecursiveFind_WithFullDepth_WithMask) {
  constexpr auto FULL_TEST_DEPTH = 4;
  auto res = utils::recursive_find(test_dir_path, FULL_TEST_DEPTH, ".*\\.hex");

  std::vector<fs::path> expected = {test_dir_path / "subdirectory" /
                                    "third_subdirectory" / "file8.hex"};
  ASSERT_EQ(expected, res);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnNoSuchPath_ThrowError) {
  fs::path bad_dir_path = fs::path{TEST_DIR_PATH} / "dummy_bad_path";
  ASSERT_FALSE(fs::exists(bad_dir_path));

  ASSERT_THROW(utils::recursive_find(bad_dir_path, 0, std::nullopt),
               boost::filesystem::filesystem_error);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnBadRegex_ThrowError) {
  ASSERT_THROW(utils::recursive_find(test_dir_path, 0, "ba(d"),
               std::regex_error);
}
