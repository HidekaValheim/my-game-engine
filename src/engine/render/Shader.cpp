#include "engine/render/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

namespace engine {

    std::string Shader::readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader: " << path << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    long Shader::getFileTime(const std::string& path) {
        struct stat result;
        if (stat(path.c_str(), &result) == 0) {
            return result.st_mtime;
        }
        return 0;
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        }
        return id;
    }

    bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
                return false;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "PROGRAM_LINKING_ERROR:\n" << infoLog << std::endl;
                return false;
            }
        }
        return true;
    }

    bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
        m_vertexPath = vertexPath;
        m_fragmentPath = fragmentPath;

        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);
        if (vertexCode.empty() || fragmentCode.empty()) return false;

        unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
        unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertex);
        glAttachShader(m_program, fragment);
        glLinkProgram(m_program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (!checkCompileErrors(m_program, "PROGRAM")) {
            return false;
        }

        m_vertexTime = getFileTime(vertexPath);
        m_fragmentTime = getFileTime(fragmentPath);

        return true;
    }

    Shader::~Shader() {
        if (m_program) glDeleteProgram(m_program);
    }

    void Shader::bind() const {
        glUseProgram(m_program);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    int Shader::getUniformLocation(const std::string& name) {
        if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
            return m_uniformLocationCache[name];
        }
        int location = glGetUniformLocation(m_program, name.c_str());
        if (location == -1) {
            std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        }
        m_uniformLocationCache[name] = location;
        return location;
    }

    void Shader::setMat4(const std::string& name, const float* value) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
    }

    void Shader::setFloat(const std::string& name, float value) {
        glUniform1f(getUniformLocation(name), value);
    }

    bool Shader::needsReload() {
        return getFileTime(m_vertexPath) > m_vertexTime || getFileTime(m_fragmentPath) > m_fragmentTime;
    }

    void Shader::reloadIfNeeded() {
        if (needsReload()) {
            std::cout << "Hot-reloading shader..." << std::endl;
            unsigned int old = m_program;
            loadFromFile(m_vertexPath, m_fragmentPath);
            if (old) glDeleteProgram(old);
        }
    }

}  // namespace engine