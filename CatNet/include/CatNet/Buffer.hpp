#ifndef H_BUFFER
#define H_BUFFER

#include "BufferView.hpp"

#include <memory>

namespace CatNet {

struct Buffer : public CatNet::BufferView 
{
  Buffer(std::size_t initialCapacity);

private:
  std::unique_ptr<int8_t[]> _buffer;
  std::size_t _capacity;
};

} // namespace CatNet

#endif
