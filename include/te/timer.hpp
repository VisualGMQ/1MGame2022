#pragma once

#include "pch.hpp"

namespace te {

class Timer final {
public:
    Timer();

    void Update();
    double Elapse() const { return elapse_; }

private:
    double oldTime_;
    double elapse_;
};

}
