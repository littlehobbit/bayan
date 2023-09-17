#include "bayan.h"

#include "block_file.h"

namespace bayan {

auto BayanFinder::find_duplicates(std::vector<block_file::BlockFile *> files)
    const noexcept -> std::vector<std::set<boost::filesystem::path>> {
  if (files.size() == 1) {
    return {};
  }

  std::map<HashSequence, std::vector<block_file::BlockFile *>> files_per_hash;

  for (auto &file : files) {
    auto hash = hash_new_block_from_file(*file);
    files_per_hash[HashSequence{hash}].push_back(file);
  }

  bool stop = false;
  while (!stop) {
    stop = true;

    std::map<HashSequence, std::vector<block_file::BlockFile *>>
        new_files_per_hash;
    for (auto &[seq, files] : files_per_hash) {
      if (files.size() < 2) {
        continue;
      }

      for (const auto &file : files) {
        if (!file->finished()) {
          stop = false;

          auto hash = hash_new_block_from_file(*file);

          new_files_per_hash[append_hash(seq, std::move(hash))].push_back(file);
        } else {
          new_files_per_hash[seq].push_back(file);
        }
      }
    }

    files_per_hash = std::move(new_files_per_hash);
  }

  std::vector<std::set<boost::filesystem::path>> ret;

  for (const auto &[hash, files] : files_per_hash) {
    std::set<boost::filesystem::path> duplicates;
    if (files.size() > 1) {
      for (auto *file : files) {
        duplicates.insert(file->path());
      }
      ret.push_back(std::move(duplicates));
    }
  }

  return ret;
}

auto BayanFinder::hash_new_block_from_file(
    block_file::BlockFile &file) const noexcept -> hash::Hash {
  return _hasher.hash(file.read(_block_size));
}

auto BayanFinder::append_hash(const HashSequence &seq, hash::Hash &&hash) const
    -> HashSequence {
  auto new_seq = seq;
  new_seq.emplace_back(std::move(hash));
  return new_seq;
}

}  // namespace bayan