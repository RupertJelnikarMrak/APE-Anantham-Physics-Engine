#include "Editor.hpp"

namespace Editor
{

void Editor::Run()
{
    while (!_window.shouldClose()) {
        Update();
        Render();
        _window.resetResizedFlag();
    }
}

void Editor::Update() {}

void Editor::Render() {}

} // namespace Editor
