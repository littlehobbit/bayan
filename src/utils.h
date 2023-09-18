#ifndef __UTILS_H_62T1XCIZ9UD6__
#define __UTILS_H_62T1XCIZ9UD6__

#include <optional>
#include <regex>
#include <string>
#include <vector>

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace utils {

inline auto recursive_find(const boost::filesystem::path &directory,
                           std::size_t scan_level, std::size_t minimal_size,
                           std::optional<std::string> mask = std::nullopt)
    -> std::vector<boost::filesystem::path> {
  namespace fs = boost::filesystem;

  std::vector<fs::path> paths{};

  auto directory_iterator = fs::recursive_directory_iterator{directory};
  for (auto &&entry : directory_iterator) {
    if (directory_iterator.depth() >= scan_level) {
      directory_iterator.disable_recursion_pending();
    }

    if (fs::is_regular_file(entry) && fs::file_size(entry) >= minimal_size) {
      if (mask.has_value() &&
          !std::regex_search(entry.path().filename().string(),
                             std::regex{*mask})) {
        continue;
      }
      paths.push_back(std::move(entry));
    }
  }

  std::sort(paths.begin(), paths.end());
  return paths;
}

}  // namespace utils

#endif  // __UTILS_H_62T1XCIZ9UD6__
