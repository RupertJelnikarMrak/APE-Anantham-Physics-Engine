#pragma once

#include "Core/Input/InputController.hpp"
#include "Core/Rendering/Camera.hpp"
#include "Core/Rendering/FrameInfo.hpp"

namespace Core::Ecs::Systems
{

class SelectionSystem
{
public:
    SelectionSystem(Rendering::Camera &, Input::InputController &);
    ~SelectionSystem() = default;

    void update(Rendering::FrameInfo &);

private:
    void setupControls();

    Rendering::Camera &_camera;
    Input::InputController &_input;
};

} // namespace Core::Ecs::Systems
