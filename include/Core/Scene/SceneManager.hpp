#pragma once

#include "Core/Scene/Scene.hpp"

// std
#include <memory>

namespace Core
{

class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;
    SceneManager(SceneManager &&) = delete;
    SceneManager &operator=(SceneManager &&) = delete;

private:
    std::unique_ptr<Scene> _currentScene;
};

} // namespace Core
