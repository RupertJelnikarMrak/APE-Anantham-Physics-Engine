#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Core::Rendering
{

class Camera
{
public:
    void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovY, float aspect, float near, float far);

    void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewXYZ(glm::vec3 position, glm::vec3 rotation);

    const glm::mat4 &getProjection() const { return _projectionMatrix; }
    const glm::mat4 &getView() const { return _viewMatrix; }
    const glm::mat4 &getInverseView() const { return _inverseViewMatrix; }
    const glm::vec3 getPosition() const { return glm::vec3(_inverseViewMatrix[3]); }

private:
    glm::mat4 _projectionMatrix{1.f};
    glm::mat4 _viewMatrix{1.f};
    glm::mat4 _inverseViewMatrix{1.f};
};

} // namespace Core::Rendering
