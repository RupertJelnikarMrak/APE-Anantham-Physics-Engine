#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace Core::Input
{

class InputController
{
public:
    typedef std::unordered_map<std::string, std::vector<int>> ActionKeyMap;

    /**
     * @brief Constructor.
     * @param window A shared pointer to the GLFW window.
     */
    InputController(GLFWwindow *window);

    /**
     * @brief Destructor. Resets GLFW user pointer.
     */
    ~InputController();

    /**
     * @brief Binds a specific keyboard key to a named action.
     * @param actionName The name of the action (e.g., "Jump", "MoveForward").
     * @param key The GLFW key code (e.g., GLFW_KEY_W).
     */
    void bindKeyToAction(const std::string &actionName, int key);

    /**
     * @brief Binds a specific mouse button to a named action.
     * @param actionName The name of the action (e.g., "Fire", "Aim").
     * @param button The GLFW mouse button code (e.g., GLFW_MOUSE_BUTTON_LEFT).
     */
    void bindMouseButtonToAction(const std::string &actionName, int button);

    /**
     * @brief Sets default key bindings.
     * @param keyBindings A map of action names to key codes.
     */
    void setDefaultKeyBindings(ActionKeyMap &keyBindings);

    /**
     * @brief Sets default mouse button bindings.
     * @param mouseButtonBindings A map of action names to mouse button codes.
     */
    void setDefaultMouseButtonBindings(ActionKeyMap &mouseButtonBindings);

    /**
     * @brief Clears all key bindings for a specific action.
     * @param actionName The name of the action.
     */
    void clearKeyBind(const std::string &actionName);

    /**
     * @brief Resets all key bindings for a specific action to default.
     * @param actionName The name of the action.
     */
    void resetKeyBind(const std::string &actionName);

    /**
     * @brief Clears all mouse button bindings for a specific action.
     * @param actionName The name of the action.
     */
    void clearMouseButtonBind(const std::string &actionName);

    /**
     * @brief Resets all mouse button bindings for a specific action to default.
     * @param actionName The name of the action.
     */
    void resetMouseButtonBind(const std::string &actionName);

    /**
     * @brief Clears all action bindings.
     */
    void clearAllBindings();

    /**
     * @brief Resets all action bindings to default.
     */
    void resetAllBindings();

    bool isKeyDown(int key) const;
    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;

    bool isMouseButtonDown(int button) const;
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonReleased(int button) const;

    bool isActionDown(const std::string &actionName) const;
    bool isActionPressed(const std::string &actionName) const;
    bool isActionReleased(const std::string &actionName) const;

    void getCursorPosition(double &x, double &y) const;
    double getCursorX() const;
    double getCursorY() const;

    void getCursorDelta(double &x, double &y) const;
    double getCursorDeltaX() const;
    double getCursorDeltaY() const;

    void getScrollOffset(double &x, double &y) const;
    double getScrollX() const;
    double getScrollY() const;

    void getScrollDelta(double &x, double &y) const;
    double getScrollDeltaX() const;
    double getScrollDeltaY() const;

    void setCursorPosition(double x, double y);
    void setCursorMode(int mode);
    void setCursorShape(int shape);

    /**
     * @brief Updates input states. Call once per frame before querying input.
     * Copies current states to previous states and resets frame-specific deltas (like scroll).
     */
    void update();

private:
    // --- GLFW Callback Handlers (Member Functions) ---
    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleMouseButtonEvent(int button, int action, int mods);
    void handleCursorPosEvent(double xpos, double ypos);
    void handleScrollEvent(double xoffset, double yoffset);

    // --- Static GLFW Callbacks (Required for the C base GLFW API) ---
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    // --- Member Variables ---
    GLFWwindow *_window;

    // Action bindings
    ActionKeyMap _keyBindings;
    ActionKeyMap _mouseButtonBindings;
    ActionKeyMap _defaultKeyBindings;
    ActionKeyMap _defaultMouseButtonBindings;

    // Input states
    std::unordered_map<int, bool> _currentKeyStates;
    std::unordered_map<int, bool> _previousKeyStates;
    std::unordered_map<int, bool> _currentMouseButtonStates;
    std::unordered_map<int, bool> _previousMouseButtonStates;

    // Cursor state
    double _cursorX = 0.0;
    double _cursorY = 0.0;
    double _previousCursorX = 0.0;
    double _previousCursorY = 0.0;

    // Scroll state
    double _scrollX = 0.0;         // Total accumulated scroll X
    double _scrollY = 0.0;         // Total accumulated scroll Y
    double _previousScrollX = 0.0; // Previous total scroll X (for calculating delta if needed differently)
    double _previousScrollY = 0.0; // Previous total scroll Y (for calculating delta if needed differently)
};

} // namespace Core::Input
