#ifndef __BLOCK_FILE_H_FS9DMB25VDHQ__
#define __BLOCK_FILE_H_FS9DMB25VDHQ__

#include <algorithm>
#include <vector>

#include "block.h"

namespace block_file {

class BlockFile {
 public:
  virtual ~BlockFile() = default;

  virtual auto read(std::size_t block_size) noexcept -> Block = 0;

  virtual bool finished() const noexcept = 0;
};

template <typename Stream>
class BlockStream : public BlockFile {
 public:
  BlockStream(Stream&& stream, std::size_t size)
      : _stream{std::move(stream)}, _remaining{size} {}

  auto read(std::size_t block_size) noexcept -> Block override {
    Block block(block_size, std::byte{0x00});

    auto read = _stream.readsome(                            //
        reinterpret_cast<char*>(block.data()), block_size);  // NOLINT

    _remaining = std::clamp(_remaining - read, std::size_t{0}, _remaining);
    return block;
  }

  bool finished() const noexcept override { return _remaining == 0; }

 private:
  Stream _stream;
  std::size_t _remaining;
};

}  // namespace block_file

#endif  // __BLOCK_FILE_H_FS9DMB25VDHQ__
