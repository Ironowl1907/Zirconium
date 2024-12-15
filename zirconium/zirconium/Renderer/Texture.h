#pragma once

namespace zirconium {
class Texture {
public:
    virtual ~Texture() = default;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(const uint32_t slot = 0) const = 0;

private:
};

class Texture2D : public Texture {
public:
    static Ref<Texture2D> Create(const std::string& path);
};
}; // namespace zirconium
