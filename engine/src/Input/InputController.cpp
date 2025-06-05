#include "Input/InputController.hpp"
#include <GLFW/glfw3.h>

namespace Anantham::Input
{

InputController::InputController(Platform::Window &window) : _window(window)
{
    _window.getCursorPosition(_cursorX, _cursorY);
    _previousCursorX = _cursorX;
    _previousCursorY = _cursorY;

    _scrollX = 0.0;
    _scrollY = 0.0;
    _previousScrollX = 0.0;
    _previousScrollY = 0.0;
}

void InputController::bindKeyToAction(const std::string &actionName, int key)
{
    _keyBindings[actionName].push_back(key);
}

void InputController::bindMouseButtonToAction(const std::string &actionName, int button)
{
    _mouseButtonBindings[actionName].push_back(button);
}

void InputController::setDefaultKeyBindings(ActionKeyMap &keyBindings)
{
    _defaultKeyBindings = keyBindings;
    _keyBindings = keyBindings;
}

void InputController::setDefaultMouseButtonBindings(ActionKeyMap &mouseButtonBindings)
{
    _defaultMouseButtonBindings = mouseButtonBindings;
    _mouseButtonBindings = mouseButtonBindings;
}

void InputController::clearKeyBind(const std::string &actionName) { _keyBindings.erase(actionName); }

void InputController::resetKeyBind(const std::string &actionName)
{
    auto it = _defaultKeyBindings.find(actionName);
    if (it != _defaultKeyBindings.end()) {
        _keyBindings[actionName] = it->second;
    } else {
        _keyBindings.erase(actionName);
    }
}

void InputController::clearMouseButtonBind(const std::string &actionName) { _mouseButtonBindings.erase(actionName); }

void InputController::resetMouseButtonBind(const std::string &actionName)
{
    auto it = _defaultMouseButtonBindings.find(actionName);
    if (it != _defaultMouseButtonBindings.end()) {
        _mouseButtonBindings[actionName] = it->second;
    } else {
        _mouseButtonBindings.erase(actionName);
    }
}

void InputController::clearAllBindings()
{
    _keyBindings.clear();
    _mouseButtonBindings.clear();
}

void InputController::resetAllBindings()
{
    _keyBindings = _defaultKeyBindings;
    _mouseButtonBindings = _defaultMouseButtonBindings;
}

bool InputController::isKeyDown(int key) const
{
    auto it = _currentKeyStates.find(key);
    return it != _currentKeyStates.end() && it->second;
}

bool InputController::isKeyPressed(int key) const
{
    bool current = isKeyDown(key);
    auto it_prev = _previousKeyStates.find(key);
    bool previous = (it_prev != _previousKeyStates.end() && it_prev->second);
    return current && !previous;
}

bool InputController::isKeyReleased(int key) const
{
    bool current = isKeyDown(key);
    auto it_prev = _previousKeyStates.find(key);
    bool previous = (it_prev != _previousKeyStates.end() && it_prev->second);
    return !current && previous;
}

bool InputController::isMouseButtonDown(int button) const
{
    auto it = _currentMouseButtonStates.find(button);
    return it != _currentMouseButtonStates.end() && it->second;
}

bool InputController::isMouseButtonPressed(int button) const
{
    bool current = isMouseButtonDown(button);
    auto it_prev = _previousMouseButtonStates.find(button);
    bool previous = (it_prev != _previousMouseButtonStates.end() && it_prev->second);
    return current && !previous;
}

bool InputController::isMouseButtonReleased(int button) const
{
    bool current = isMouseButtonDown(button);
    auto it_prev = _previousMouseButtonStates.find(button);
    bool previous = (it_prev != _previousMouseButtonStates.end() && it_prev->second);
    return !current && previous;
}

bool InputController::isActionDown(const std::string &actionName) const
{
    auto keyIt = _keyBindings.find(actionName);
    if (keyIt != _keyBindings.end()) {
        for (int key : keyIt->second) {
            if (isKeyDown(key)) {
                return true;
            }
        }
    }
    auto mouseIt = _mouseButtonBindings.find(actionName);
    if (mouseIt != _mouseButtonBindings.end()) {
        for (int button : mouseIt->second) {
            if (isMouseButtonDown(button)) {
                return true;
            }
        }
    }
    return false;
}

bool InputController::isActionPressed(const std::string &actionName) const
{
    auto keyIt = _keyBindings.find(actionName);
    if (keyIt != _keyBindings.end()) {
        for (int key : keyIt->second) {
            if (isKeyPressed(key)) {
                return true;
            }
        }
    }
    auto mouseIt = _mouseButtonBindings.find(actionName);
    if (mouseIt != _mouseButtonBindings.end()) {
        for (int button : mouseIt->second) {
            if (isMouseButtonPressed(button)) {
                return true;
            }
        }
    }
    return false;
}

bool InputController::isActionReleased(const std::string &actionName) const
{
    auto keyIt = _keyBindings.find(actionName);
    if (keyIt != _keyBindings.end()) {
        for (int key : keyIt->second) {
            if (isKeyReleased(key)) {
                return true;
            }
        }
    }
    auto mouseIt = _mouseButtonBindings.find(actionName);
    if (mouseIt != _mouseButtonBindings.end()) {
        for (int button : mouseIt->second) {
            if (isMouseButtonReleased(button)) {
                return true;
            }
        }
    }
    return false;
}

void InputController::getCursorPosition(double &x, double &y) const
{
    x = _cursorX;
    y = _cursorY;
}

double InputController::getCursorX() const { return _cursorX; }

double InputController::getCursorY() const { return _cursorY; }

void InputController::getCursorDelta(double &x, double &y) const
{
    x = _cursorX - _previousCursorX;
    y = _cursorY - _previousCursorY;
}

double InputController::getCursorDeltaX() const { return _cursorX - _previousCursorX; }

double InputController::getCursorDeltaY() const { return _cursorY - _previousCursorY; }

void InputController::getScrollOffset(double &x, double &y) const
{
    x = _scrollX;
    y = _scrollY;
}

double InputController::getScrollX() const { return _scrollX; }

double InputController::getScrollY() const { return _scrollY; }

void InputController::getScrollDelta(double &x, double &y) const
{
    x = _scrollX - _previousScrollX;
    y = _scrollY - _previousScrollY;
}

double InputController::getScrollDeltaX() const { return _scrollX - _previousScrollX; }

double InputController::getScrollDeltaY() const { return _scrollY - _previousScrollY; }

void InputController::setCursorPosition(double x, double y) { _window.setCursorPosition(x, y); }

void InputController::setCursorMode(int mode) { _window.setCursorMode(mode); }

void InputController::setCursorShape(int shape) { _window.setCursorShape(shape); }

void InputController::update()
{
    _previousKeyStates = _currentKeyStates;
    _previousMouseButtonStates = _currentMouseButtonStates;
    _previousCursorX = _cursorX;
    _previousCursorY = _cursorY;
    _previousScrollX = _scrollX;
    _previousScrollY = _scrollY;

    glfwPollEvents();
}

void InputController::handleKeyEvent(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        _currentKeyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        _currentKeyStates[key] = false;
    }
}

void InputController::handleMouseButtonEvent(int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        _currentMouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        _currentMouseButtonStates[button] = false;
    }
}

void InputController::handleCursorPosEvent(double xpos, double ypos)
{
    _cursorX = xpos;
    _cursorY = ypos;
}

void InputController::handleScrollEvent(double xoffset, double yoffset)
{
    _scrollX += xoffset;
    _scrollY += yoffset;
}

} // namespace Anantham::Input
