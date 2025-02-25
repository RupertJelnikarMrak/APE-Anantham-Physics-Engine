#pragma once

#include "ape_device.hpp"
#include "ape_game_object.hpp"
#include "ape_renderer.hpp"
#include "ape_window.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ape
{

class FirstApp
{
public:
    static constexpr int WIDTH = 1800;
    static constexpr int HEIGHT = 1350;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

private:
    void loadGameObjects();

    ApeWindow apeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    ApeDevice apeDevice{apeWindow};
    ApeRenderer apeRenderer{apeWindow, apeDevice};

    std::vector<ApeGameObject> gameObjects;
};
} // namespace ape
