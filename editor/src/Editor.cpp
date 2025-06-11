#include "Editor.hpp"
#include "Platform/Window.hpp"
#include "Rendering/Descriptors.hpp"
#include <memory>

namespace Editor
{

Editor::Editor() { init(); }

void Editor::init()
{
    _inputController = _window.getInputController();
    _imguiPool = Anantham::Rendering::DescriptorPool::Builder(_graphicsDevice).build();

    // TODO I have still not decided whether there is only one sceen which is the Engine window
    // sceen or to have 2 or more active scenes including the Editor buttons etc.
    _activeScene = nullptr;
}

void Editor::stop() {}

void Editor::run()
{
    init();
    while (!_window.shouldClose()) {
        update();
        render();
        _window.resetResizedFlag();
    }
    stop();
}

void Editor::update()
{
    _inputController->update();
    _window.pollEvents();
}

void Editor::render() {}

} // namespace Editor
