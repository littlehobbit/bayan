#ifndef __FILE_SEEKER_H_8NUO95F8EK6N__
#define __FILE_SEEKER_H_8NUO95F8EK6N__

#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace utils {

class FileSeeker {
 public:
  FileSeeker(std::vector<boost::filesystem::path> scan_paths,
             std::vector<boost::filesystem::path> ignore_paths)
      : _scan_paths{std::move(scan_paths)},
        _ignore_paths{std::move(ignore_paths)} {}

  auto recursive_find(std::size_t scan_level, std::size_t minimal_size,
                      const std::optional<std::string> &mask = std::nullopt)
      -> std::vector<boost::filesystem::path> {
    namespace fs = boost::filesystem;

    std::vector<fs::path> paths{};

    for (const auto &directory : _scan_paths) {
      auto directory_iterator = fs::recursive_directory_iterator{directory};
      for (auto &&entry : directory_iterator) {
        if (static_cast<std::size_t>(directory_iterator.depth()) ==
                (scan_level + 1) ||
            is_ignored(entry)) {
          directory_iterator.disable_recursion_pending();
          continue;
        }

        if (fs::is_regular_file(entry) &&
            fs::file_size(entry) >= minimal_size) {
          if (mask.has_value() &&
              !std::regex_search(entry.path().filename().string(),
                                 std::regex{*mask})) {
            continue;
          }

          paths.push_back(std::move(entry));
        }
      }
    }

    return paths;
  }

 private:
  bool is_ignored(const boost::filesystem::path &path) const noexcept {
    return std::any_of(
        _ignore_paths.cbegin(), _ignore_paths.cend(),
        [&](auto &ignored) { return is_subpath(ignored, path); });
  }

  bool is_subpath(const boost::filesystem::path &base,
                  const boost::filesystem::path &entry) const noexcept {
    const auto mismatch_pair =
        std::mismatch(entry.begin(), entry.end(), base.begin(), base.end());
    return mismatch_pair.second == base.end();
  }

  std::vector<boost::filesystem::path> _scan_paths;
  std::vector<boost::filesystem::path> _ignore_paths;
};

}  // namespace utils

#endif  // __FILE_SEEKER_H_8NUO95F8EK6N__
