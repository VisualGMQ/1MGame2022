#pragma once

#include "te/renderer.hpp"

class TransformComponent final {
public:
    void Reset() {}

    te::Vec2 GetTopLeft() const { return te::Vec2(transform_.dstRect.x, transform_.dstRect.y) - anchor_ * GetSize(); }
    te::Vec2 GetPos() const { return te::Vec2(transform_.dstRect.x, transform_.dstRect.y); }
    te::Vec2 GetSize() const { return te::Vec2(transform_.dstRect.w, transform_.dstRect.h); }
    float GetRotation() const { return transform_.rotation; }
    te::Flip GetFlip() const { return transform_.flip; }
    TransformComponent* SetPos(const te::Vec2& pos) { transform_.SetPos(pos); return this; }
    TransformComponent* SetSize(const te::Vec2& size) { transform_.SetSize(size); return this; }
    TransformComponent* SetRotation(float rotation) { transform_.SetRotation(rotation); return this; }
    TransformComponent* SetFlip(te::Flip flip) { transform_.SetFlip(flip); return this; }
    te::Vec2 GetAnchor() const { return anchor_; }
    TransformComponent* SetAnchor(float x, float y) { anchor_.Set(std::clamp(x, 0.f, 1.f), std::clamp(y, 0.f, 1.f)); return this; }

private:
    te::Transform transform_;
    te::Vec2 anchor_;
};
