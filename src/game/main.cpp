#include "te/engine.hpp"
#include "te/animation.hpp"
#include "game/entity.hpp"
#include "game/image.hpp"

class WelcomeLayer: public te::Layer {
public:
    WelcomeLayer(std::string_view name): te::Layer(name) {}

    void OnInit() override {
        tilesheet_.reset(new te::TileSheet(te::Image::Create("kirby"), 16, 2));

        std::vector<uint32_t> delays(16, 50);
        anim_ = te::Animation::Create(*tilesheet_, delays, 0);
        anim_.SetLoop(-1);
        anim_.Play();

        entity_.SetDraw(std::unique_ptr<DrawComponent>(new ImageComponent("kirby")));
        auto transform = entity_.GetTransform();
        transform->SetAnchor(0.5, 0.5)
                 ->SetPos(te::Vec2(200, 300))
                 ->SetFlip(te::Flip::Both);
    }

    void Update() override {
        anim_.Update();
        entity_.Update();
    }

    void Render() override {
        te::Image image = te::Image::Create("cube_man");

        te::Transform transform;
        transform.SetRotation(30).SetPos(te::Vec2(100, 100));
        te::Renderer::DrawImage(image, transform, {0, 0});

        te::Renderer::FillRect(te::Rect(10, 10, 50, 100), te::Color(0, 1, 0, 1));
        te::Renderer::DrawRect(te::Rect(10, 200, 50, 50), te::Color(0, 0, 1, 1));
        te::Renderer::DrawLine(te::Vec2(500, 500), te::Vec2(600, 700), te::Color(0, 1, 0, 1));

        te::Transform animTrans;
        animTrans.SetPos(te::Vec2(500, 500));
        te::Renderer::DrawImage(anim_.GetCurFrame().image, animTrans, {0, 0});
    }

    void OnQuit() override {
        GAME_LOG_DEBUG("layer quit");
    }

private:
    std::unique_ptr<te::TileSheet> tilesheet_;
    te::Animation anim_;
    Entity entity_;
};

class WelcomeScence: public te::Scence {
public:
    WelcomeScence(std::string_view name): te::Scence(name) {}

    void OnInit() override {
        te::TextureMgr::Load("assets/image/cube_man.png", "cube_man");
        te::TextureMgr::Load("assets/image/kirby.png", "kirby");

        GAME_LOG_DEBUG("welcome scence init");
        PushBackLayer<WelcomeLayer>("StartLayer");
    }

    bool OnQuit() override {
        GAME_LOG_DEBUG("welcome scence quit");
        return true;
    }
};

int main(int , char**) {
    te::RunScence<WelcomeScence>("StartScence");
    return 0;
}
