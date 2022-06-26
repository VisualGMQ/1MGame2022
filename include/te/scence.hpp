#pragma once

#include "pch.hpp"
#include "log.hpp"
#include "layer.hpp"
#include "event_dispatcher.hpp"
#include "storage.hpp"

namespace te {

class Scence {
public:
    friend class ScenceMgr;
    friend class Engine;

    using LayerContainer = std::list<std::unique_ptr<Layer>>;

    /* use `string_view` means it won't change the string and hope you pass string-literal*/
    Scence(std::string_view name);

    virtual ~Scence() = default;

    virtual void OnInit() {}
    virtual bool OnQuit() { return true; }

    void OnUpdate();
    void OnRender();

    std::string_view Name() const { return name_; }

    template <typename LayerT, typename... Args>
    void PushBackLayer(const char* name, Args... args) {
        layers_.emplace_back(new LayerT(name, std::forward<Args>(args)...));
    }

    template <typename LayerT, typename... Args>
    void PushFrontLayer(const char* name, Args... args) {
        layers_.emplace_front(new LayerT(name, std::forward<Args>(args)...));
    }

    Layer* FindLayer(const char* name) const {
        auto it = std::find_if(layers_.begin(), layers_.end(),
                               [=](const std::unique_ptr<Layer>& layer) {
                                   return layer->GetName() == name;
                               });
        if (it != layers_.end()) {
            return nullptr;
        } else {
            return it->get();
        }
    }

    LayerContainer& GetLayers() { return layers_; }
    size_t LayerCount() const { return layers_.size(); }

private:
    std::string_view name_;
    LayerContainer layers_;
};


class ScenceMgr final {
public:
    static void Init(Scence*);
    static void Quit();

    template <typename T, typename... Args>
    static Scence* CreateScence(std::string_view name, Args&&... args) {
        if (storage_.IsExists(name)) {
            ENGINE_LOG_WARN("Scence %s exists", name.data());
            return nullptr;
        } else {
            Scence* scence = new T(name, std::forward<Args>(args)...);
            storage_.Create(name, scence);
            return scence;
        }
    }

    static void SwitchScence(std::string_view name);
    static Scence* GetScence(std::string_view name);
    static Scence* GetCurrentScence();
    static void CleanUpOldScence();

private:
    static Storage<std::string_view, std::unique_ptr<Scence>> storage_;
    static Scence* curScence_;
    static Scence* oldScence_;
};

}
