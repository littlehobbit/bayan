#include <cstddef>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "block.h"
#include "hash/crc32.h"
#include "hash/md5.h"

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