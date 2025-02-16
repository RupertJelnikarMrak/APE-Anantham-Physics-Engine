#pragma once

#include "ape_model.hpp"

// std
#include <memory>

namespace ape
{

struct Transform2dComponent {
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation{};

    glm::mat2 mat2()
    {
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMat{{c, s}, {-s, c}};

        glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
        return rotMat * scaleMat;
    }
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
    Transform2dComponent transform2d{};

private:
    ApeGameObject(id_t objId) : id{objId} {}

    id_t id{0};
};

} // namespace ape
