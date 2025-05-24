#include "Core/Utils/Raycasting/Raycasting.hpp"
#include "Core/Rendering/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp> // For glm::inverse
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp> // For glm::intersectRayTriangle

namespace Core::Utils::Raycasting
{

constexpr float EPSILON = 1e-6f;

Ray createRayFromScreenCoords(
    float screenX,
    float screenY,
    float screenWidth,
    float screenHeight,
    const Core::Rendering::Camera &camera)
{
    float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
    float ndcY = (2.0f * screenY) / screenHeight - 1.0f;

    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

    glm::mat4 invProjection = camera.getInverseProjection();
    glm::vec4 eyeCoords = invProjection * clipCoords;

    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    glm::mat4 invView = camera.getInverseView();
    glm::vec4 worldCoordsRayDir = invView * eyeCoords;

    Ray ray;
    ray.origin = camera.getPosition();
    ray.direction = glm::normalize(glm::vec3(worldCoordsRayDir));

    return ray;
}

// Möller–Trumbore intersection algorithm
bool intersectRayTriangle(
    const Ray &ray,
    const glm::vec3 &v0,
    const glm::vec3 &v1,
    const glm::vec3 &v2,
    float &outDistance)
{
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;

    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v0;
    float u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f)
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(ray.direction, q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * glm::dot(edge2, q);

    if (t > EPSILON) {
        outDistance = t;
        return true;
    } else
        return false;
}

// Slab method for Ray-AABB intersection
bool intersectRayAABB(const Ray &ray, const AABB &aabb, float &outMinDistance, float &outMaxDistance)
{
    glm::vec3 invDir = 1.0f / ray.direction;
    glm::vec3 tMin = (aabb.min - ray.origin) * invDir;
    glm::vec3 tMax = (aabb.max - ray.origin) * invDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    if (tNear <= tFar && tFar > 0.0f) {
        outMinDistance = glm::max(0.0f, tNear);
        outMaxDistance = tFar;
        return true;
    }

    return false;
}

AABB calculateAABB(const std::vector<Core::Resources::Mesh::Vertex> &vertices)
{
    AABB aabb;
    if (vertices.empty()) {
        aabb.min = glm::vec3(0.0f);
        aabb.max = glm::vec3(0.0f);
        return aabb;
    }

    aabb.min = vertices[0].position;
    aabb.max = vertices[0].position;

    for (size_t i = 1; i < vertices.size(); ++i) {
        aabb.expand(vertices[i].position);
    }
    return aabb;
}

AABB transformAABB(const AABB &aabb, const glm::mat4 &transformMatrix)
{
    glm::vec3 corners[8] = {
        glm::vec3(aabb.min.x, aabb.min.y, aabb.min.z),
        glm::vec3(aabb.max.x, aabb.min.y, aabb.min.z),
        glm::vec3(aabb.min.x, aabb.max.y, aabb.min.z),
        glm::vec3(aabb.min.x, aabb.min.y, aabb.max.z),
        glm::vec3(aabb.max.x, aabb.max.y, aabb.min.z),
        glm::vec3(aabb.max.x, aabb.min.y, aabb.max.z),
        glm::vec3(aabb.min.x, aabb.max.y, aabb.max.z),
        glm::vec3(aabb.max.x, aabb.max.y, aabb.max.z)};

    AABB transformedAABB;
    transformedAABB.min = glm::vec3(transformMatrix * glm::vec4(corners[0], 1.0f));
    transformedAABB.max = transformedAABB.min;

    for (int i = 1; i < 8; ++i) {
        glm::vec3 transformedCorner = glm::vec3(transformMatrix * glm::vec4(corners[i], 1.0f));
        transformedAABB.expand(transformedCorner);
    }

    return transformedAABB;
}

} // namespace Core::Utils::Raycasting
