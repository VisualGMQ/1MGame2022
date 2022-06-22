#pragma once

#include "texture.hpp"

namespace te {

class Image final {
public:
    friend class Renderer;

    static Image Create(Texture& texture);
    static Image Create(Texture& texture, const Rect& rect);
    static Image Create(std::string_view name);
    static Image Create(std::string_view name, const Rect& rect);
    static Image Create(const Image& image, const Rect& rect);

    bool Valid() const { return texture_; }
    operator bool() const { return Valid(); }
    Image& operator=(const Image& image) {
        texture_ = image.texture_;
        srcRect_ = image.srcRect_;
        return *this;
    }
    Size GetSize() const { return Size(srcRect_.w, srcRect_.h); }

private:
    Texture* texture_;
    Rect srcRect_;

    Image();
    Image(Texture& texture);
    Image(Texture& texture, const Rect& rect);
};

}
