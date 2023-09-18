#include <algorithm>
#include <cstddef>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bayan.h"
#include "block.h"
#include "block_file.h"
#include "hash/hash_algorithm.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>

class MockHashAlgorithm : public hash::HashAlgorithm {
 public:
  MOCK_METHOD(std::vector<std::uint8_t>, hash, (const Block& block),
              (const, noexcept, override));
};

class MockBlockFile : public block_file::BlockFile {
 public:
  MOCK_METHOD(Block, read, (std::size_t block_size), (noexcept, override));

  MOCK_METHOD(const boost::filesystem::path&, path, (),
              (const, noexcept, override));

  MOCK_METHOD(bool, finished, (), (const, noexcept, override));
};

using namespace ::testing;

class BayanTest : public ::testing::Test {
 public:
  auto create_file_block(std::size_t block_size,
                         const std::vector<std::byte>& content)
      -> std::vector<std::byte> {
    std::vector<std::byte> block{};
    block.resize(block_size);
    std::copy(content.cbegin(), content.cend(), block.begin());
    return block;
  }

  static constexpr auto BLOCK_SIZE = 512;
  MockHashAlgorithm hash_mock{};
  bayan::BayanFinder finder{hash_mock, BLOCK_SIZE};
};

// NOLINTNEXTLINE
TEST_F(BayanTest, OnFindDuplicatesFor1File_DoNothing_ReturnEmpty) {
  // One file can not have duplicates
  auto file = MockBlockFile{};
  EXPECT_CALL(file, read).Times(0);  // no reads for file

  EXPECT_CALL(hash_mock, hash).Times(0);

  auto res = finder.find_duplicates({&file});
  ASSERT_TRUE(res.empty());
}

// NOLINTNEXTLINE
TEST_F(BayanTest, For2SameFile_WithSameSize_ReturnAsDuplicates) {
  const auto first_file_path = boost::filesystem::path{"first.txt"};
  auto first_file = MockBlockFile{};
  EXPECT_CALL(first_file, path).WillRepeatedly(ReturnRef(first_file_path));

  const auto second_file_path = boost::filesystem::path{"second.txt"};
  auto second_file = MockBlockFile{};
  EXPECT_CALL(second_file, path).WillRepeatedly(ReturnRef(second_file_path));

  EXPECT_CALL(first_file, finished).WillRepeatedly(Return(true));
  EXPECT_CALL(second_file, finished).WillRepeatedly(Return(true));

  // NOLINTNEXTLINE
  auto file_content = create_file_block(BLOCK_SIZE, {std::byte{0x42}});

  EXPECT_CALL(first_file, read(BLOCK_SIZE)).WillOnce(Return(file_content));
  EXPECT_CALL(second_file, read(BLOCK_SIZE)).WillOnce(Return(file_content));

  std::vector<std::uint8_t> hash{0x42, 0x24};  // NOLINT
  EXPECT_CALL(hash_mock, hash(file_content))
      .Times(2)
      .WillRepeatedly(Return(hash));

  auto res = finder.find_duplicates({&first_file, &second_file});
  ASSERT_EQ(res.size(), 1);
  ASSERT_THAT(res[0], UnorderedElementsAre(&first_file, &second_file));
}

// NOLINTNEXTLINE
TEST_F(BayanTest, For2DifferentFiles_WithSameSize_ReturnNothing) {
  auto first_file = MockBlockFile{};
  auto second_file = MockBlockFile{};

  EXPECT_CALL(first_file, finished).WillRepeatedly(Return(true));
  EXPECT_CALL(second_file, finished).WillRepeatedly(Return(true));

  // NOLINTNEXTLINE
  auto first_block = create_file_block(BLOCK_SIZE, {std::byte{0x42}});
  // NOLINTNEXTLINE
  auto second_block = create_file_block(BLOCK_SIZE, {std::byte{0x24}});

  EXPECT_CALL(first_file, read(BLOCK_SIZE)).WillOnce(Return(first_block));
  EXPECT_CALL(second_file, read(BLOCK_SIZE)).WillOnce(Return(second_block));

  std::vector<std::uint8_t> first_file_hash{0x42, 0x24};  // NOLINT
  EXPECT_CALL(hash_mock, hash(first_block)).WillOnce(Return(first_file_hash));

  std::vector<std::uint8_t> second_file_hash{0x24, 0x42};  // NOLINT
  EXPECT_CALL(hash_mock, hash(second_block)).WillOnce(Return(second_file_hash));

  auto res = finder.find_duplicates({&first_file, &second_file});
  ASSERT_TRUE(res.empty());
}

// NOLINTNEXTLINE
TEST_F(BayanTest, For2DifferentFiles_WithDifferentSize_ReturnNothing) {
  auto big_file = MockBlockFile{};
  auto small_file = MockBlockFile{};

  EXPECT_CALL(big_file, finished).WillRepeatedly(Return(false));

  EXPECT_CALL(small_file, finished).WillRepeatedly(Return(true));

  // NOLINTNEXTLINE
  auto first_file_blocks =
      std::vector<Block>{create_file_block(BLOCK_SIZE, {std::byte{0x1}}),
                         create_file_block(BLOCK_SIZE, {std::byte{0x2}}),
                         create_file_block(BLOCK_SIZE, {std::byte{0x3}})};

  // NOLINTNEXTLINE
  auto second_block = create_file_block(BLOCK_SIZE, {std::byte{0x1}});

  // Will read first block, and than second block. After second block files will
  // be different
  EXPECT_CALL(big_file, read(BLOCK_SIZE))
      .Times(2)
      .WillRepeatedly(ReturnRoundRobin(first_file_blocks));

  EXPECT_CALL(small_file, read(BLOCK_SIZE)).WillOnce(Return(second_block));

  // On same first block return same hash twice
  hash::Hash first_block_hash{0x1};  // NOLINT
  EXPECT_CALL(hash_mock, hash(second_block))
      .Times(2)
      .WillRepeatedly(Return(first_block_hash));

  // On another blocks return different hashes
  std::vector<hash::Hash> another_hashes = {{0x2}, {0x3}, {0x4}};
  EXPECT_CALL(hash_mock, hash(Ne(second_block)))
      .WillRepeatedly(ReturnRoundRobin(another_hashes));

  auto res = finder.find_duplicates({&big_file, &small_file});
  ASSERT_TRUE(res.empty());
}
