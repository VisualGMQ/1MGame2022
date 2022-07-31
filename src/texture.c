#include "hazel/texture.h"

struct Hazel_Texture {
    GLuint id;
    int w;
    int h;
    float r, g, b, a;
};

Hazel_Texture* Hazel_LoadTexture(const char* filename) {
    int w, h, channel;
    unsigned char* data = stbi_load(filename, &w, &h, &channel, 4);
    if (!data) {
        ENGINE_LOG_ERROR("load %s failed", filename);
        return NULL;
    }
    Hazel_Texture* texture = Hazel_CreateTexture(data, w, h);
    stbi_image_free(data);
    return texture;
}

void Hazel_TextureSetAlpha(Hazel_Texture* texture, float alpha) {
    if (!texture) return;
    texture->a = alpha;
}

void Hazel_TextureSetColor(Hazel_Texture* texture, float r, float g, float b) {
    if (!texture) return;
    texture->r = r;
    texture->g = g;
    texture->b = b;
}

Hazel_Texture* Hazel_CreateTexture(const unsigned char* data, int w, int h) {
    Hazel_Texture* result = (Hazel_Texture*)malloc(sizeof(Hazel_Texture));
    result->w = w;
    result->h = h;
    result->r = 1;
    result->g = 1;
    result->b = 1;
    result->a = 1;
    GL_CALL(glGenTextures(1, &result->id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, result->id));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));   
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    return result;
}

void Hazel_DestroyTexture(Hazel_Texture* texture) {
    if (texture) {
        GL_CALL(glDeleteTextures(1, &texture->id));
        free(texture);
    }
}

void Hazel_TextureGetSize(Hazel_Texture* texture, int* w, int* h) {
    if (texture) {
        if (w) *w = texture->w;
        if (h) *h = texture->h;
    } else {
        if (w) *w = 0;
        if (h) *h = 0;
    }
}

void Hazel_BindTexture(Hazel_Texture* texture) {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture ? texture->id : 0));
}

float Hazel_TextureGetAlpha(Hazel_Texture* texture) {
    return texture ? texture->a : -1;
}

void Hazel_TextureGetColor(Hazel_Texture* texture, float* r, float* g, float* b) {
    if (texture) {
        if (r) *r = texture->r;
        if (g) *g = texture->g;
        if (b) *b = texture->b;
    }
}
