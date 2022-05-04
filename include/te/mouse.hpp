#pragma once

#include "engine.hpp"

namespace te {

class Mouse final {
public:
    static bool IsLeftPress();
    static bool IsLeftRelease();
    static bool IsRightPress();
    static bool IsRightRelease();

    static Vec2 Position();
    static Vec2 RealPosition();
};

}
