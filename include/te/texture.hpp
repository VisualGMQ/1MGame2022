#pragma once

#include "pch.hpp"
#include "glhelp.hpp"
#include "storage.hpp"
#include "mathf.hpp"

namespace te {

class Texture final {
public:
    Texture(const unsigned char* data, int w, int h, int channel);
    Texture(const Texture&) = delete;
    ~Texture();

    Texture& operator=(const Texture&) = delete;

    int Width() const { return size_.w; }
    int Height() const { return size_.h; }
    const Size& Size() const { return size_; }

    void Bind(unsigned int slot = 0);
    void Unbind();

private:
    GLuint id_;
    ::te::Size size_;
};

class TextureMgr final {
public:
    static Texture* Load(std::string_view filename, std::string_view name);
    static Texture* Find(std::string_view name);

    static void Quit();

private:
    static Storage<std::string_view, std::unique_ptr<Texture>> storage_;
};

}
