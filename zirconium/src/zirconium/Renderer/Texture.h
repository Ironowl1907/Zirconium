#pragma once

#include <cstdint>
namespace zirconium {
class Texture {
public:
    virtual ~Texture() = default;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual void Bind(const uint32_t slot = 0) const = 0;
    virtual void SetData(const void* data, const uint32_t& size) const = 0;

private:
};

class Texture2D : public Texture {
public:
    static Ref<Texture2D> Create(const uint32_t& width, const uint32_t& height);
    static Ref<Texture2D> Create(const std::string& path);

    virtual bool operator==(const Texture2D& other) = 0;
};

class Texture2DLibrary {

public:
    Texture2DLibrary();
    ~Texture2DLibrary();

    void Add(const std::string& name, const Ref<Texture2D>& shader);
    Ref<Texture2D> Load(const std::string& name, const std::string& filePath);

private:
    std::unordered_map<std::string, Ref<Texture2D>> m_Shaders;
};
}; // namespace zirconium
