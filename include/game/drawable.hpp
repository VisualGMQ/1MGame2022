#pragma once

#include "te/renderer.hpp"

class DrawComponent {
public:
    virtual ~DrawComponent() = default;

    virtual void Draw(const te::Transform&, const te::Vec2& anchor = {0, 0}, const te::Color& = te::Color(1, 1, 1, 1)) = 0;
    virtual te::Vec2 GetSize() const = 0;
};
