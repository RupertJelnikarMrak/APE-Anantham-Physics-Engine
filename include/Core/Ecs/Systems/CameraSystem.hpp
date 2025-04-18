#pragma once

#include "Core/Input/InputController.hpp"
#include "Core/Rendering/Camera.hpp"
#include "Core/Rendering/FrameInfo.hpp"
#include "Core/Rendering/Renderer.hpp"
namespace Core::Ecs::Systems
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

} // namespace Core::Ecs::Systems
