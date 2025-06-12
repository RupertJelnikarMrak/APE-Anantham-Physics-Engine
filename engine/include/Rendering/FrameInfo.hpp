#pragma once

#include "Input/InputController.hpp"
#include "Rendering/Camera.hpp"

// lib
#include <entt/entity/fwd.hpp>
#include <vulkan/vulkan.h>

namespace Anantham::Rendering
{

#define MAX_LIGHTS 10

struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::mat4 inverseView{1.f};
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};
    glm::vec4 directionalLightColor{1.f, 1.f, 1.f, 1.f}; // w is intensity
    alignas(16) glm::vec3 directionalLightDirection{.1f, 1.f, .1f};
};

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
};
} // namespace Anantham::Rendering
