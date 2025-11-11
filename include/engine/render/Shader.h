#pragma once
#include <string>
#include <unordered_map>

namespace engine {

    class Shader {
    public:
        Shader() = default;
        ~Shader();

        bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
        void bind() const;
        void unbind() const;

        void setMat4(const std::string& name, const float* value);
        void setFloat(const std::string& name, float value);

        bool needsReload();
        void reloadIfNeeded();

    private:
        unsigned int compileShader(unsigned int type, const std::string& source);
        bool checkCompileErrors(unsigned int shader, const std::string& type);
        int getUniformLocation(const std::string& name);

        unsigned int m_program = 0;
        std::string m_vertexPath;
        std::string m_fragmentPath;
        std::unordered_map<std::string, int> m_uniformLocationCache;

        std::string readFile(const std::string& path);
        long getFileTime(const std::string& path);
        long m_vertexTime = 0;
        long m_fragmentTime = 0;
    };

}  // namespace engine