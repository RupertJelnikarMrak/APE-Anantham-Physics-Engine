#pragma once

#include "Rendering/Camera.hpp"
#include "Resources/Mesh.hpp"

namespace Anantham::Rendering
{

struct RenderCommand {
    Resources::Mesh *mesh;
    glm::mat4 transform;
};

class RenderQueue
{
public:
    std::vector<RenderCommand> opaqueCommands;

    Camera camera;
    glm::mat3 viewMatrix;
    glm::mat4 projectionMatrix;

    void submit(Resources::Mesh *mesh, const glm::mat4 &transform)
    {
        opaqueCommands.push_back({mesh, transform});
    }

    void clear() { opaqueCommands.clear(); }
};

} // namespace Anantham::Rendering
