#include "Core/Ecs/Systems/CameraSystem.hpp"
#include "Core/Ecs/Components/Components.hpp"
#include "Core/Input/InputController.hpp"
#include "Core/Rendering/FrameInfo.hpp"

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <glm/gtc/constants.hpp>
#include <spdlog/spdlog.h>

namespace Core::Ecs::Systems
{

CameraSystem::CameraSystem(Rendering::Camera &camera, Input::InputController &input, Rendering::Renderer &renderer)
    : _camera(camera), _input(input), _renderer(renderer)
{
    setupControls();
}

void CameraSystem::update(Rendering::FrameInfo &frameInfo)
{
    entt::entity cameraEntity = entt::null;
    frameInfo.registry.view<const Components::Camera>().each([&](auto entity, const Components::Camera &camera) {
        if (camera.active) {
            cameraEntity = entity;
        }
    });
    if (cameraEntity == entt::null) {
        SPDLOG_WARN("No active camera found in the scene.");
        return;
    }

    auto &cameraTransform = frameInfo.registry.get<Components::Transform>(cameraEntity);
    auto &cameraComponent = frameInfo.registry.get<Components::Camera>(cameraEntity);

    float aspect = _renderer.getAspectRatio();
    _camera.setPerspectiveProjection(cameraComponent.fov, aspect, cameraComponent.near, cameraComponent.far);

    // Handle camera movement

    if (_input.isActionPressed("CameraLookAround"))
        _input.setCursorMode(GLFW_CURSOR_DISABLED);

    if (_input.isActionReleased("CameraLookAround"))
        _input.setCursorMode(GLFW_CURSOR_NORMAL);

    glm::vec3 rotate{0};
    if (_input.isActionDown("cameraLookAround")) {
        rotate.x = _input.getCursorDeltaX();
        rotate.y = _input.getCursorDeltaY();
    } else {
        if (_input.isActionDown("CameraLookRight"))
            rotate.y += 1.f;
        if (_input.isActionDown("CameraLookLeft"))
            rotate.y -= 1.f;
        if (_input.isActionDown("CameraLookUp"))
            rotate.x += 1.f;
        if (_input.isActionDown("CameraLookDown"))
            rotate.x -= 1.f;
    }

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        cameraTransform.rotation += cameraComponent.lookSpeed * frameInfo.frameTime * glm::normalize(rotate);
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
        cameraTransform.translation += cameraComponent.moveSpeed * frameInfo.frameTime * glm::normalize(moveDir);
    }

    _camera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);
}

void CameraSystem::setupControls()
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
    _input.bindKeyToAction("CameraLookRight", GLFW_KEY_RIGHT);
    _input.bindKeyToAction("cameraLookAround", GLFW_MOUSE_BUTTON_1);
}

} // namespace Core::Ecs::Systems
