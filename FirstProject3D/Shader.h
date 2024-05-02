#pragma once
#include <string>
#include <glm/glm.hpp>
class Shader
{
public:
    unsigned int ID;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    Shader(const char* vertexPath, const char* fragmentPath);
    void SetMatrix4(const std::string &name, const glm::mat4 &value) const;
    void SetVector3(const std::string &name, float x, float y, float z) const;
    void SetVector3(const std::string &name, glm::vec3 value) const;
    void SetFloat(const std::string &name, float value) const;

    void Process(std::string vertexCode, std::string fragmentCode);
    void SetInt(const std::string &name, int value);
private:
    std::string GetFileContent(const char* path);
};
