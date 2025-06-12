#pragma once

#include "Base.hpp"
#include "Ecs/Systems/CameraSystem.hpp"
#include "Ecs/Systems/MeshRenderSystem.hpp"
#include "Rendering/RenderQueue.hpp"

namespace Anantham::Ecs::Scenes
{

class Scene3D : public Base
{
public:
    Scene3D() = delete;
    ~Scene3D() override = default;

    /**
     * @brief Updates the scene for the current frame.
     * @param deltaTime The time elapsed since the last frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Populates the render queue with commands for drawing the scene.
     * @param renderQueue The render queue to populate with draw commands.
     */
    void render(Rendering::RenderQueue &renderQueue) override;

private:
    Systems::CameraSystem _cameraSystem;
    Systems::MeshRenderSystem _meshRenderSystem;
};

} // namespace Anantham::Ecs::Scenes
