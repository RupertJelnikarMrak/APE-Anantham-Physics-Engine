#pragma once

#include "Rendering/Camera.hpp"
#include "Resources/Mesh.hpp"

#include <entt/entity/entity.hpp>
#include <glm/glm.hpp>

#include <limits>
#include <vector>

namespace Anantham::Utils::Raycasting
{

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct AABB {
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

    void expand(const glm::vec3 &point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }

    glm::vec3 center() const { return (min + max) * 0.5f; }

    glm::vec3 size() const { return max - min; }
};

struct IntersectionResult {
    bool hit = false;
    float distance = std::numeric_limits<float>::max();
    glm::vec3 point = glm::vec3(0.0f);
    entt::entity entity = entt::null;

    explicit operator bool() const { return hit; }
};

/**
 * @brief Creates a Ray originating from the camera and pointing through the specified screen coordinates.
 * @param screenX Mouse X coordinate (pixels, origin top-left).
 * @param screenY Mouse Y coordinate (pixels, origin top-left).
 * @param screenWidth Width of the window/viewport in pixels.
 * @param screenHeight Height of the window/viewport in pixels.
 * @param camera The camera providing view and projection matrices.
 * @return A Ray object in world space.
 */
Ray createRayFromScreenCoords(
    float screenX,
    float screenY,
    float screenWidth,
    float screenHeight,
    const Anantham::Rendering::Camera &camera);

/**
 * @brief Performs Ray-Triangle intersection using the Möller–Trumbore algorithm.
 * @param ray The ray to test.
 * @param v0 First vertex of the triangle (world space).
 * @param v1 Second vertex of the triangle (world space).
 * @param v2 Third vertex of the triangle (world space).
 * @param outDistance If intersection occurs, the distance along the ray is stored here.
 * @return True if the ray intersects the triangle, false otherwise.
 */
bool intersectRayTriangle(
    const Ray &ray,
    const glm::vec3 &v0,
    const glm::vec3 &v1,
    const glm::vec3 &v2,
    float &outDistance);

/**
 * @brief Performs Ray-AABB (Axis-Aligned Bounding Box) intersection test.
 * Uses the Slab method.
 * @param ray The ray to test.
 * @param aabb The Axis-Aligned Bounding Box (world space).
 * @param outMinDistance If intersection occurs, the distance to the near intersection point.
 * @param outMaxDistance If intersection occurs, the distance to the far intersection point.
 * @return True if the ray intersects the AABB, false otherwise.
 */
bool intersectRayAABB(const Ray &ray, const AABB &aabb, float &outMinDistance, float &outMaxDistance);

/**
 * @brief Calculates the AABB for a given set of vertices.
 * @param vertices Vector of vertices.
 * @return The calculated AABB.
 */
AABB calculateAABB(const std::vector<Anantham::Resources::Mesh::Vertex> &vertices);

/**
 * @brief Transforms an AABB by a given transformation matrix.
 * @param aabb The original AABB.
 * @param transformMatrix The transformation matrix (e.g., model matrix).
 * @return The transformed AABB (will still be axis-aligned in world space, but potentially larger).
 */
AABB transformAABB(const AABB &aabb, const glm::mat4 &transformMatrix);

} // namespace Anantham::Utils::Raycasting
