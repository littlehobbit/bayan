#include "bayan.h"

#include <algorithm>
#include <iterator>
#include <numeric>

namespace bayan {

auto BayanFinder::find_duplicates(const FileSequence &files) const noexcept
    -> std::vector<DuplicatesSet> {
  if (files.size() <= 1) {
    return {};
  }

  auto files_per_hash = init_hash_dict(files);
  while (need_read_more(files_per_hash)) {
    files_per_hash = read_more(files_per_hash);
  }

  return to_return_val(std::move(files_per_hash));
}

auto BayanFinder::init_hash_dict(const FileSequence &files) const noexcept
    -> HashDict {
  HashDict files_per_hash;

  for (const auto &file : files) {
    auto hash = hash_new_block_from_file(*file);
    files_per_hash[HashSequence{hash}].push_back(file);
  }

  return files_per_hash;
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

auto BayanFinder::need_read_more(const HashDict &dict) const noexcept -> bool {
  return std::any_of(dict.cbegin(), dict.cend(), [](const auto &pair) {
    const auto &[hash, files] = pair;
    auto bigger_then_2 = files.size() >= 2;
    auto have_one_nonfinished =
        std::any_of(files.cbegin(), files.cend(),
                    [](auto &file) { return !file->finished(); });
    return bigger_then_2 && have_one_nonfinished;
  });
}

auto BayanFinder::read_more(const HashDict &dict) const noexcept -> HashDict {
  HashDict new_files_per_hash;
  for (const auto &[seq, files] : dict) {
    if (files.size() < 2) {
      continue;
    }

    for (const auto &file : files) {
      if (!file->finished()) {
        auto hash = hash_new_block_from_file(*file);

        new_files_per_hash[append_hash(seq, std::move(hash))].push_back(file);
      } else {
        new_files_per_hash[seq].push_back(file);
      }
    }
  }
  return new_files_per_hash;
}

auto BayanFinder::to_return_val(HashDict &&dict) const noexcept
    -> std::vector<DuplicatesSet> {
  std::vector<DuplicatesSet> ret;

  for (const auto &[hash, files] : dict) {
    if (files.size() >= 2) {
      ret.emplace_back(DuplicatesSet{files.cbegin(), files.cend()});
    }
  }

  return ret;
}

}  // namespace bayan