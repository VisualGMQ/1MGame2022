#include "te/texture.hpp"

namespace te {

Texture::Texture(const unsigned char* data, int w, int h, int channel): size_(w, h) {
    GL_CALL(glGenTextures(1, &id_));

    Bind();

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    if (channel == 3) {
        GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
                             w, h,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE, data));
    } else {
        GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
                             w, h,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE, data));
    }
    if (w == h) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    Unbind();
}

void Texture::Bind(unsigned int slot) {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, id_));
}

void Texture::Unbind() {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
    GL_CALL(glDeleteTextures(1, &id_));
}


Storage<std::string_view, std::unique_ptr<Texture>> TextureMgr::storage_;

Texture* TextureMgr::Load(std::string_view filename, std::string_view name) {
    if (storage_.Find(name)) {
        ENGINE_LOG_WARN("%s texture already exists", name.data());
        return nullptr;
    } else {
        int w, h, channel;
        unsigned char* data = stbi_load(filename.data(), &w, &h, &channel, 4);
        if (!data) {
            ENGINE_LOG_ERROR("%s load failed", filename.data());
            return nullptr;
        }
        Texture* texture = new Texture(data, w, h, channel);
        storage_.Create(name, texture);
        stbi_image_free(data);
        return texture;
    }
}

Texture* TextureMgr::Find(std::string_view name) {
    auto texture = storage_.Find(name);
    if (texture) {
        return texture->get();
    } else {
        return nullptr;
    }
}

}
