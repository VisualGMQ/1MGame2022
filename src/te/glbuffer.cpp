#include "te/glbuffer.hpp"

namespace te {

GLenum BufferType2GL(BufferType type) {
    switch (type) {
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;
        case BufferType::Indices:
            return GL_ELEMENT_ARRAY_BUFFER;
    }
}

Buffer::Buffer(BufferType type): type_(type) {
    GL_CALL(glGenBuffers(1, &id_));
}

void Buffer::SetData(void* data, size_t size) {
    GL_CALL(glBufferData(BufferType2GL(type_), size, data, GL_STATIC_DRAW));
}

void Buffer::Bind() {
    GL_CALL(glBindBuffer(BufferType2GL(type_), id_));
}

void Buffer::Unbind() {
    GL_CALL(glBindBuffer(BufferType2GL(type_), 0));
}

Buffer::~Buffer() {
    GL_CALL(glDeleteBuffers(1, &id_));
}

}
