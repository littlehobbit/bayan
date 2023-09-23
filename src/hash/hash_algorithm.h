#ifndef __HASH_ALGORITHM_H_EY22VN0ZRBNP__
#define __HASH_ALGORITHM_H_EY22VN0ZRBNP__

#include <cstdint>
#include <memory>
#include <vector>

#include "block/block.h"

namespace hash {

enum class HashType { md5, crc32, sha256 };

using Hash = std::vector<std::uint8_t>;

class HashAlgorithm {
 public:
  virtual ~HashAlgorithm() = default;

  virtual auto hash(const Block &block) const noexcept -> Hash = 0;
};

auto create(HashType type) -> std::unique_ptr<HashAlgorithm>;

}  // namespace hash

#endif  // __HASH_ALGORITHM_H_EY22VN0ZRBNP__
