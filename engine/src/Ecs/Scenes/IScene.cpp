#include "Ecs/Scenes/IScene.hpp"
#include "Ecs/Systems/ISystem.hpp"

namespace Anantham::Ecs::Scenes
{

void IScene::update(float deltaTime)
{
    for (const auto &system : _updateSystems) {
        system->update(_registry, deltaTime);
    }
}

void IScene::render(Rendering::FrameInfo &frameInfo)
{
    for (const auto &system : _renderSystems) {
        system->render(_registry, frameInfo);
    }
}

} // namespace Anantham::Ecs::Scenes
