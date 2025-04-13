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

    virtual const std::string& GetPath() const = 0;
    virtual bool operator==(const Texture2D& other) = 0;
};

class Texture2DLibrary {
public:
    static Texture2DLibrary* Get() {
        return m_Instance;
    }
    static void Init();

    Ref<Texture2D> Add(const std::string& path);
    Ref<Texture2D> Load(const std::string& path);

private:
    Texture2DLibrary();
    ~Texture2DLibrary();
    static Texture2DLibrary* m_Instance;
    std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
};

}; // namespace zirconium
