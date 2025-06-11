#pragma once

namespace Anantham::Ecs::Scenes
{

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    Base(const Base &) = delete;
    Base &operator=(const Base &) = delete;
    Base(Base &&) = delete;
    Base &operator=(Base &&) = delete;

    /**
     * @brief Draws the scene for the current frame.
     * @param frameTime The time elapsed since the last frame.
     */
    virtual void drawFrame(float frameTime) = 0;
};

} // namespace Anantham::Ecs::Scenes
