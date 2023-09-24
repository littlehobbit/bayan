#include <algorithm>
#include <iostream>
#include <iterator>

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include "app_config.h"
#include "bayan.h"
#include "block/block_file.h"
#include "block/block_file_impl.h"
#include "hash/hash_algorithm.h"
#include "utils/file_seeker.h"

template <typename Range, typename Transformer>
inline auto print_transformed(const Range &range,
                              Transformer &&transformer) noexcept {
  std::transform(
      std::cbegin(range), std::cend(range),
      std::ostream_iterator<boost::filesystem::path>(std::cout, "\n"),
      transformer);
}

int main(int argc, char *argv[]) try {
  AppConfig config;

  if (!config.parse(argc, argv)) {
    return 1;
  }

  // Find all files by config
  utils::FileSeeker file_seeker{config.paths_to_scan, config.paths_to_ignore};
  auto files = file_seeker.recursive_find(config.scan_level,
                                          config.minimal_size, config.filemask);

  std::vector<block_file::BlockFileImpl> block_files;
  std::transform(files.cbegin(), files.cend(), std::back_inserter(block_files),
                 [](const auto &path) {
                   auto entry = boost::filesystem::directory_entry{path};
                   return block_file::BlockFileImpl{std::move(entry)};
                 });

  auto hash_algorithm = hash::create(config.hash_function);

  bayan::BayanFinder::FileSequence file_sequence;
  std::transform(block_files.begin(), block_files.end(),
                 std::back_inserter(file_sequence),
                 [](auto &file) { return &file; });

  // Find all duplicates
  bayan::BayanFinder bayan{*hash_algorithm, config.block_size};
  auto duplicates = bayan.find_duplicates(file_sequence);

  for (const auto &duplicated : duplicates) {
    print_transformed(duplicated,
                      [](const auto &file) { return file->path(); });
    std::cout << std::endl;
  }
} catch (boost::filesystem::filesystem_error &err) {
  std::cerr << "Filesystem error: " << err.what() << std::endl;
} catch (std::exception &err) {
  std::cerr << "Error: " << err.what() << std::endl;
} catch (...) {
  std::cerr << "Unexpected error" << std::endl;
}