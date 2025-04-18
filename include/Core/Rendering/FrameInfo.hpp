#pragma once

#include "Core/Input/InputController.hpp"
#include "Core/Rendering/Camera.hpp"

// lib
#include <entt/entity/fwd.hpp>
#include <vulkan/vulkan.h>

namespace Core::Rendering
{

#define MAX_LIGHTS 10

struct PointLightUniform {
    glm::vec4 position{}; // ignore w
    glm::vec4 color{};    // w is intensity
};

struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::mat4 inverseView{1.f};
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f}; // w is intensity
    PointLightUniform pointLights[MAX_LIGHTS];
    int numLights;
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
