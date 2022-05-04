#include "pch.hpp"
#include "engine.hpp"

namespace te {

class Key final {
public:
    static bool IsPress(int key);
    static bool IsRelease(int key);
};

}
