#include "Editor.hpp"

namespace Editor
{

void Editor::init() { _window.setInputController(_inputController); }

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
