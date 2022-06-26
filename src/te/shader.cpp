#include "te/shader.hpp"

namespace te {

ShaderModule::ShaderModule(ShaderType type, const char* code): type_(type) {
    switch (type) {
        case ShaderType::Vertex:
            shader_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::Fragment:
            shader_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
    }

    const char* typeName = type_ == ShaderType::Vertex ? "vertex" : "fragment";

    if (shader_ == 0) {
        ENGINE_LOG_CRITICAL("%s create failed", typeName);
    }

    GL_CALL(glShaderSource(shader_, 1, &code, NULL));
    GL_CALL(glCompileShader(shader_));

    int  success;
    char infoLog[512];
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader_, 512, NULL, infoLog);
        ENGINE_LOG_ERROR("%s compile failed:\n%s", typeName, infoLog);
    }
}

ShaderModule::~ShaderModule() {
    GL_CALL(glDeleteShader(shader_));
}

std::unique_ptr<ShaderModule> ShaderModule::CreateVertexByCode(const std::string& code) {
    return std::unique_ptr<ShaderModule>(new ShaderModule(ShaderType::Vertex, code.data()));
}

std::unique_ptr<ShaderModule> ShaderModule::CreateFragmentByCode(const std::string& code) {
    return std::unique_ptr<ShaderModule>(new ShaderModule(ShaderType::Fragment, code.data()));
}

std::unique_ptr<ShaderModule> ShaderModule::CreateVertexByFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string code((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();
    return ShaderModule::CreateVertexByCode(code);
}

std::unique_ptr<ShaderModule> ShaderModule::CreateFragmentByFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string code((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();
    return ShaderModule::CreateFragmentByCode(code);
}

Shader::Shader(const ShaderModule& vertex,
			   const ShaderModule& fragment) {
    program_ = glCreateProgram();
    if (program_ == 0) {
        ENGINE_LOG_WARN("shader program create failed");
    }

    GL_CALL(glAttachShader(program_, vertex.shader_));
    GL_CALL(glAttachShader(program_, fragment.shader_));
    GL_CALL(glLinkProgram(program_));

    char infoLog[512];
    GLint success;
    GL_CALL(glGetProgramiv(program_, GL_LINK_STATUS, &success));
    if(!success) {
        GL_CALL(glGetProgramInfoLog(program_, 512, NULL, infoLog));
        ENGINE_LOG_ERROR("program link failed:\n%s", infoLog);
    }
}

void Shader::Use() const {
    GL_CALL(glUseProgram(program_));
}

void Shader::Unuse() const {
    GL_CALL(glUseProgram(0));
}

void Shader::UniformInt(const char* name ,int value) {
    GLint loc = glGetUniformLocation(program_, name);
    if (loc == - 1) {
        ENGINE_LOG_ERROR("%s uniform vec2 buffer failed", name);
    } else {
        GL_CALL(glUniform1i(loc, value));
    }
}

void Shader::UniformVec2(const char* name, const Vector2& v) {
    GLint loc = glGetUniformLocation(program_, name);
    if (loc == - 1) {
        ENGINE_LOG_ERROR("%s uniform vec2 buffer failed", name);
    } else {
        GL_CALL(glUniform2f(loc, v.x, v.y));
    }
}

void Shader::UniformVec4(const char* name, const Vector4& v) {
    GLint loc = glGetUniformLocation(program_, name);
    if (loc == - 1) {
        ENGINE_LOG_ERROR("%s uniform vec4 buffer failed", name);
    } else {
        GL_CALL(glUniform4f(loc, v.x, v.y, v.z, v.w));
    }
}

void Shader::UniformMat4(const char* name, const Mat4& m) {
    GLint loc = glGetUniformLocation(program_, name);
    if (loc == - 1) {
        ENGINE_LOG_ERROR("%s uniform mat4 buffer failed", name);
    } else {
        GL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data()));
    }
}

Shader::~Shader() {
    GL_CALL(glDeleteProgram(program_));
}


 }
