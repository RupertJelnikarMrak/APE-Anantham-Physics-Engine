#include "Ecs/Scenes/Scene3D.hpp"
#include "Ecs/Systems/CameraUpdateSystem.hpp"
#include "Ecs/Systems/MeshRenderSystem.hpp"
#include <memory>

namespace Anantham::Ecs::Scenes
{

Scene3D::Scene3D(
    Rendering::GraphicsDevice &graphicsDevice,
    Rendering::Renderer &renderer,
    Input::InputController &inputController)
    : _graphicsDevice(graphicsDevice), _renderer(renderer), _inputController(inputController)
{
    _updateSystems.push_back(
        std::make_unique<Ecs::Systems::CameraUpdateSystem>(_camera, _inputController, _renderer));
}

} // namespace Anantham::Ecs::Scenes
