#ifndef __BLOCK_FILE_H_FS9DMB25VDHQ__
#define __BLOCK_FILE_H_FS9DMB25VDHQ__

#include <boost/filesystem/path.hpp>

#include "block.h"

namespace block_file {

class BlockFile {
 public:
  virtual ~BlockFile() = default;

  virtual auto read(std::size_t block_size) noexcept -> Block = 0;

  virtual auto path() const noexcept -> const boost::filesystem::path& = 0;

  virtual bool finished() const noexcept = 0;
};

}  // namespace block_file

#endif  // __BLOCK_FILE_H_FS9DMB25VDHQ__
