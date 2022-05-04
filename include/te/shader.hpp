#pragma once

#include "pch.hpp"
#include "glhelp.hpp"
#include "mathf.hpp"

namespace te {

enum class ShaderType {
    Vertex,
    Fragment,
};

class ShaderModule final {
public:
    friend class Shader;

    static std::shared_ptr<ShaderModule> CreateVertexByCode(const std::string& code);
    static std::shared_ptr<ShaderModule> CreateFragmentByCode(const std::string& code);
    static std::shared_ptr<ShaderModule> CreateVertexByFile(const std::string& filename);
    static std::shared_ptr<ShaderModule> CreateFragmentByFile(const std::string& filename);

    ~ShaderModule();

private:
    GLuint shader_;
    ShaderType type_;

    ShaderModule(ShaderType type, const char* code);
};

class Shader final {
public:
    Shader(const std::shared_ptr<ShaderModule>& vertex,
                const std::shared_ptr<ShaderModule>& fragment);
    ~Shader();

    void Use() const;
    void Unuse() const;

    void UniformVec2(const char* name, const Vector2& v);
    void UniformVec4(const char* name, const Vector4& v);
    void UniformMat4(const char* name, const Mat4& m);
    void UniformInt(const char* name ,int value);

private:
    GLuint program_;
};

}
