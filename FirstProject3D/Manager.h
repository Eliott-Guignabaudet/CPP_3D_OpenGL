#pragma once
#include <map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Lights.h"
#include "Mesh.h"
#include "Square.h"

class Shader;

class Manager
{
private:
    GLFWwindow* _window;
    Square* _square;
    Shader* _shader;
    Shader* _shaderLight;
    double _lastTime;
    float _deltaTime;
    double _cubeRotation;

    glm::vec3 _worldFront;
    glm::vec3 _worldUp;
    glm::vec3 _worldRight;

    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    glm::vec3 _cameraRight;
    glm::vec3 _lightPosition;
    float _cameraSpeed;

    static bool _isInit;

    std::map<std::string,Mesh*> _meshs;

    Lights* _light;
    
    void InitWindow();
    void InitGlad();
    void InitShaders();
    void InitObjects();
    void Update();
    void Draw();
    void HandleInput();
    
public:
    Manager();
    ~Manager();
    void Run();
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseMoveCallback(GLFWwindow* window, double x, double y);
    static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static glm::vec2 _lastMousePosition;
    float Yaw, Pitch;
    static float XOffset, YOffset;
    static int WindowWidth, WindowHeight;
};
