#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Mesh
{
public:
    unsigned int Texture;
    Mesh(const char* objPath, const char* texturePath,float scale = 1.f);
    ~Mesh();
    void Unload();
    void Draw();
private:
    void InitVectors(const char* objPath);
    void InitArray();
    void GenerateNormals();
    void GenerateAll();
    unsigned int _VAO,_VBO,_EBO;
    std::vector<glm::vec3> _vertice;
    std::vector<unsigned int> _verticeIndex;
    std::vector<glm::vec3> _allVertices;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec3> _normalsFromFile;
    std::vector<unsigned int> _normalIndexs;
    std::vector<glm::vec3> _uvs;
    std::vector<unsigned> _uvIndexs;
    
    std::vector<glm::vec3> _all;

    const char* _texturePath;
    std::vector<unsigned int> TriangulateByIndexs(const std::vector<unsigned int> &vertices);
    float _scale;
};
