#include <cstddef>
#include <fstream>
#include <ios>
#include <sstream>

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "block_file.h"

#include <gmock/gmock-matchers.h>

#if not defined(TEST_DIR_PATH)
#define TEST_DIR_PATH "tests"
#endif

using namespace ::testing;

class BlockStreamTest : public ::testing::Test {
 public:
  boost::filesystem::path hello_world_path =
      boost::filesystem::path{TEST_DIR_PATH} / "hello_world.txt";
  block_file::BlockFileImpl file{
      boost::filesystem::directory_entry{hello_world_path}};
};

// NOLINTNEXTLINE
TEST_F(BlockStreamTest, AfterInit_NotFinished) {
  ASSERT_EQ(file.path(), hello_world_path);
  ASSERT_FALSE(file.finished());
}

// NOLINTNEXTLINE
TEST_F(BlockStreamTest, OnRead_ReturnsBlockOfSpecifiedSize) {
  {
    auto ret = file.read(1);
    ASSERT_EQ(ret, std::vector<std::byte>{std::byte{'H'}});
  }

  {
    auto ret = file.read(2);
    std::vector<std::byte> expected{std::byte{'e'}, std::byte{'l'}};
    ASSERT_EQ(ret, expected);
  }

  {
    auto ret = file.read(3);
    std::vector<std::byte> expected{std::byte{'l'}, std::byte{'o'},
                                    std::byte{','}};
    ASSERT_EQ(ret, expected);
  }

  {
    auto rest = file.read(20);  // NOLINT
    ASSERT_TRUE(file.finished());
  }
}

// NOLINTNEXTLINE
TEST_F(BlockStreamTest, EmptyFile_ByDefultFinished) {
  boost::filesystem::directory_entry empty_file{
      boost::filesystem::path{TEST_DIR_PATH} / "empty_file.txt"};
  ASSERT_TRUE(boost::filesystem::exists(empty_file));

  block_file::BlockFileImpl block_file{empty_file};

  ASSERT_EQ(block_file.path(), empty_file.path());
  ASSERT_TRUE(block_file.finished());
  ASSERT_THAT(block_file.read(42), Each(Eq(std::byte{0})));
}
