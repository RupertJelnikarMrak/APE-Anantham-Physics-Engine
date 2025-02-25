#pragma once

namespace Core
{

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &&) = delete;

private:
};

} // namespace Core
