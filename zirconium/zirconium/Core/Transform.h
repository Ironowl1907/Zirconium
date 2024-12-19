#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zirconium {
struct Transform {
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;

    inline Transform(glm::vec3 position, glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scale = {1.0f, 1.0f, 1.0f})
        : Position(position)
        , Scale(scale)
        , Rotation(rotation) {}

    inline operator glm::mat4() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);

        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), Scale);

        return translationMatrix * rotationMatrix * scalingMatrix;
    }
};
}; // namespace zirconium
