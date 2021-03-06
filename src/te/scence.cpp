#include "te/scence.hpp"

namespace te {

Scence::Scence(std::string_view name): name_(name) {}

void Scence::OnUpdate() {
    for (auto& layer : layers_) {
        if (layer->IsWorking()) {
            layer->Update();
        }
    }
}

void Scence::OnRender() {
    for (auto& layer : layers_) {
        if (layer->IsWorking()) {
            layer->Render();
        }
    }
}



Storage<std::string_view, std::unique_ptr<Scence>> ScenceMgr::storage_;
Scence* ScenceMgr::curScence_ = nullptr;
Scence* ScenceMgr::oldScence_ = nullptr;

void ScenceMgr::Init(Scence* scence) {
    curScence_ = scence;
    if (curScence_) {
        curScence_->OnInit();
        for (auto& layer : scence->layers_) {
            layer->OnInit();
        }
    } else {
        ENGINE_LOG_ERROR("first runnig scence can't be nullptr");
    }
}

void ScenceMgr::Quit() {
    if (curScence_) {
        for (auto& layer : curScence_->layers_) {
            layer->OnQuit();
        }
        curScence_->OnQuit();
    }
    storage_.Clear();
}

void ScenceMgr::SwitchScence(std::string_view name) {
    oldScence_ = curScence_;
    Scence* scence = GetScence(name);
    if (scence) {
        scence->OnInit();
    } else {
        ENGINE_LOG_ERROR("Scence `%s` not exists", name.data());
    }
    curScence_ = scence;
}

Scence* ScenceMgr::GetCurrentScence() {
    return curScence_;
}

void ScenceMgr::CleanUpOldScence() {
    if (oldScence_) {
        oldScence_->OnQuit();
        oldScence_ = nullptr;
    }
}

Scence* ScenceMgr::GetScence(std::string_view name) {
    auto scence = storage_.Find(name);
    if (!scence) {
        return nullptr;
    } else {
        return scence->get();
    }
}

}
