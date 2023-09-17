#ifndef __SHA256_H_UZT876JPW1EP__
#define __SHA256_H_UZT876JPW1EP__

#include <vector>

#include <picosha2.h>

#include "hash_algorithm.h"

namespace hash {

class Sha256 final : public HashAlgorithm {
 public:
  auto hash(const Block &block) const noexcept -> Hash override {
    Hash hash{};

    constexpr auto SHA256_HASH_SIZE = 32;
    hash.resize(SHA256_HASH_SIZE);

    // NOLINTNEXTLINE
    const auto begin = reinterpret_cast<const std::uint8_t *>(block.data());
    const auto end = begin + block.size();  // NOLINT
    picosha2::hash256(begin, end, hash.begin(), hash.end());
    return hash;
  }
};

}  // namespace hash

#endif  // __SHA256_H_UZT876JPW1EP__
