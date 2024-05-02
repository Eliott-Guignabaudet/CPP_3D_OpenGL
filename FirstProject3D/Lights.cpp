#include "Lights.h"

#include <glad/glad.h>

Lights::Lights() : _VBO(), _VAO(),
                   _vertices{
                    -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                    0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    0.5f,  0.5f, -0.5f, 1.f, 1.f,
                    -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                    0.5f,  0.5f, -0.5f, 1.f, 1.f,
                    -0.5f,  0.5f, -0.5f, 0.f, 1.f,
        
                    -0.5f,  0.5f, -0.5f, 0.f, 0.f,
                    0.5f,  0.5f, -0.5f, 1.f, 0.f,
                    0.5f,  0.5f,  0.5f, 1.f, 1.f,
                    -0.5f,  0.5f, -0.5f, 0.f, 0.f,
                    -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                    0.5f,  0.5f,  0.5f, 1.f, 1.f,
        
                    -0.5f, -0.5f,  0.5f, 0.f, 0.f,
                    0.5f, -0.5f,  0.5f, 1.f, 0.f,
                    0.5f,  0.5f,  0.5f, 1.f, 1.f,
                    -0.5f, -0.5f,  0.5f, 0.f, 0.f,
                    0.5f,  0.5f,  0.5f, 1.f, 1.f,
                    -0.5f,  0.5f,  0.5f, 0.f, 1.f,
        
                    -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                    0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    0.5f, -0.5f,  0.5f, 1.f, 1.f,
                    -0.5f, -0.5f, -0.5f, 0.f, 0.f,
                    -0.5f, -0.5f,  0.5f, 0.f, 1.f,
                    0.5f, -0.5f,  0.5f, 1.f, 1.f,
        
                    -0.5f,  0.5f, -0.5f, 1.f, 1.f,
                    -0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                    -0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    -0.5f,  0.5f,  0.5f, 0.f, 1.f,
                    -0.5f, -0.5f,  0.5f, 0.f, 0.f,
        
                    0.5f,  0.5f, -0.5f, 1.f, 1.f,
                    0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    0.5f,  0.5f,  0.5f, 0.f, 1.f,
                    0.5f, -0.5f, -0.5f, 1.f, 0.f,
                    0.5f,  0.5f,  0.5f, 0.f, 1.f,
                    0.5f, -0.5f,  0.5f, 0.f, 0.f,
                   }
{
}

Lights::~Lights()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Lights::Init()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lights::Draw()
{
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
