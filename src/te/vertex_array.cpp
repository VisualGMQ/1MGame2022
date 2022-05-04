#include "te/vertex_array.hpp"

namespace te {

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &id_));
}

VertexArray::~VertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &id_));
}

void VertexArray::Bind() {
    GL_CALL(glBindVertexArray(id_));
}

void VertexArray::ActiveSlot(unsigned int slot, unsigned int num, GLenum type, unsigned int stride, unsigned int offset) {
    GL_CALL(glVertexAttribPointer(slot, num, type, GL_FALSE, stride, (void*)offset));
    GL_CALL(glEnableVertexAttribArray(slot));
}

void VertexArray::Unbind() {
    GL_CALL(glBindVertexArray(0));
}

}
