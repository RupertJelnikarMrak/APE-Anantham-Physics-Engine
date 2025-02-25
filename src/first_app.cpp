#include "first_app.hpp"

#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <cassert>

namespace ape
{

struct SimplePushConstantData {
    glm::mat2 transform{1.f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run()
{
    SimpleRenderSystem simpleRenderSystem{apeDevice, apeRenderer.getSwapChainRenderPass()};

    while (!apeWindow.shouldClose()) {
        glfwPollEvents();

        if (auto commandBuffer = apeRenderer.beginFrame()) {
            apeRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
            apeRenderer.endSwapChainRenderPass(commandBuffer);
            apeRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(apeDevice.device());
}

void FirstApp::loadGameObjects()
{
    std::vector<ApeModel::Vertex> vertices{
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
        {{0.0f, -0.5f}, {0.0f, 1.0f, 1.0f}},
    };
    auto apeModel = std::make_shared<ApeModel>(apeDevice, vertices);

    auto triangle = ApeGameObject::createGameObject();
    triangle.model = apeModel;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};

    gameObjects.push_back(std::move(triangle));
}

} // namespace ape
