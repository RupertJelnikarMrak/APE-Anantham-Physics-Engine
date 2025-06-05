#pragma once
#include "Platform/Window.hpp"

namespace Editor
{
#define EDITOR_WINDOW_WIDTH 1920
#define EDITOR_WINDOW_HEIGHT 1080
#define EDITOR_WINDOW_NAME "Anantham Editor"

class Editor
{
public:
    Editor() = default;
    ~Editor() = default;

    void Run();

private:
    void Update();
    void Render();
    void Shutdown();

    Anantham::Platform::Window _window{
        EDITOR_WINDOW_WIDTH,
        EDITOR_WINDOW_HEIGHT,
        EDITOR_WINDOW_NAME};
};

} // namespace Editor
