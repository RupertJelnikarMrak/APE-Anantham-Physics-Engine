#pragma once

namespace Core
{

class Engine
{
public:
    Engine() = default;

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

    void update();
    void render();
};

} // namespace Core
