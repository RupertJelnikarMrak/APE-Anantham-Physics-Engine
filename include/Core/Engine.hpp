#pragma once

#include "Core/Scene/SceneManager.hpp"

namespace Core
{

class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

    void run();

private:
    void init();
    void shutdown();

    SceneManager _sceneManager;
};

} // namespace Core
