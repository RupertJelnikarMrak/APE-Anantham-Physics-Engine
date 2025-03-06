#pragma once

#include "ape_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace ape
{

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

class ApeGameObject
{
public:
    using id_t = unsigned int;

    static ApeGameObject createGameObject()
    {
        static id_t currentId = 0;
        return ApeGameObject{currentId++};
    }

    ApeGameObject(const ApeGameObject &) = delete;
    ApeGameObject &operator=(const ApeGameObject &) = delete;
    ApeGameObject(ApeGameObject &&) = default;
    ApeGameObject &operator=(ApeGameObject &&) = default;

    id_t getId() { return id; }

    std::shared_ptr<ApeModel> model{};
    glm::vec3 color{};
    TransformComponent transform{};

private:
    ApeGameObject(id_t objId) : id{objId} {}

    id_t id{0};
};

} // namespace ape
