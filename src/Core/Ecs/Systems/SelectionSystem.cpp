#include "Core/Ecs/Systems/SelectionSystem.hpp"

namespace Core::Ecs::Systems
{

SelectionSystem::SelectionSystem(Rendering::Camera &camera, Input::InputController &input)
    : _camera(camera), _input(input)
{
    setupControls();
}

void SelectionSystem::update(Rendering::FrameInfo &frameInfo)
{
    if (_input.isActionDown("EntitySelect")) {
        glm::vec2 cursorPos{_input.getCursorX(), _input.getCursorY()};
    }
}

void SelectionSystem::setupControls()
{
    _input.bindMouseButtonToAction("EntitySelect", GLFW_MOUSE_BUTTON_1);
    //
}

} // namespace Core::Ecs::Systems
