#ifndef __CRC32_H_B6XTA03IMCJH__
#define __CRC32_H_B6XTA03IMCJH__

#include <boost/crc.hpp>
#include <boost/integer.hpp>

#include "hash_algorithm.h"

namespace hash {

class Crc32 final : public HashAlgorithm {
 public:
  auto hash(const Block &block) const noexcept -> Hash override {
    boost::crc_32_type crc32;
    // NOLINTNEXTLINE
    crc32.process_bytes(block.data(), block.size());
    const auto checksum = static_cast<std::uint32_t>(crc32.checksum());
    return {static_cast<std::uint8_t>(checksum >> 24),           // NOLINT
            static_cast<std::uint8_t>((checksum >> 16) & 0xFF),  // NOLINT
            static_cast<std::uint8_t>((checksum >> 8) & 0xFF),   // NOLINT
            static_cast<std::uint8_t>(checksum & 0xFF)};         // NOLINT
  }
};

}  // namespace hash

#endif  // __CRC32_H_B6XTA03IMCJH__
