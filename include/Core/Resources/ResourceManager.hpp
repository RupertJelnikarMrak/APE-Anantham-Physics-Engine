#pragma once

#include "Core/Rendering/GraphicsDevice.hpp"
#include "Core/Resources/Mesh.hpp"

#include <memory>
#include <unordered_map>

namespace Core::Resources
{

template <typename T> class ResourceCache
{
public:
    std::shared_ptr<T> get(const std::string &key) const
    {
        if (_cache.find(key) != _cache.end()) {
            return _cache.at(key);
        } else {
            return nullptr;
        }
    }

    void put(const std::string &key, std::shared_ptr<T> resource) { _cache[key] = resource; }

    bool contains(const std::string &key) const { return _cache.find(key) != _cache.end(); }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> _cache;
};

class ResourceManager
{
public:
    ResourceManager(Rendering::GraphicsDevice &device) : _device{device} {}
    ~ResourceManager() = default;

    ResourceCache<Mesh> meshCache;

private:
    Rendering::GraphicsDevice &_device;
};

} // namespace Core::Resources
