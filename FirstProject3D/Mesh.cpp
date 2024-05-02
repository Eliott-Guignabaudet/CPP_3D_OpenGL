#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <glad/glad.h>

#include "stb_image.h"
#include "triangulate/polypartition.h"

Mesh::Mesh(const char* objPath, const char* texturePath, float scale) : _scale(scale), _texturePath(texturePath)
{
    InitVectors(objPath);
    GenerateNormals();
    GenerateAll();
    InitArray();
}

Mesh::~Mesh()
{
    Unload();
}

void Mesh::Unload()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

void Mesh::InitVectors(const char* objPath)
{
    std::ifstream objFile;
    objFile.open(objPath);
    for (std::string element; std::getline(objFile, element);)
    {
        std::stringstream temp(element);
        std::string tempSliced;
        temp >> tempSliced;
        if (tempSliced == "v")
        {
            std::string x,y,z;
            temp >> x;
            temp >> y;
            temp >> z;

            _vertice.push_back({std::stof(x) * _scale,std::stof(y) *_scale,std::stof(z) * _scale});
        }
        else if(tempSliced == "vn")
        {
            std::string x,y,z;
            temp >> x;
            temp >> y;
            temp >> z;
            _normalsFromFile.push_back({std::stof(x) * _scale,std::stof(y) *_scale,std::stof(z) * _scale});
        }
        else if (tempSliced == "vt")
        {
            std::string x,y,z;
            temp >> x;
            temp >> y;
            _uvs.push_back({std::stof(x) ,std::stof(y) ,0});
        }
        else if (tempSliced == "f")
        {
            std::vector<unsigned int> tempVerticesIndexs;
            std::map<unsigned int, unsigned int> verticesNormals;
            std::map<unsigned int, unsigned int> verticesTextures;
            for (std::string line; std::getline(temp, line, ' ');)
            {
                std::stringstream streamBlock(line);
                int i = 0;
                for (std::string blockElement; std::getline(streamBlock, blockElement, '/');)
                {
                    if (i == 0)
                    {
                        tempVerticesIndexs.push_back(std::stoul(blockElement) - 1);
                    }
                    if (i== 1 && blockElement != "")
                    {

                        verticesTextures.insert({tempVerticesIndexs[tempVerticesIndexs.size()-1], std::stoul(blockElement)});
                    }
                    if (i== 2 && blockElement != "")
                    {
                        verticesNormals.insert({tempVerticesIndexs[tempVerticesIndexs.size()-1], std::stoul(blockElement)});
                    }
                    i++;
                }
            }
            if (tempVerticesIndexs.size()%3 != 0 )
            {
                tempVerticesIndexs = TriangulateByIndexs(tempVerticesIndexs);
            }

            for (int i = 0; i < tempVerticesIndexs.size(); ++i)
            {
                _verticeIndex.push_back(tempVerticesIndexs[i]);
                // if (i < verticesNormals.size())
                // {
                    _normalIndexs.push_back(verticesNormals[tempVerticesIndexs[i]] - 1);
                // }
                // if (i < verticesTextures.size())
                // {
                    _uvIndexs.push_back(verticesTextures[tempVerticesIndexs[i]] - 1);
                // }
            }

        }
    }
    std::cout << "Vertcice indexs: " << _verticeIndex.size() << std::endl;
    objFile.close();
    for (int i = 0; i < _verticeIndex.size(); ++i)
    {
        _allVertices.push_back(_vertice[_verticeIndex[i]]);
    }

}

std::vector<unsigned int> Mesh::TriangulateByIndexs(const std::vector<unsigned>& vertices)
{
    std::vector<glm::vec2> glmPolygon2D;
    for (int i = 0; i < vertices.size(); ++i)
    {
        glm::vec2 tempVertice2D;
        tempVertice2D.x = _vertice[vertices[i]].x;
        tempVertice2D.y = _vertice[vertices[i]].y;
        glmPolygon2D.push_back(tempVertice2D);
    }
    TPPLPoly tpplPolygon;
    tpplPolygon.Init(glmPolygon2D.size());
    for (int i = 0; i < glmPolygon2D.size(); ++i)
    {
        tpplPolygon[i].x = glmPolygon2D[i].x;
        tpplPolygon[i].y = glmPolygon2D[i].y;
        tpplPolygon[i].id = vertices[i];
    }
    tpplPolygon.SetOrientation(TPPL_ORIENTATION_CCW);
    TPPLPolyList result;
    TPPLPartition partition;
    partition.Triangulate_EC(&tpplPolygon, &result);

    std::vector<unsigned int> triangulatePoints;
    for (auto poly2D : result)
    {
        for (int i = 0; i < poly2D.GetNumPoints(); ++i)
        {
            triangulatePoints.push_back(poly2D[i].id);
        }
    }
    // if (triangulatePoints.size() == 0)
    // {
    //     std::cout << "(";
    //     for (int i = 0; i < tpplPolygon.GetNumPoints(); ++i)
    //     {
    //         std::cout << "(" << tpplPolygon[i].x << ", " << tpplPolygon[i].y << ") ";
    //     }
    //     std::cout << ")" << std::endl;
    // }
    
    return triangulatePoints;
}



void Mesh::GenerateNormals()
{
    if (_normalsFromFile.size() > 0)
    {
        for (int i = 0; i < _normalIndexs.size(); ++i)
        {
            _normals.push_back(_normalsFromFile[_normalIndexs[i]]);
        }
        return;
    }
    for (int i = 0; i < _allVertices.size(); i += 3)
    {
        glm::vec3 p1 = _allVertices[i];
        glm::vec3 p2 = _allVertices[i + 1];
        glm::vec3 p3 = _allVertices[i + 2];

        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;

        glm::vec3 normal =  glm::cross(u, v);

        _normals.push_back(normal);
        _normals.push_back(normal);
        _normals.push_back(normal);
    }
}

void Mesh::GenerateAll()
{
    std::cout << "All vertices: "<< _allVertices.size() << std::endl;
    std::cout << "UV Indexs: "<< _uvIndexs.size() << std::endl;
    std::cout << "Normals Indexs: "<< _normalIndexs.size() << std::endl;

    for (int i = 0; i < _allVertices.size(); ++i)
    {
        _all.push_back(_allVertices[i]);
        if (i < _uvIndexs.size())
        {
            _all.push_back(_uvs[_uvIndexs[i]]);
        }
        // else
        // {
        //     _all.push_back({0,0,0});
        // }
        if (i < _normalIndexs.size())
        {
            _all.push_back(_normals[i]);
        }
    }

    std::cout << "All size: " << _all.size() << std::endl;
}

void Mesh::InitArray()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _all.size() * sizeof(glm::vec3), _all.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);
    // glGenBuffers(1, &_EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, _verticeIndex.size() * sizeof(unsigned int), _verticeIndex.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nbChannels;
    unsigned char* data = stbi_load(_texturePath, &width, &height, &nbChannels, 0);

    if (data)
    {
        std::cout << nbChannels << std::endl;
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (nbChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        if (nbChannels == 4 )
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load ressources !" << std::endl;
    }
    stbi_image_free(data);

}

void Mesh::Draw()
{

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _all.size()/3);
    glBindVertexArray(0);
}
