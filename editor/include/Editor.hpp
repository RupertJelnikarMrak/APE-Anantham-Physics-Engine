#pragma once
#include "Input/InputController.hpp"
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

    void run();

private:
    void init();
    void update();
    void render();
    void stop();

    Anantham::Platform::Window _window{
        EDITOR_WINDOW_WIDTH,
        EDITOR_WINDOW_HEIGHT,
        EDITOR_WINDOW_NAME};

    std::shared_ptr<Anantham::Input::InputController> _inputController{
        std::make_shared<Anantham::Input::InputController>(_window)};
};

} // namespace Editor
