#pragma once

#include "Input/InputController.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/FrameInfo.hpp"
#include "Rendering/Renderer.hpp"

namespace Anantham::Ecs::Systems
{

class CameraSystem
{
public:
    CameraSystem(Rendering::Camera &, Input::InputController &, Rendering::Renderer &);
    ~CameraSystem() = default;

    void update(Rendering::FrameInfo &);

private:
    void setupControls();

    Rendering::Camera &_camera;
    Input::InputController &_input;
    Rendering::Renderer &_renderer;
};

} // namespace Anantham::Ecs::Systems
