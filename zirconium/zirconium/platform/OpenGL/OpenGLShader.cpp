#include "zrpch.h"

#include "OpenGLShader.h"
#include "core.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace zirconium {

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(vertexShader);

        ZR_CORE_ERROR("{0}", infoLog.data());
        ZR_CORE_ASSERT(false, "Vertex Shader Compilation failiture!");
        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        ZR_CORE_ERROR("{0}", infoLog.data());
        ZR_CORE_ASSERT(false, "Fragment Shader Compilation failiture!");
        return;
    }

    m_RendererID = glCreateProgram();
    uint32_t program = m_RendererID;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}
OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}
void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}
void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetUniformMatrix4f(const std::string& name, const glm::mat4& mat) {
    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4 vec) {
    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(vec));
}
} // namespace zirconium
