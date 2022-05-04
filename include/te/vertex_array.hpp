#pragma once

#include "glhelp.hpp"

namespace te {

class VertexArray final {
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void Unbind();
    void ActiveSlot(unsigned int slot, unsigned int num, GLenum type, unsigned int stride, unsigned int offset);

private:
    GLuint id_;
};

}
