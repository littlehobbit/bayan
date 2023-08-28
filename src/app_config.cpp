#include "app_config.h"

#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Error.hpp>
#include <CLI/Validators.hpp>

bool AppConfig::parse(int argc, char *argv[]) noexcept {  // NOLINT
  CLI::App app{"duplicate files finder", "bayan"};

  app.add_option("-s,--scan", paths_to_scan, "List of paths for scanning")
      ->check(CLI::ExistingPath)
      ->expected(1, -1)
      ->required();

  app.add_option("-i,--ignore", paths_to_ignore,
                 "List of paths to ignore scanning")
      ->check(CLI::ExistingPath);

  app.add_option("-l,--level", scan_level, "Level of scannig")
      ->default_val(scan_level);

  app.add_option("-m,--size", minimal_size, "Minimal size of file (in bytes)")
      ->default_val(minimal_size);

  app.add_option("-f,--filemask", filemask, "Mask of files to compare");

  app.add_option("-b,--block-size", block_size, "Read block size")
      ->default_val(block_size);

  using hash::HashFunction;
  std::map<std::string, HashFunction> hash_mapping{
      {"md5", HashFunction::md5},
      {"crc32", HashFunction::crc32},
      {"sha256", HashFunction::sha256}};
  app.add_option("-a,--algorithm", hash_function,
                 "Hash algorithm for file comparison")
      ->transform(CLI::CheckedTransformer{hash_mapping, CLI::ignore_case})
      ->default_val(hash_function);

  try {
    app.parse(argc, argv);
  } catch (CLI::Error &err) {
    app.exit(err);
    return false;
  }

  return true;
}