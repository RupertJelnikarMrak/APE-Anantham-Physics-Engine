#pragma once

#include "Ecs/Systems/ISystem.hpp"
#include "Rendering/FrameInfo.hpp"

#include <entt/entt.hpp>
#include <vulkan/vulkan_core.h>

namespace Anantham::Ecs::Scenes
{

class IScene
{
public:
    IScene() = default;
    virtual ~IScene() = default;

    void update(float deltaTime);

    void render(Rendering::FrameInfo &frameInfo);

    entt::registry &getRegistry() { return _registry; }

protected:
    entt::registry _registry;

    std::vector<std::unique_ptr<Ecs::Systems::IUpdateSystem>> _updateSystems;
    std::vector<std::unique_ptr<Ecs::Systems::IRenderSystem>> _renderSystems;
};

} // namespace Anantham::Ecs::Scenes
