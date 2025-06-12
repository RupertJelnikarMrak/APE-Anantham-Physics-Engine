#include "Ecs/Systems/CameraUpdateSystem.hpp"
#include "Ecs/Components/Components.hpp"
#include "Input/InputController.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <entt/entt.hpp>
#include <glm/gtc/constants.hpp>
#include <spdlog/spdlog.h>

namespace Anantham::Ecs::Systems
{

#define MOUSE_SENSITIVITY 0.001f

CameraUpdateSystem::CameraUpdateSystem(
    Rendering::Camera &camera,
    Input::InputController &input,
    Rendering::Renderer &renderer)
    : _camera(camera), _input(input), _renderer(renderer)
{
    setupControls();
}

void CameraUpdateSystem::update(entt::registry &registry, float deltaTime)
{
    entt::entity cameraEntity = entt::null;
    registry.view<const Components::Camera>().each(
        [&](auto entity, const Components::Camera &camera) {
            if (camera.active) {
                cameraEntity = entity;
            }
        });
    if (cameraEntity == entt::null) {
        SPDLOG_WARN("No active camera found in the scene.");
        return;
    }

    auto &cameraTransform = registry.get<Components::Transform>(cameraEntity);
    auto &cameraComponent = registry.get<Components::Camera>(cameraEntity);

    float aspect = _renderer.getAspectRatio();
    _camera.setPerspectiveProjection(
        cameraComponent.fov,
        aspect,
        cameraComponent.near,
        cameraComponent.far);

    // Handle camera movement

    double scrollDeltaY = _input.getScrollDeltaY();
    if (std::abs(scrollDeltaY) > std::numeric_limits<double>::epsilon()) {
        const float speedChangeFactor = 0.1f;

        float speedStep = cameraComponent.moveSpeed * speedChangeFactor;

        const float minSpeedStep = 0.05f;
        speedStep = std::max(speedStep, minSpeedStep);

        if (scrollDeltaY > 0) {
            cameraComponent.moveSpeed += speedStep;
        } else {
            cameraComponent.moveSpeed -= speedStep;
        }

        cameraComponent.moveSpeed = std::clamp(cameraComponent.moveSpeed, .1f, 100.f);
    }

    if (_input.isActionPressed("CameraLookAround"))
        _input.setCursorMode(GLFW_CURSOR_DISABLED);
    else if (_input.isActionReleased("CameraLookAround"))
        _input.setCursorMode(GLFW_CURSOR_NORMAL);

    glm::vec3 rotate{0};
    if (_input.isActionDown("CameraLookAround")) {
        rotate.y += _input.getCursorDeltaX();
        rotate.x -= _input.getCursorDeltaY();

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            cameraTransform.rotation += cameraComponent.lookSpeed * MOUSE_SENSITIVITY * rotate;
        }
    } else {
        if (_input.isActionDown("CameraLookRight"))
            rotate.y += 1.f;
        if (_input.isActionDown("CameraLookLeft"))
            rotate.y -= 1.f;
        if (_input.isActionDown("CameraLookUp"))
            rotate.x += 1.f;
        if (_input.isActionDown("CameraLookDown"))
            rotate.x -= 1.f;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            cameraTransform.rotation +=
                cameraComponent.lookSpeed * deltaTime * glm::normalize(rotate);
        }
    }

    // limit pitch values between about +/- 85ish degrees
    cameraTransform.rotation.x = glm::clamp(cameraTransform.rotation.x, -1.5f, 1.5f);
    cameraTransform.rotation.y = glm::mod(cameraTransform.rotation.y, glm::two_pi<float>());

    float yaw = cameraTransform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0};
    if (_input.isActionDown("CameraMoveForward"))
        moveDir += forwardDir;
    if (_input.isActionDown("CameraMoveBackward"))
        moveDir -= forwardDir;
    if (_input.isActionDown("CameraMoveRight"))
        moveDir += rightDir;
    if (_input.isActionDown("CameraMoveLeft"))
        moveDir -= rightDir;
    if (_input.isActionDown("CameraMoveUp"))
        moveDir += upDir;
    if (_input.isActionDown("CameraMoveDown"))
        moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        cameraTransform.translation +=
            cameraComponent.moveSpeed * deltaTime * glm::normalize(moveDir);
    }

    _camera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);
}

void CameraUpdateSystem::setupControls()
{
    // TODO: One day this should become a default binding
    _input.bindKeyToAction("CameraMoveForward", GLFW_KEY_W);
    _input.bindKeyToAction("CameraMoveBackward", GLFW_KEY_S);
    _input.bindKeyToAction("CameraMoveLeft", GLFW_KEY_A);
    _input.bindKeyToAction("CameraMoveRight", GLFW_KEY_D);
    _input.bindKeyToAction("CameraMoveUp", GLFW_KEY_SPACE);
    _input.bindKeyToAction("CameraMoveDown", GLFW_KEY_LEFT_CONTROL);
    _input.bindKeyToAction("CameraLookUp", GLFW_KEY_UP);
    _input.bindKeyToAction("CameraLookDown", GLFW_KEY_DOWN);
    _input.bindKeyToAction("CameraLookLeft", GLFW_KEY_LEFT);
    _input.bindKeyToAction("CameraLookLeft", GLFW_KEY_Q);
    _input.bindKeyToAction("CameraLookRight", GLFW_KEY_RIGHT);
    _input.bindKeyToAction("CameraLookRight", GLFW_KEY_E);
    _input.bindMouseButtonToAction("CameraLookAround", GLFW_MOUSE_BUTTON_2);
}

} // namespace Anantham::Ecs::Systems
