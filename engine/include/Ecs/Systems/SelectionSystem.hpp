#pragma once

#include "Input/InputController.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/FrameInfo.hpp"

namespace Anantham::Ecs::Systems
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

} // namespace Anantham::Ecs::Systems
