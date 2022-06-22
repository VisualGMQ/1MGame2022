#include "game/entity.hpp"

Entity::Entity() {
    transform_.reset(new TransformComponent);
}

void Entity::Update() {
    if (draw_) {
        te::Transform transform;
        transform.SetPos(transform_->GetPos())
                 .SetSize(transform_->GetSize())
                 .SetFlip(transform_->GetFlip())
                 .SetRotation(transform_->GetRotation());
        draw_->Draw(transform, transform_->GetAnchor());
    }
}

void Entity::SetDraw(std::unique_ptr<DrawComponent> draw) {
    if (draw) {
        transform_->SetSize(draw->GetSize());
    }
    draw_ = std::move(draw);
}
