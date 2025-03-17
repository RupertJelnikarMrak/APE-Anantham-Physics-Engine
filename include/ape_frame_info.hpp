#pragma once

#include "ape_camera.hpp"

// lib
#include <vulkan/vulkan_core.h>

namespace ape
{

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    ApeCamera camera;
};

} // namespace ape
