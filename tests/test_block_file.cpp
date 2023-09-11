#include <cstddef>
#include <sstream>

#include <gtest/gtest.h>

#include "block_file.h"

class BlockStreamTest : public ::testing::Test {
 public:
  std::stringstream string_file{"Hello, World!"};
  block_file::BlockStream<std::stringstream> file{std::move(string_file),
                                                  string_file.str().size()};
};

// NOLINTNEXTLINE
TEST_F(BlockStreamTest, AfterInit_NotFinished) {
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
    auto rest = file.read(20); // NOLINT
    ASSERT_TRUE(file.finished());
  }
}

// NOLINTNEXTLINE
TEST_F(BlockStreamTest, EmptyFileByDefaultFinished) {
  std::stringstream empty{};
  block_file::BlockStream file{std::move(empty), empty.str().size()};
  ASSERT_TRUE(file.finished());
}
