#pragma once

class Lights
{
private:
    unsigned int _VBO;
    unsigned int _VAO;
    float _vertices[180];
public:
    unsigned int Texture;
    Lights();
    ~Lights();
    void Init();
    void Draw();
};
