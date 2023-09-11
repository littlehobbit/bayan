#include <cstddef>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "block.h"
#include "hash/crc32.h"

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