#pragma once

// lib
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <entt/signal/fwd.hpp>

namespace Core::Scene
{

class Scene
{
private:
    entt::registry _registry;
    entt::dispatcher _dispatcher;

    entt::entity _cameraEntity;
};

} // namespace Core::Scene
