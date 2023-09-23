#ifndef __APP_CONFIG_H_DAZODIQ7IEDH__
#define __APP_CONFIG_H_DAZODIQ7IEDH__

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "hash/hash_algorithm.h"

struct AppConfig {
  std::vector<boost::filesystem::path> paths_to_scan;

  std::vector<boost::filesystem::path> paths_to_ignore;

  std::size_t scan_level = 0;

  std::size_t minimal_size = 1;

  std::optional<std::string> filemask;

  static constexpr auto DEFAULT_BLOCK_SIZE = 512;
  std::size_t block_size = DEFAULT_BLOCK_SIZE;

  hash::HashType hash_function = hash::HashType::md5;

  bool parse(int argc, char *argv[]) noexcept;  // NOLINT
};

#endif  // __APP_CONFIG_H_DAZODIQ7IEDH__
