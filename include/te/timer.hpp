#pragma once

#include "pch.hpp"

namespace te {

class Timer final {
public:
    static Timer SteadyTimer;

    Timer();

    void Update();
    double GetMsElapse() const { return elapse_  * 1000; }
    double GetSecondsElampse() const { return elapse_; }

private:
    double oldTime_;
    double elapse_;
};

}
