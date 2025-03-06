#include "first_app.hpp"

#include "ape_camera.hpp"
#include "ape_game_object.hpp"
#include "ape_model.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

// std
#include <cassert>
#include <chrono>
#include <memory>

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
    ApeCamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = ApeGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!apeWindow.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(apeWindow.getWindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = apeRenderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

        if (auto commandBuffer = apeRenderer.beginFrame()) {
            apeRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
            apeRenderer.endSwapChainRenderPass(commandBuffer);
            apeRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(apeDevice.device());
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<ApeModel> apeModel = ApeModel::createModleFromFile(apeDevice, "assets/models/smooth_vase.obj");

    auto cube = ApeGameObject::createGameObject();
    cube.model = apeModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};
    gameObjects.push_back(std::move(cube));
}

} // namespace ape
