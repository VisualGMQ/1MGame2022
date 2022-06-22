#pragma once

#include "pch.hpp"
#include "mathf.hpp"
#include "glhelp.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "glbuffer.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "image.hpp"

namespace te {

enum Flip {
    None = 0x00,
    Vertical = 0x01,
    Horizontal = 0x02,
    Both = Vertical | Horizontal,
};

struct Transform {
    Flip flip = Flip::None;
    Rect dstRect = {0, 0, 0, 0};
    float rotation = 0;
    
    Transform& SetFlip(Flip flip) {
        this->flip = flip;
        return *this;
    }

    Transform& SetRotation(float degree) {
        this->rotation = degree;
        return *this;
    }

    Transform& SetSize(const Size& size) {
        dstRect.size = size;
        return *this;
    }

    Transform& SetPos(const Vec2& pos) {
        dstRect.pos = pos;
        return *this;
    }
};

class Renderer final {
public:
    static void Init(int w, int h);
    static void Quit();

    static void SetClearColor(const Color&);
    static void Clear();

    static void SetViewport(int x, int y, int w, int h);

    static void SetViewMat(const Mat4& view);

    static void DrawImage(const Image&, const Transform&, const Vec2& anchor, const Color& = Color(1, 1, 1, 1));
    static void DrawTexture(Texture& texture,
                            const Rect* src, const Rect* dst,
                            const Vec2& anchor,
                            float rotation,
                            const Color& color = Color(1, 1, 1),
                            Flip flip = None);
    static void DrawRect(const Rect& rect, const Color& color);
    static void FillRect(const Rect& rect, const Color& color);
    static void DrawLine(const Vec2& p1, const Vec2& p2,
                         const Color& color);
    static void DrawArrays(const std::vector<Vertex>& vertices,
                           GLenum type,
                           const Mat4& transform);
    static void DrawArrays(const std::vector<Vertex>& vertices,
                           GLenum type,
                           Texture& texture,
                           const Mat4& transform);
    static void DrawElements(const std::vector<Vertex>& vertices,
                             const std::vector<GLuint>& indices,
                             GLenum type,
                             const Mat4& transform);
    static void DrawElements(const std::vector<Vertex>& vertices,
                             const std::vector<GLuint>& indices,
                             GLenum type,
                             Texture& texture,
                             const Mat4& transform);

    template <size_t N>
    static void DrawArrays(const std::array<Vertex, N>& vertices,
                           GLenum type,
                           const Mat4& transform) {
        buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
        whiteTexture_->Bind();
        shader_->UniformMat4("Model", transform);
        GL_CALL(glDrawArrays(type, 0, vertices.size()));
    }

    template <size_t N1, size_t N2>
    static void DrawElements(const std::array<Vertex, N1>& vertices,
                             const std::array<GLuint, N2>& indices,
                             GLenum type,
                             const Mat4& transform) {
        buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
        indicesBuffer_->Bind();
        indicesBuffer_->SetData((void*)indices.data(), sizeof(GLuint) * indices.size());
        whiteTexture_->Bind();
        shader_->UniformMat4("Model", transform);
        GL_CALL(glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0));
    }

    template <size_t N1, size_t N2>
    static void DrawElements(const std::array<Vertex, N1>& vertices,
                             const std::array<GLuint, N2>& indices,
                             Texture& texture,
                             GLenum type,
                             const Mat4& transform) {
        buffer_->SetData((void*)vertices.data(), sizeof(Vertex) * vertices.size());
        indicesBuffer_->Bind();
        indicesBuffer_->SetData((void*)indices.data(), sizeof(GLuint) * indices.size());
        texture.Bind();
        shader_->UniformMat4("Model", transform);
        GL_CALL(glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0));
    }

private:
    static std::unique_ptr<Shader> shader_;
    static Mat4 project_;
    static std::unique_ptr<Texture> whiteTexture_;
    static std::unique_ptr<Buffer> buffer_;
    static std::unique_ptr<Buffer> indicesBuffer_;
    static std::unique_ptr<VertexArray> vertexArray_;

    static void initShaders();
    static void initWhiteTexture();
    static void initVertexBuffer();
    static void initIndicesBuffer();
    static void initVertexArray();
};

}
