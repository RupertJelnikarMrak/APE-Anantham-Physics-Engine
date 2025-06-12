#include "Ecs/Scenes/Scene3D.hpp"

namespace Anantham::Ecs::Scenes
{

void Scene3D::update(float deltaTime) {}

void Scene3D::render(Rendering::RenderQueue &renderQueue)
{
    _meshRenderSystem.render(_registry, renderQueue);
}

} // namespace Anantham::Ecs::Scenes
