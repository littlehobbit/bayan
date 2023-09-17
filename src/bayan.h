#ifndef __BAYAN_H_RO5GZL18P24E__
#define __BAYAN_H_RO5GZL18P24E__

#include <functional>
#include <memory>
#include <set>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "block_file.h"
#include "hash/hash_algorithm.h"

namespace bayan {

class BayanFinder {
 public:
  BayanFinder(const hash::HashAlgorithm& algorithm, std::size_t block_size)
      : _hasher{algorithm}, _block_size{block_size} {};

  auto find_duplicates(std::vector<block_file::BlockFile*> files) const noexcept
      -> std::vector<std::set<boost::filesystem::path>>;

 private:
  using HashSequence = std::vector<hash::Hash>;

  auto hash_new_block_from_file(block_file::BlockFile& file) const noexcept -> hash::Hash;

  auto append_hash(const HashSequence &seq, hash::Hash &&hash) const -> HashSequence;

  const hash::HashAlgorithm& _hasher;
  std::size_t _block_size;
};

};  // namespace bayan

#endif  // __BAYAN_H_RO5GZL18P24E__
