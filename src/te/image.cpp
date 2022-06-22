#include "te/image.hpp"

namespace te {

Image Image::Create(Texture& texture) {
    return Image(texture);
}

Image Image::Create(Texture& texture, const Rect& rect) {
    return Image(texture, rect);
}

Image Image::Create(std::string_view name) {
    auto texture = TextureMgr::Find(name);
    if (!texture) {
        ENGINE_LOG_ERROR("texture %s don't exists", name.data());
        return Image();
    } else {
        return Image(*TextureMgr::Find(name));
    }
}

Image Image::Create(std::string_view name, const Rect& rect) {
    auto texture = TextureMgr::Find(name);
    if (!texture) {
        ENGINE_LOG_ERROR("texture %s don't exists", name.data());
        return Image();
    } else {
        return Image(*TextureMgr::Find(name), rect);
    }
}

Image Image::Create(const Image& image, const Rect& rect) {
    return Image(*image.texture_,
                 Rect(rect.x + image.srcRect_.x, rect.y + image.srcRect_.y,
                      rect.w, rect.h));
}

Image::Image(): texture_(nullptr) {}

Image::Image(Texture& texture)
: texture_(&texture), srcRect_(0, 0, texture.Size().w, texture.Size().h) { }

Image::Image(Texture& texture, const Rect& rect)
: texture_(&texture), srcRect_(rect) {}

}
