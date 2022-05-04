#include "te/engine.hpp"

class WelcomeLayer: public te::Layer {
public:
    WelcomeLayer(const char* name): te::Layer(name) {}

    void OnInit() override {
    }

    void Render() override {
        auto texture = te::TextureMgr::Find("cube_man");
        te::Rect dst(100, 100, texture->Size().w, texture->Size().h);
        te::Renderer::DrawTexture(*texture, nullptr, &dst, 0, te::Color(1, 1, 1, 1), te::Renderer::Horizontal);

        te::Renderer::FillRect(te::Rect(10, 10, 50, 100), te::Color(0, 1, 0, 1));
        te::Renderer::DrawRect(te::Rect(10, 200, 50, 50), te::Color(0, 0, 1, 1));
        te::Renderer::DrawLine(te::Vec2(500, 500), te::Vec2(600, 700), te::Color(0, 1, 0, 1));
    }

    void OnQuit() override {
        GAME_LOG_DEBUG("layer quit");
    }
};

class WelcomeScence: public te::Scence {
public:
    WelcomeScence(const char* name): te::Scence(name) {}

    void OnInit() override {
        te::TextureMgr::Load("assets/image/cube_man.png", "cube_man");
        GAME_LOG_DEBUG("layer init");

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
