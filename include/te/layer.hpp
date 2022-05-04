#pragma once
#include "pch.hpp"

namespace te {

class Layer {
public:
    Layer(const char* name);
    virtual ~Layer() = default;

    virtual void OnInit() {}
    virtual void Update() {}
    virtual void Render() {}
    virtual void OnQuit() {}

    virtual bool OnEventKeyDown(int key, int scancode, int mods) { return true; }
    virtual bool OnEventKeyUp(int key, int scancode, int mods) { return true; }
    virtual bool OnEventMouseMotion(float x, float y) { return true; }
    virtual bool OnEventMouseButtonDown(int button, int mods) { return true; }
    virtual bool OnEventMouseButtonUp(int button, int mods) { return true; }

    const char* GetName() const { return name_; }

    bool IsWorking() const { return isWorking_; }
    void StartWorking() { isWorking_ = true; }
    void StopWorking() { isWorking_ = false; }

private:
    bool isWorking_ = true;
    const char* name_;
};

}
