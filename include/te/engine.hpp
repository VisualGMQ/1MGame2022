#include "pch.hpp"
#include "timer.hpp"
#include "renderer.hpp"
#include "event_dispatcher.hpp"
#include "scence.hpp"
#include "glhelp.hpp"
#include "gui/gui.hpp"

namespace te {

class Engine final {
public:
    static void Init(const char* title, int w, int h);
    static void Quit();
    static GLFWwindow* GetWindow() { return window_; }

    static void Run();
    static Size GetCanvaSize() { return initWindowSize_; }

private:
    static GLFWwindow* window_;
    static Size initWindowSize_;

    static void initGLFW();
    static void setEventDispatcher();
    static GLFWwindow* createWindow(const char* title, int w, int h);
    static void initGL();
};


// a fast way to run application
template <typename ScenceT, typename... Args>
void RunScence(const char* name, Args&&... args) {
    Engine::Init("1MGames 2th", 1024, 720);

    auto scence = te::ScenceMgr::CreateScence<ScenceT>(name, std::forward(args)...);
    ScenceMgr::Init(scence);

    Engine::Run();

    ScenceMgr::Quit();
    GAME_LOG_INFO("scence quit");
    Engine::Quit();
    GAME_LOG_INFO("engine quit");
}

}
