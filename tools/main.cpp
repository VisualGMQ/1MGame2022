#include "te/engine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WelcomeLayer: public te::Layer {
public:
    WelcomeLayer(std::string_view name): te::Layer(name) {}

    void Render() override {
        bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
};

class WelcomeScence: public te::Scence {
public:
    WelcomeScence(std::string_view name): te::Scence(name) {}

    void OnInit() override {
        PushBackLayer<WelcomeLayer>("StartLayer");
    }
};

int main(int , char**) {
    te::RunScence<WelcomeScence>("StartScence");
    return 0;
}
