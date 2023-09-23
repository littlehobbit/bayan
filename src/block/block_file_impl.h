#ifndef __BLOCK_FILE_IMPL_H_U96HY9OV9ORC__
#define __BLOCK_FILE_IMPL_H_U96HY9OV9ORC__

#include <algorithm>
#include <fstream>
#include <vector>

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>

#include "block_file.h"

namespace block_file {

class BlockFileImpl : public BlockFile {
 public:
  BlockFileImpl(boost::filesystem::directory_entry file)
      : _file{std::move(file)},
        _stream{_file.path()},
        _remaining{boost::filesystem::file_size(_file)} {}

  auto read(std::size_t block_size) noexcept -> Block override {
    Block block(block_size, std::byte{0x00});

    auto read = _stream.readsome(                            //
        reinterpret_cast<char*>(block.data()), block_size);  // NOLINT

    _remaining = std::clamp(_remaining - read, std::size_t{0}, _remaining);
    return block;
  }

  auto path() const noexcept -> const boost::filesystem::path& override {
    return _file.path();
  }

  bool finished() const noexcept override { return _remaining == 0; }

 private:
  boost::filesystem::directory_entry _file;
  std::ifstream _stream;
  std::size_t _remaining;
};

}  // namespace block_file

#endif  // __BLOCK_FILE_IMPL_H_U96HY9OV9ORC__
