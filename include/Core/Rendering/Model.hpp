#pragma once

#include "Core/Rendering/Buffer.hpp"
#include "Core/Rendering/Device.hpp"
#include <cstdint>

// lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <string>

namespace Core::Rendering
{

class Model
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

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string &filePath);
    };

    Model(Device &device, const Builder &builder);
    ~Model();

    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

    static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &filePath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);

    Device &_device;
    std::unique_ptr<Buffer> _vertexBuffer;
    uint32_t _vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<Buffer> _indexBuffer;
    uint32_t _indexCount;
};

} // namespace Core::Rendering
