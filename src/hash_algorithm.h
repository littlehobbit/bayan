#ifndef __HASH_ALGORITHM_H_EY22VN0ZRBNP__
#define __HASH_ALGORITHM_H_EY22VN0ZRBNP__

#include <vector>

namespace hash {

enum class HashFunction { md5, crc32, sha256 };

using Block = std::vector<std::byte>;

class HashAlgorithm {
 public:
  virtual ~HashAlgorithm() = default;

  virtual auto hash(const Block &block) noexcept -> std::vector<std::byte> = 0;
};

}  // namespace hash

#endif  // __HASH_ALGORITHM_H_EY22VN0ZRBNP__
