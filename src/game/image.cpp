#include "game/image.hpp"

ImageComponent::ImageComponent(std::string_view name): image_(te::Image::Create(name)) {
}

void ImageComponent::Draw(const te::Transform& transform,
                          const te::Vec2& anchor,
                          const te::Color& color) {
    te::Renderer::DrawImage(image_, transform, anchor, color);
}
