#pragma once

#include "Rendering/FrameInfo.hpp"
#include <entt/entity/fwd.hpp>

namespace Anantham::Ecs::Systems
{

class ISystem
{
public:
    ISystem() = default;
    virtual ~ISystem() = default;

    ISystem(const ISystem &) = delete;
    ISystem &operator=(const ISystem &) = delete;
    ISystem(ISystem &&) = delete;
    ISystem &operator=(ISystem &&) = delete;
};

class IRenderSystem : public ISystem
{
public:
    IRenderSystem() = default;
    virtual ~IRenderSystem() = default;

    virtual void render(entt::registry &registry, Rendering::FrameInfo &frameInfo) = 0;
};

class IUpdateSystem : public ISystem
{
public:
    IUpdateSystem() = default;
    virtual ~IUpdateSystem() = default;

    virtual void update(entt::registry &registry, float deltaTime) = 0;
};

} // namespace Anantham::Ecs::Systems
