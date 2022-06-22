#pragma once

#include "transform.hpp"
#include "drawable.hpp"

class Entity final {
public:
    Entity();

    void Update();

    TransformComponent* GetTransform() { return transform_.get(); }
    DrawComponent* GetDraw() { return draw_.get(); }
    void SetDraw(std::unique_ptr<DrawComponent> draw);

private:
    std::unique_ptr<TransformComponent> transform_;
    std::unique_ptr<DrawComponent> draw_;
};
