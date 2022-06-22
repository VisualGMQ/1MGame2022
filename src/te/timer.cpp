#include "te/timer.hpp"

namespace te {

Timer Timer::SteadyTimer;

Timer::Timer(): oldTime_(0) {}

void Timer::Update() {
    double curTime = glfwGetTime();
    elapse_ = curTime - oldTime_;
    oldTime_ = curTime;
}

}
