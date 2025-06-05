#pragma once

#include "Rendering/Buffer.hpp"
#include "Rendering/GraphicsDevice.hpp"

// lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <cstdint>
#include <memory>
#include <string>

namespace Anantham::Resources
{

class Mesh
{
public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex &other) const
        {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

    struct RawMesh {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadMesh(const std::string &filePath);
    };

    Mesh(Rendering::GraphicsDevice &device, const RawMesh &rawMesh);
    ~Mesh();

    static std::shared_ptr<Mesh> createMeshFromFile(Rendering::GraphicsDevice &device, const std::string &filepath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

    const RawMesh &getRawMesh() const { return _rawMesh; }

private:
    const RawMesh &_rawMesh;

    void createVertexBuffers(const std::vector<Vertex> &vertices, Rendering::GraphicsDevice &device);
    void createIndexBuffers(const std::vector<uint32_t> &indices, Rendering::GraphicsDevice &device);

    std::unique_ptr<Rendering::Buffer> _vertexBuffer;
    uint32_t _vertexCount;

    bool _hasIndexBuffer = false;
    std::unique_ptr<Rendering::Buffer> _indexBuffer;
    uint32_t _indexCount;
};

} // namespace Anantham::Resources
