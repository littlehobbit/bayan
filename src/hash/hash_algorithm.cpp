#include "hash_algorithm.h"

#include <memory>

#include "hash/crc32.h"
#include "hash/md5.h"
#include "hash/sha256.h"

namespace hash {

auto create(HashType type) -> std::unique_ptr<HashAlgorithm> {
  switch (type) {
    case HashType::crc32:
      return std::make_unique<Crc32>();

    case HashType::md5:
      return std::make_unique<Md5>();

    case HashType::sha256:
      return std::make_unique<Sha256>();

    default:
      return {};
  }
}

}  // namespace hash