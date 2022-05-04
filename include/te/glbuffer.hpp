#pragma once

#include "glhelp.hpp"

namespace te {

enum class BufferType {
    Vertex,
    Indices,
};

GLenum BufferType2GL(BufferType type);

class Buffer final {
public:
    Buffer(BufferType type);
    ~Buffer();

    void SetData(void* data, size_t size);

    void Bind();
    void Unbind();

private:
    GLuint id_;
    BufferType type_;
};

}
