#pragma once

#include <entt/entt.hpp>

namespace ape
{

class Core
{
public:
    Core();
    ~Core();

    void run();

private:
    entt::registry registry;
    entt::dispatcher dispatcher;
};

} // namespace ape
