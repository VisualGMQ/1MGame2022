#include "te/engine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WelcomeLayer: public te::Layer {
public:
    WelcomeLayer(const char* name): te::Layer(name) {}

    void Render() override {
        bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
};

class WelcomeScence: public te::Scence {
public:
    WelcomeScence(const char* name): te::Scence(name) {}

    void OnInit() override {
        PushBackLayer<WelcomeLayer>("StartLayer");
    }
};

int main(int , char**) {
    te::RunScence<WelcomeScence>("StartScence");
    return 0;
}
