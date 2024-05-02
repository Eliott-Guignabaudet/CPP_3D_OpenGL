#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Square
{
private:
    unsigned int _VBO;
    unsigned int _VAO;
    float _vertices[180];
    std::vector<glm::vec3> _all;

    void GenerateAll();
public:
    unsigned int Texture;
    Square();
    ~Square();
    void Init();
    void Draw();
};
