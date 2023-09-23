#ifndef __BAYAN_H_RO5GZL18P24E__
#define __BAYAN_H_RO5GZL18P24E__

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "block/block_file.h"
#include "hash/hash_algorithm.h"

namespace bayan {

class BayanFinder {
 public:
  BayanFinder(const hash::HashAlgorithm& algorithm, std::size_t block_size)
      : _hasher{algorithm}, _block_size{block_size} {};

  using FileSequence = std::vector<block_file::BlockFile*>;
  using DuplicatesSet = std::unordered_set<block_file::BlockFile*>;

  auto find_duplicates(const FileSequence& files) const noexcept
      -> std::vector<DuplicatesSet>;

 private:
  using HashSequence = std::vector<hash::Hash>;
  using HashDict = std::map<HashSequence, FileSequence>;

  auto init_hash_dict(const FileSequence& files) const noexcept -> HashDict;

  auto hash_new_block_from_file(block_file::BlockFile& file) const noexcept
      -> hash::Hash;

  auto append_hash(const HashSequence& seq, hash::Hash&& hash) const
      -> HashSequence;

  auto to_return_val(HashDict&& dict) const noexcept
      -> std::vector<DuplicatesSet>;

  auto need_read_more(const HashDict& dict) const noexcept -> bool;

  auto read_more(const HashDict& dict) const noexcept -> HashDict;

  const hash::HashAlgorithm& _hasher;
  std::size_t _block_size;
};

};  // namespace bayan

#endif  // __BAYAN_H_RO5GZL18P24E__
