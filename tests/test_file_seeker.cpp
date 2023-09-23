#include <optional>
#include <regex>
#include <system_error>

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>

#include <gtest/gtest.h>

#include "utils/file_seeker.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-matchers.h>

#if not defined(TEST_DIR_PATH)
#define TEST_DIR_PATH "tests"
#endif

namespace fs = boost::filesystem;

using namespace ::testing;

class FilesSeekerTest : public ::testing::Test {
 public:
  void SetUp() override { ASSERT_TRUE(fs::exists(TEST_DIR)); }

  fs::path TEST_DIR = fs::path{TEST_DIR_PATH} / "for_tests";
};

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, Find_NoDepth_NoMask_NoIgnore) {
  utils::FileSeeker seeker{{TEST_DIR}, {}};
  auto res = seeker.recursive_find(0, 0);

  std::vector<fs::path> expected = {
      TEST_DIR / "file1.txt",
      TEST_DIR / "file2.bat",
      TEST_DIR / "file3.sh",
  };
  ASSERT_THAT(res, UnorderedElementsAreArray(expected));
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, Find_NoDepth_NoMask_NoIgnore_WithSize1) {
  utils::FileSeeker seeker{{TEST_DIR}, {}};
  auto res = seeker.recursive_find(0, 1);

  std::vector<fs::path> expected = {TEST_DIR / "file1.txt"};
  ASSERT_EQ(expected, res);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, Find_With1Depth_NoMask_NoIgnore) {
  utils::FileSeeker seeker{{TEST_DIR}, {}};
  auto res = seeker.recursive_find(1, 0);

  std::vector<fs::path> expected = {
      TEST_DIR / "file1.txt",
      TEST_DIR / "file2.bat",
      TEST_DIR / "file3.sh",
      TEST_DIR / "subdirectory" / "file4.sh",
      TEST_DIR / "subdirectory" / "file5.data",
      TEST_DIR / "ignore" / "ignored.txt",
  };
  ASSERT_THAT(res, UnorderedElementsAreArray(expected));
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, Find_WithFullDepth_WithMask_NoIgnore) {
  constexpr auto FULL_TEST_DEPTH = 4;

  utils::FileSeeker seeker{{TEST_DIR}, {}};
  auto res = seeker.recursive_find(FULL_TEST_DEPTH, 0, ".*\\.hex");

  std::vector<fs::path> expected = {TEST_DIR / "subdirectory" /
                                    "third_subdirectory" / "file8.hex"};
  ASSERT_EQ(expected, res);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnNoSuchPath_ThrowError) {
  fs::path bad_dir_path = fs::path{TEST_DIR_PATH} / "dummy_bad_path";
  ASSERT_FALSE(fs::exists(bad_dir_path));

  utils::FileSeeker seeker{{bad_dir_path}, {}};
  ASSERT_THROW(seeker.recursive_find(0, 0),
               boost::filesystem::filesystem_error);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnBadRegex_ThrowError) {
  utils::FileSeeker seeker{{TEST_DIR}, {}};
  ASSERT_THROW(seeker.recursive_find(0, 0, "ba(d"), std::regex_error);
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, Find_With1Depth_WithIgnore_NoMask) {
  const auto INGORE_DIR = TEST_DIR / "ignore";
  utils::FileSeeker seeker{{TEST_DIR}, {INGORE_DIR}};
  auto res = seeker.recursive_find(1, 0);

  std::vector<fs::path> expected = {
      TEST_DIR / "file1.txt",
      TEST_DIR / "file2.bat",
      TEST_DIR / "file3.sh",
      TEST_DIR / "subdirectory" / "file4.sh",
      TEST_DIR / "subdirectory" / "file5.data",
  };
  ASSERT_THAT(res, UnorderedElementsAreArray(expected));
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnFind_InIgnoredPath_ReturnNothing) {
  const auto INGORED_DIR = TEST_DIR / "ignore";
  const auto SUB_INGORED_DIR = TEST_DIR / "ignore" / "sub_ignored";

  utils::FileSeeker seeker{{SUB_INGORED_DIR}, {INGORED_DIR}};
  auto res = seeker.recursive_find(1, 0);

  ASSERT_THAT(res, IsEmpty());
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnNoScanDirectory_ReturnNothing) {
  utils::FileSeeker seeker{{}, {}};
  auto res = seeker.recursive_find(1, 0);
  ASSERT_THAT(res, IsEmpty());
}

// NOLINTNEXTLINE
TEST_F(FilesSeekerTest, OnFind_WithMultipleScanDirectories_ReturnAllFiles) {
  const auto FIRST_SCAN_DIR = TEST_DIR / "subdirectory" / "second_subdirectory";
  const auto SECOND_SCAN_DIR = TEST_DIR / "subdirectory" / "third_subdirectory";

  utils::FileSeeker seeker{{FIRST_SCAN_DIR, SECOND_SCAN_DIR}, {}};
  auto res = seeker.recursive_find(1, 0);

  std::vector<fs::path> expected = {
      FIRST_SCAN_DIR / "file6.foo",
      FIRST_SCAN_DIR / "file7.bar",
      SECOND_SCAN_DIR / "file8.hex",
  };
  ASSERT_THAT(res, UnorderedElementsAreArray(expected));
}