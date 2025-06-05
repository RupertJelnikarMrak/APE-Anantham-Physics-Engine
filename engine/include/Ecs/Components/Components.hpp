#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Anantham::Ecs::Components
{

struct Transform {
    glm::vec3 translation{0.f, 0.f, 0.f};
    glm::vec3 rotation{0.f, 0.f, 0.f};
    glm::vec3 scale{1.f, 1.f, 1.f};
};

struct Mesh {
    std::string meshName;
};

struct Camera {
    bool active = false;
    float fov = glm::radians(45.f);
    float near = 0.1f;
    float far = 100.f;
    float lookSpeed = 1.f;
    float moveSpeed = 2.f;
};

struct Selected {
};

} // namespace Anantham::Ecs::Components
