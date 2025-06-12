#pragma once

#include "Ecs/Systems/ISystem.hpp"
#include "Input/InputController.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Renderer.hpp"
#include <entt/entity/fwd.hpp>

namespace Anantham::Ecs::Systems
{

class CameraUpdateSystem : public IUpdateSystem
{
public:
    CameraUpdateSystem(Rendering::Camera &, Input::InputController &, Rendering::Renderer &);
    ~CameraUpdateSystem() = default;

    void update(entt::registry &registry, float deltaTime) override;

private:
    void setupControls();

    Rendering::Camera &_camera;
    Input::InputController &_input;
    Rendering::Renderer &_renderer;
};

} // namespace Anantham::Ecs::Systems
