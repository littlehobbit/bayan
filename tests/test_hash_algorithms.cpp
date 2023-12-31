#include <cstddef>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "block/block.h"
#include "hash/crc32.h"
#include "hash/hash_algorithm.h"
#include "hash/md5.h"
#include "hash/sha256.h"

#include <gmock/gmock-matchers.h>

using namespace ::testing;

// NOLINTNEXTLINE
TEST(HashAlgorithmsTest, Crc32) {
  // NOTE: according to https://crccalc.com/
  hash::Crc32 crc32;
  ASSERT_THAT(crc32.hash(Block{std::byte{0x1}}),
              ElementsAre(0xa5, 0x05, 0xdf, 0x1b));
  ASSERT_THAT(crc32.hash(Block{std::byte{0x1}, std::byte{0x2}}),
              ElementsAre(0xb6, 0xcc, 0x42, 0x92));
}

// NOLINTNEXTLINE
TEST(HashAlgorithmsTest, Md5) {
  // NOTE: according to https://emn178.github.io/online-tools/md5.html
  hash::Md5 md5;
  ASSERT_THAT(md5.hash(Block{std::byte{0x0}}),
              ElementsAre(0x93, 0xb8, 0x85, 0xad, 0xfe, 0x0d, 0xa0, 0x89, 0xcd,
                          0xf6, 0x34, 0x90, 0x4f, 0xd5, 0x9f, 0x71));
  ASSERT_THAT(md5.hash(Block{std::byte{0x11}, std::byte{0x22}}),
              ElementsAre(0xc7, 0x00, 0xed, 0x4f, 0xdb, 0x1d, 0x27, 0x05, 0x5a,
                          0xa3, 0xfa, 0xa2, 0xc2, 0x43, 0x22, 0x83));
}

// NOLINTNEXTLINE
TEST(HashAlgorithmsTest, Sha256) {
  // NOTE: according to https://emn178.github.io/online-tools/sha256.html
  hash::Sha256 sha256;
  ASSERT_THAT(sha256.hash(Block{std::byte{0x0}}),
              ElementsAre(0x6e, 0x34, 0x0b, 0x9c, 0xff, 0xb3, 0x7a, 0x98, 0x9c,
                          0xa5, 0x44, 0xe6, 0xbb, 0x78, 0x0a, 0x2c, 0x78, 0x90,
                          0x1d, 0x3f, 0xb3, 0x37, 0x38, 0x76, 0x85, 0x11, 0xa3,
                          0x06, 0x17, 0xaf, 0xa0, 0x1d));

  ASSERT_THAT(sha256.hash(Block{std::byte{0x11}, std::byte{0x22}}),
              ElementsAre(0x04, 0x4e, 0x2f, 0x81, 0x9a, 0x4a, 0x59, 0x92, 0xc4,
                          0x6c, 0xbc, 0xb5, 0xd1, 0x8f, 0x96, 0x23, 0x6d, 0xa9,
                          0x24, 0xe2, 0x72, 0x74, 0xec, 0xb6, 0xa4, 0x6f, 0x93,
                          0x90, 0x3e, 0x27, 0x2c, 0xa6));
}

// NOLINTNEXTLINE
TEST(HashAlgorithmsTest, OnCreate_ReturnSpecifiedAlgorithm) {
  ASSERT_THAT(hash::create(hash::HashType::crc32).get(),
              WhenDynamicCastTo<hash::Crc32*>(NotNull()));

  ASSERT_THAT(hash::create(hash::HashType::md5).get(),
              WhenDynamicCastTo<hash::Md5*>(NotNull()));

  ASSERT_THAT(hash::create(hash::HashType::sha256).get(),
              WhenDynamicCastTo<hash::Sha256*>(NotNull()));

  ASSERT_THAT(hash::create(static_cast<hash::HashType>(0xff)), IsNull());
}
