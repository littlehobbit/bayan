#ifndef __MD5_H_T9AAY374XGNS__
#define __MD5_H_T9AAY374XGNS__

#include <vector>

#include <boost/uuid/detail/md5.hpp>

#include "hash_algorithm.h"

namespace hash {

class Md5 final : public HashAlgorithm {
 public:
  auto hash(const Block &block) noexcept -> std::vector<std::uint8_t> override {
    boost::uuids::detail::md5 md5;
    md5.process_bytes(block.data(), block.size());

    boost::uuids::detail::md5::digest_type digets;
    md5.get_digest(digets);

    constexpr auto MD5_HASH_SIZE = 16;
    std::vector<std::uint8_t> hash{};
    hash.reserve(MD5_HASH_SIZE);

    for (auto diget : digets) {
      hash.push_back(static_cast<std::uint8_t>(diget >> 24));  // NOLINT
      hash.push_back(
          static_cast<std::uint8_t>((diget >> 16) & 0xFF));            // NOLINT
      hash.push_back(static_cast<std::uint8_t>((diget >> 8) & 0xFF));  // NOLINT
      hash.push_back(static_cast<std::uint8_t>(diget & 0xFF));         // NOLINT
    }

    return hash;
  }
};

}  // namespace hash

#endif  // __MD5_H_T9AAY374XGNS__
