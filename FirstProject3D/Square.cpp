#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Square.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/geometric.hpp>

Square::Square() : _VBO(), _VAO(),
                   _vertices{
                       -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                        0.5f, -0.5f, -0.5f, 1.f, 0.f,
                        0.5f,  0.5f, -0.5f, 1.f, 1.f,
                       -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                        0.5f,  0.5f, -0.5f, 1.f, 1.f,
                       -0.5f,  0.5f, -0.5f, 0.f, 1.f,
        
                        0.5f,  0.5f, -0.5f, 1.f, 0.f,
                       -0.5f,  0.5f, -0.5f, 0.f, 0.f,
                        0.5f,  0.5f,  0.5f, 1.f, 1.f,
                       -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                        0.5f,  0.5f,  0.5f, 1.f, 1.f,
                       -0.5f,  0.5f, -0.5f, 0.f, 0.f,
        
                       -0.5f, -0.5f,  0.5f, 0.f, 0.f,
                        0.5f, -0.5f,  0.5f, 1.f, 0.f,
                        0.5f,  0.5f,  0.5f, 1.f, 1.f,
                       -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                       -0.5f, -0.5f,  0.5f, 0.f, 0.f,
                        0.5f,  0.5f,  0.5f, 1.f, 1.f,
        
                        0.5f, -0.5f, -0.5f, 1.f, 0.f,
                       -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                        0.5f, -0.5f,  0.5f, 1.f, 1.f,
                       -0.5f, -0.5f,  0.5f, 0.f, 1.f,
                        0.5f, -0.5f,  0.5f, 1.f, 1.f,
                       -0.5f, -0.5f, -0.5f, 0.f, 0.f,
        
                       -0.5f,  0.5f, -0.5f, 1.f, 1.f,
                       -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                       -0.5f, -0.5f, -0.5f, 1.f, 0.f,
                       -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                       -0.5f, -0.5f,  0.5f, 0.f, 0.f,
                       -0.5f, -0.5f, -0.5f, 1.f, 0.f,
        
                       0.5f,  0.5f, -0.5f, 1.f, 1.f,
                       0.5f,  0.5f,  0.5f, 0.f, 1.f,
                       0.5f, -0.5f, -0.5f, 1.f, 0.f,
                       0.5f,  0.5f,  0.5f, 0.f, 1.f,
                       0.5f, -0.5f,  0.5f, 0.f, 0.f,
                       0.5f, -0.5f, -0.5f, 1.f, 0.f,
                   }
{
    
}

Square::~Square()
{
    glDeleteVertexArrays(1,&_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Square::GenerateAll()
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> uvs;
    std::vector<glm::vec3> normals;

    for (int i = 0; i < sizeof(_vertices) / sizeof(float); i += 5)
    {
        glm::vec3 vertice = glm::vec3(_vertices[i],_vertices[i+1],_vertices[i+2]);
        glm::vec3 uv = glm::vec3(_vertices[i + 3],_vertices[i + 4], 0 );

        vertices.push_back(vertice);
        uvs.push_back(uv);
    }

    for (int i = 0; i < vertices.size();i += 3)
    {
        glm::vec3 p1 = vertices[i];
        glm::vec3 p2 = vertices[i + 1];
        glm::vec3 p3 = vertices[i + 2];

        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;

        glm::vec3 normal =  glm::cross(u, v);

        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }

    for (int i = 0; i < vertices.size(); ++i)
    {
        _all.push_back(vertices[i]);
        _all.push_back(uvs[i]);
        _all.push_back(normals[i]);
    }
    
}

void Square::Init()
{
    GenerateAll();

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _all.size() * sizeof(glm::vec3), _all.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nbChannels;
    unsigned char* data = stbi_load("Assets/Textures/RoofTiles_basecolor.png", &width, &height, &nbChannels, 0);

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
        std::cout << "Test2" << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load ressources !" << std::endl;
    }
    stbi_image_free(data);
}

void Square::Draw()
{
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
