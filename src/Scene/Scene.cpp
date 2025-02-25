#include "Core/Scene/Scene.hpp"

namespace Core
{

Scene::Scene() { entt::entity entity = m_Registry.create(); }

} // namespace Core
