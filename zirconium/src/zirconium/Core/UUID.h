#pragma once

#include <cstdint>
namespace zirconium {
class UUID {
public:
    UUID();
    UUID(uint64_t uuid);

    operator uint64_t() const {
        return m_UUID;
    }

private:
    uint64_t m_UUID;
};
} // namespace zirconium

namespace std {
template <>
struct hash<zirconium::UUID> {
    std::size_t operator()(const zirconium::UUID& uuid) const {
        return hash<uint64_t>()((uint64_t)uuid);
    }
};
} // namespace std
