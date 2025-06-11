#include "Buffer.hpp"

namespace CatNet
{
    Buffer::Buffer(std::size_t initialCapacity)
        : BufferView(nullptr, initialCapacity),
        _buffer(std::make_unique<int8_t[]>(initialCapacity)),
        _capacity(initialCapacity)

    {
        BufferView::_buffer = _buffer.get();
    }
}
