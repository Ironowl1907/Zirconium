#include "zrpch.h"

#include "glad/glad.h"

#include "OpenGLShader.h"
#include "core.h"
#include "glm/gtc/type_ptr.hpp"

namespace zirconium {

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name) {

    ZR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderMap;
    shaderMap[GL_VERTEX_SHADER] = vertexSrc;
    shaderMap[GL_FRAGMENT_SHADER] = fragmentSrc;

    Compile(shaderMap);
}

OpenGLShader::OpenGLShader(const std::string& filePath) {

    ZR_PROFILE_FUNCTION();

    std::string shaderSrc = ReadFile(filePath);
    auto shaderSrcs = PreProcess(shaderSrc);
    Compile(shaderSrcs);

    auto lastSlash = filePath.find_last_of("/");
    auto lastDot = filePath.find_last_of(".");

    if (lastSlash == std::string::npos)
        lastSlash = -1;
    if (lastDot == std::string::npos)
        lastDot = filePath.size();

    m_Name = filePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& shaderSrc) {

    ZR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderMap;

    const std::string typeToken = "#type";
    size_t pos = shaderSrc.find(typeToken);
    while (pos != std::string::npos) {
        size_t eol = shaderSrc.find_first_of("\r\n", pos);
        if (eol == std::string::npos)
            ZR_CORE_ASSERT(false, "Syntax error: Missing end of line after #type");

        size_t begin = pos + typeToken.length() + 1; // +1 to skip the space after #type
        std::string type = shaderSrc.substr(begin, eol - begin);

        GLenum shaderType;
        if (type == "vertex") {
            shaderType = GL_VERTEX_SHADER;
        } else if (type == "fragment") {
            shaderType = GL_FRAGMENT_SHADER;
        } else if (type == "geometry") {
            shaderType = GL_GEOMETRY_SHADER;
        } else {
            ZR_CORE_ERROR("Undefined Type: {0}", type);
            ZR_CORE_ASSERT(false, "Syntaxis error: Undefined shader type");
        }

        size_t nextPos = shaderSrc.find(typeToken, eol);

        std::string shaderCode = shaderSrc.substr(eol + 1, nextPos - (eol + 1));

        // Trim shaderCode (optional, depending on your use case)
        shaderMap[shaderType] = shaderCode;

        pos = nextPos;
    }

    return shaderMap;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs) {

    ZR_PROFILE_FUNCTION();

    uint32_t program = glCreateProgram();

    std::vector<GLenum> shaderIDs;

    for (const auto& [type, source] : shaderSrcs) {
        shaderIDs.push_back(type);
        GLuint shader = glCreateShader(type);

        const GLchar* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            ZR_CORE_ERROR("{0}", infoLog.data());
            ZR_CORE_ASSERT(false, "Shader Compilation failiture!");
            return;
        }
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        for (auto shaderID : shaderIDs)
            glDeleteShader(shaderID);

        glDeleteProgram(program);
        return;
    }
    for (auto shaderID : shaderIDs)
        glDetachShader(program, shaderID);

    m_RendererID = program;
}

std::string OpenGLShader::ReadFile(const std::string& filePath) {

    ZR_PROFILE_FUNCTION();

    std::string result = "";
    std::ifstream in(filePath.c_str(), std::ios::in);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(result.data(), result.size());
        in.close();
    } else {
        ZR_CORE_ERROR("Could not open shader file! '{0}'", filePath);
    }
    return result;
}

OpenGLShader::~OpenGLShader() {

    ZR_PROFILE_FUNCTION();

    glDeleteProgram(m_RendererID);
}
void OpenGLShader::Bind() const {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
}
void OpenGLShader::Unbind() const {

    ZR_PROFILE_FUNCTION();

    glUseProgram(0);
}

void OpenGLShader::SetMatrix4f(const std::string& name, const glm::mat4& mat) {

    ZR_PROFILE_FUNCTION();

    SetUniformMatrix4f(name, mat);
}
void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec) {

    ZR_PROFILE_FUNCTION();

    SetUniformFloat3(name, vec);
}
void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec) {

    ZR_PROFILE_FUNCTION();

    SetUniformFloat4(name, vec);
}
void OpenGLShader::SetInt(const std::string& name, const int& n) {

    ZR_PROFILE_FUNCTION();

    SetUniformInt(name, n);
}

void OpenGLShader::SetUniformMatrix4f(const std::string& name, const glm::mat4& mat) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4 vec) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3 vec) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void OpenGLShader::SetFloat(const std::string& name, const float& n) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, n);
}

void OpenGLShader::SetUniformInt(const std::string& name, const int n) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, n);
}

void OpenGLShader::SetIntArray(const std::string& name, const int* arr, size_t count) {
    ZR_PROFILE_FUNCTION();

    SetUniformIntArray(name, arr, count);
}
void OpenGLShader::SetUniformIntArray(const std::string& name, const int* arr, size_t count) {

    ZR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, arr);
}
} // namespace zirconium
