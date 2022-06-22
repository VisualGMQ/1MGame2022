#include "te/renderer.hpp"

namespace te {

std::unique_ptr<Shader> Renderer::shader_ = nullptr;
Mat4 Renderer::project_;
std::unique_ptr<Texture> Renderer::whiteTexture_ = nullptr;
std::unique_ptr<Buffer> Renderer::buffer_ = nullptr;
std::unique_ptr<Buffer> Renderer::indicesBuffer_ = nullptr;
std::unique_ptr<VertexArray> Renderer::vertexArray_ = nullptr;

void Renderer::Init(int w, int h) {
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    SetClearColor(Color(0.1, 0.1, 0.1));

    initShaders();

    project_ = CreateOrtho(0, w, h, 0, -1, 1);
    shader_->Use();
    shader_->UniformMat4("Project", project_);

    initWhiteTexture();
    initVertexBuffer();
    initIndicesBuffer();
    initVertexArray();

    shader_->UniformMat4("View", CreateEyeMat(1));
}

void Renderer::initShaders() {
    auto vertexShader = ShaderModule::CreateVertexByFile("shader/shader.vert");
    auto fragShader = ShaderModule::CreateFragmentByFile("shader/shader.frag");
    shader_.reset(new Shader(vertexShader, fragShader));
}

void Renderer::initWhiteTexture() {
    unsigned char data[] = { 0xFF, 0xFF, 0xFF};
    whiteTexture_.reset(new Texture(data, 1, 1, 3));
}

void Renderer::initVertexBuffer() {
    buffer_.reset(new Buffer(BufferType::Vertex));
    buffer_->Bind();
}

void Renderer::initIndicesBuffer() {
    indicesBuffer_.reset(new Buffer(BufferType::Indices));
    indicesBuffer_->Bind();
}

void Renderer::initVertexArray() {
    vertexArray_.reset(new VertexArray);
    vertexArray_->Bind();
    vertexArray_->ActiveSlot(0, 2, GL_FLOAT, sizeof(Vertex), 0);
    vertexArray_->ActiveSlot(1, 2, GL_FLOAT, sizeof(Vertex), sizeof(Vec2));
    vertexArray_->ActiveSlot(2, 4, GL_FLOAT, sizeof(Vertex), sizeof(Vec2) * 2);
}

void Renderer::Quit() {
    vertexArray_.reset();
    buffer_.reset();
    indicesBuffer_.reset();
    whiteTexture_.reset();
    shader_.reset();
}

void Renderer::SetClearColor(const Color& color) {
    GL_CALL(glClearColor(color.r, color.g, color.b, color.a));
}

void Renderer::Clear() {
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetViewport(int x, int y, int w, int h) {
    GL_CALL(glViewport(x, y, w, h));
}

void Renderer::SetViewMat(const Mat4& view) {
    shader_->UniformMat4("View", view);
}

void Renderer::DrawTexture(Texture& texture,
                           const Rect* src, const Rect* dst,
                           const Vec2& anchor,
                           float rotation,
                           const Color& color,
                           Flip flip) {
    std::array<Vertex, 4> vertices;
    vertices[0].position.Set(0, 1);
    vertices[1].position.Set(1, 0);
    vertices[2].position.Set(1, 1);
    vertices[3].position.Set(0, 0);

    for (auto& vertex : vertices) {
        vertex.position -= anchor;
    }

    if (flip & Vertical) {
        for (auto& vertex : vertices) {
            vertex.position.y *= -1;
        }
    }
    if (flip & Horizontal) {
        for (auto& vertex : vertices) {
            vertex.position.x *= -1;
        }
    }

    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;

    Rect fullRect(0, 0, texture.Size().w, texture.Size().h);
    if (!src) {
        src = &fullRect;
    }
    auto& size = texture.Size();
    vertices[0].texcoord.Set(src->x / size.w, (src->y + src->h) / size.h);
    vertices[1].texcoord.Set((src->x + src->w) / size.w, src->y / size.h);
    vertices[2].texcoord.Set((src->x + src->w) / size.w, (src->y + src->h) / size.h);
    vertices[3].texcoord.Set(src->x / size.w, src->y / size.h);

    auto modelMat = CreateSRT(Vec2(dst->x, dst->y), Vec2(dst->w, dst->h), rotation);
    DrawElements(vertices,
                 std::array<GLuint, 6>{0, 1, 2, 0, 3, 1},
                 texture,
                 GL_TRIANGLES,
                 modelMat);
}

void Renderer::DrawImage(const Image& image,
                         const Transform& transform,
                         const Vec2& anchor,
                         const Color& color) {
    Rect dstRect = transform.dstRect;
    if (dstRect.w == 0 || dstRect.h == 0) {
        dstRect.size = image.srcRect_.size;
    }
    Renderer::DrawTexture(*image.texture_,
                          &image.srcRect_,
                          &dstRect,
                          anchor,
                          transform.rotation,
                          color,
                          transform.flip);
}

void Renderer::FillRect(const Rect& rect, const Color& color) {
    std::array<Vertex, 6> vertices;
    vertices[0].position.Set(rect.x, rect.y);
    vertices[1].position.Set(rect.x + rect.w, rect.y + rect.h);
    vertices[2].position.Set(rect.x + rect.w, rect.y);
    vertices[3].position.Set(rect.x, rect.y + rect.h);
    for (auto& vertex : vertices) {
        vertex.color = color;
        vertex.texcoord.Set(0, 0);
    }
    std::array<GLuint, 6> indices{0, 1, 2, 0, 3, 1};
    DrawElements(vertices, indices, GL_TRIANGLES, CreateEyeMat(1.0f));
}

void Renderer::DrawRect(const Rect& rect, const Color& color) {
    std::array<Vertex, 4> vertices;
    vertices[0].position.Set(rect.x, rect.y);
    vertices[1].position.Set(rect.x + rect.w, rect.y);
    vertices[2].position.Set(rect.x + rect.w, rect.y + rect.h);
    vertices[3].position.Set(rect.x, rect.y + rect.h);

    for (auto& vertex : vertices) {
        vertex.texcoord.Set(0, 0);
        vertex.color = color;
    }
    DrawArrays(vertices, GL_LINE_LOOP, CreateEyeMat(1.0f));
}

void Renderer::DrawLine(const Vec2& p1, const Vec2& p2, const Color& color){
    std::array<Vertex, 2> vertices;
    vertices[0].position = p1;
    vertices[1].position = p2;
    for (auto& vertex : vertices) {
        vertex.texcoord.Set(0, 0);
        vertex.color = color;
    }
    DrawArrays(vertices, GL_LINES, CreateEyeMat(1.0f));
}

void Renderer::DrawArrays(const std::vector<Vertex>& vertices,
                          GLenum type,
                          const Mat4& transform) {
    buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
    whiteTexture_->Bind();
    shader_->UniformMat4("Model", transform);
    GL_CALL(glDrawArrays(type, 0, vertices.size()));
}

void Renderer::DrawArrays(const std::vector<Vertex>& vertices,
                          GLenum type,
                          Texture& texture,
                          const Mat4& transform) {
    buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
    texture.Bind();
    shader_->UniformMat4("Model", transform);
    GL_CALL(glDrawArrays(type, 0, vertices.size()));
}

void Renderer::DrawElements(const std::vector<Vertex>& vertices,
                            const std::vector<GLuint>& indices,
                            GLenum type,
                            const Mat4& transform) {
    buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
    indicesBuffer_->Bind();
    indicesBuffer_->SetData((void*)indices.data(), sizeof(GLuint) * indices.size());
    whiteTexture_->Bind();
    shader_->UniformMat4("Model", transform);
    GL_CALL(glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0));
}

void Renderer::DrawElements(const std::vector<Vertex>& vertices,
                            const std::vector<GLuint>& indices,
                            GLenum type,
                            Texture& texture,
                            const Mat4& transform) {
    buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
    indicesBuffer_->Bind();
    indicesBuffer_->SetData((void*)indices.data(), sizeof(GLuint) * indices.size());
    texture.Bind();
    shader_->UniformMat4("Model", transform);
    GL_CALL(glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0));
}

}
