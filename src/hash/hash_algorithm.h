#ifndef __HASH_ALGORITHM_H_EY22VN0ZRBNP__
#define __HASH_ALGORITHM_H_EY22VN0ZRBNP__

#include <cstdint>
#include <vector>

#include "block.h"

namespace hash {

enum class HashFunction { md5, crc32, sha256 };

class HashAlgorithm {
 public:
  virtual ~HashAlgorithm() = default;

  virtual auto hash(const Block &block) noexcept
      -> std::vector<std::uint8_t> = 0;
};

}  // namespace hash

#endif  // __HASH_ALGORITHM_H_EY22VN0ZRBNP__
