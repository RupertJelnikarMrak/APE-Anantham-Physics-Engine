#pragma once

#include "Core/Input/InputController.hpp"
#include "Core/Rendering/Camera.hpp"

// lib
#include <entt/entity/fwd.hpp>
#include <vulkan/vulkan.h>

namespace Core::Rendering
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
    entt::registry &registry;
    Input::InputController &inputController;
};
} // namespace Core::Rendering
