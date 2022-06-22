#pragma once

#include "te/image.hpp"
#include "game/drawable.hpp"

class ImageComponent final: public DrawComponent {
public:
    ImageComponent(std::string_view name);
    ImageComponent(std::string_view name, te::Rect srcRect);

    void Draw(const te::Transform&, const te::Vec2& anchor, const te::Color&) override;

    te::Vec2 GetSize() const override { return image_.GetSize(); }

private:
    te::Image image_;
};
