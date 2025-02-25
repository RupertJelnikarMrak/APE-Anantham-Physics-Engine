#pragma once

#include <entt/entt.hpp>

namespace Core
{

class Scene
{
public:
    Scene();
    ~Scene();

private:
    entt::registry m_Registry;
};

} // namespace Core
