#pragma once

#include "Core/Engine.hpp"

// std
#include <memory>

namespace App
{

class App
{
public:
    App() = default;

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    App(App &&) = delete;
    App &operator=(App &&) = delete;

    void run();

private:
    std::unique_ptr<Core::Engine> engine{};
};

} // namespace App
