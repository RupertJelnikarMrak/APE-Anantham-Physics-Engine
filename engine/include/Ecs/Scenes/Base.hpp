#pragma once

#include "Rendering/RenderQueue.hpp"

#include <entt/entt.hpp>
#include <vulkan/vulkan_core.h>

namespace Anantham::Ecs::Scenes
{

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    /**
     * @brief Updates the scene for the current frame.
     * @param deltaTime The time elapsed since the last frame.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Populates the render queue with commands for drawing the scene.
     * @param commandBuffer The Vulkan command buffer to record the draw commands.
     * @param pipelineLayout The Vulkan pipeline layout to use for rendering.
     */
    virtual void render(Rendering::RenderQueue &renderQueue) = 0;

    entt::registry &getRegistry() { return _registry; }

protected:
    entt::registry _registry;
};

} // namespace Anantham::Ecs::Scenes
