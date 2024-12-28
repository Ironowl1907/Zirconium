#pragma once
#include "zirconium.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <sstream>
#include <vector>

class Pipe {
public:
    Pipe(glm::vec2 position, bool alive);
    ~Pipe();

    void OnUpdate(const zirconium::TimeStep& delta);
    void OnRender();

    inline std::string ToString() const {
        std::stringstream ss;
        ss << "[ " << m_Position.x << ' ' << m_Position.y << " ]";
        return ss.str();
    }

private:
    glm::vec2 m_Position;
    bool m_Alive;
};

class PipeContainer {
public:
    PipeContainer(uint32_t size);
    ~PipeContainer();

    void Init();

    std::vector<Pipe>::iterator begin();
    std::vector<Pipe>::iterator end();

    std::vector<Pipe>::const_iterator begin() const;
    std::vector<Pipe>::const_iterator end() const;

    void OnUpdate(const zirconium::TimeStep& delta);
    void OnRender();

    void GenPipe(const Pipe pipe);

private:
    uint32_t m_ContainerIndex = 0;
    size_t m_ContainerSize;
    std::vector<Pipe> m_Container;
};
