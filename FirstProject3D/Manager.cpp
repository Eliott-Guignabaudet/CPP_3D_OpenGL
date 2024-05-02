
#include <iostream>

#include "Manager.h"

#include <fstream>
#include <sstream>

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
glm::vec2 Manager::_lastMousePosition;
float Manager::XOffset = 0.f;
float Manager::YOffset = 0.f;
bool Manager::_isInit = false;
int Manager::WindowWidth = 800;
int Manager::WindowHeight = 600;
Manager::Manager() : _window(), Pitch(0), Yaw(0)
{
    _square = new Square();
}

Manager::~Manager()
{
    delete _square;
    delete _light;
    glDeleteProgram(_shader->ID);
    glDeleteProgram(_shaderLight->ID);
}

void Manager::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
    WindowWidth = width;
    WindowHeight = height;
}

void Manager::MouseMoveCallback(GLFWwindow* window, double x, double y)
{
    if (!_isInit)
    {
        _lastMousePosition = glm::vec2(x, y);
        _isInit = true;
        return;
    }
    XOffset = x - _lastMousePosition.x;
    YOffset = y - _lastMousePosition.y;
    _lastMousePosition = glm::vec2(x, y);
}

void Manager::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    std::cout << '(' << xOffset << ", " << yOffset <<")" << std::endl;
}

void Manager::InitWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    _window = glfwCreateWindow(WindowWidth, WindowHeight, "FirstProject3D",NULL,NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);
    glfwSwapInterval(1);
    
}

void Manager::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }
}

void Manager::InitShaders()
{
    _shader = new Shader("Assets/Shaders/VertexShader.shader", "Assets/Shaders/FragmentShader.shader");
    _shaderLight = new Shader("Assets/Lights/Shaders/VertexShader.shader", "Assets/Lights/Shaders/FragmentShader.shader");
}

void Manager::InitObjects()
{
    _square->Init();

    Mesh* nGone = new Mesh("Assets/Mesh/12221_Cat_v1_l3.obj", "Assets/Textures/Cat_diffuse.jpg", 0.1f);
    _meshs.insert({"Cat", nGone});
    // Mesh* cow = new Mesh("Assets/Mesh/cow-nonormals.obj");
    // _meshs.insert({"Cow", cow});
    // Mesh* pumpkin = new Mesh("Assets/Mesh/pumpkin_tall_10k.obj", 0.1f);
    // _meshs.insert({"Pumpkin", pumpkin});
    // Mesh* teapot = new Mesh("Assets/Mesh/teapot.obj");
    // _meshs.insert({"Teapot", pumpkin});
    // Mesh* teddy = new Mesh("Assets/Mesh/teddy.obj");
    // _meshs.insert({"Teddy", pumpkin});
    _light = new Lights();
    _light->Init();
    _lightPosition = glm::vec3(2,8,50);
}

void Manager::Update()
{
    _cubeRotation += _deltaTime * 0;
    
    Yaw+= XOffset * _deltaTime * 10;
    Pitch+= YOffset * _deltaTime * 10;
    glm::mat4 transform = (1.f);
    transform = glm::rotate(transform, glm::radians(-Yaw), _worldUp);
    _cameraFront = transform * glm::vec4(_worldFront, 1.0f);
    transform = glm::rotate( transform, glm::radians(-Pitch), glm::normalize(glm::cross(_cameraFront, _worldUp)));
    _cameraFront = transform * glm::vec4(_cameraFront, 1.0f);

    //_lightPosition.z += 1.f * _deltaTime;
}

void Manager::Draw()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _shader->SetInt("Texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _square->Texture);
    glUseProgram(_shader->ID);
    _shader->SetVector3("lightColor", 1.0f, 1.0f, 1.0f);
    _shader->SetVector3("objectColor", 1.0f, 1.0f, 1.0f);
    _shader->SetFloat("ambiantStrength", 1.f);
    _shader->SetVector3("lightPos", _lightPosition );
    _shader->SetVector3("viewPos", _cameraPos);
    
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view  = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
    glm::mat4 model = glm::mat4(1.0f);
    
    projection = glm::perspective(glm::radians(45.0f), (float)WindowWidth/(float)WindowHeight, 0.1f, 1000.0f);
    model = glm::translate(model, glm::vec3(0.f,-1.f,0.f));
    model = glm::rotate(model, glm::radians(static_cast<float>(_cubeRotation)), glm::cross(_cameraFront, _cameraRight));

    _shader->SetMatrix4("projection", projection);
    _shader->SetMatrix4("view", view);
    _shader->SetMatrix4("model", model);
    _square->Draw();

    model = glm::translate(model, glm::vec3(0.f,2.f,0.f));
    model = glm::rotate(model, glm::radians(static_cast<float>(_cubeRotation)), glm::vec3(0,1,0));
    _shader->SetMatrix4("model", model);
    _square->Draw();
    
    model = glm::translate(model, glm::vec3(0.f, 0.f, 20.f));
    // _shader->SetMatrix4("model", model);
    // _meshs["Cow"]->Draw();
    model = glm::translate(model, glm::vec3(0.f, 0.f, 20.f));
    model = glm::rotate(model, glm::radians(-90.f),glm::vec3(1,0,0));
    _shader->SetMatrix4("model", model);

    glBindTexture(GL_TEXTURE_2D, _meshs["Cat"]->Texture);
    _meshs["Cat"]->Draw();
    // for (int i = 0; i < 20; ++i)
    // {
    //     model = glm::translate(model, glm::vec3(0.f, 0.f, 20.f));
    //     _shader->SetMatrix4("model", model);
    //     _meshs["Cow"]->Draw();
    // }

    
    XOffset = 0;
    YOffset = 0;

    glUseProgram(_shaderLight->ID);
    _shaderLight->SetMatrix4("projection", projection);
    _shaderLight->SetMatrix4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, _lightPosition);
    model = glm::scale(model, glm::vec3(0.2f,0.2f,0.2f));
    _shaderLight->SetMatrix4("model", model);

    _light->Draw();
}

void Manager::HandleInput()
{
}



void Manager::Run()
{
    glfwInit();
    InitWindow();
    InitGlad();
    glEnable(GL_DEPTH_TEST);
    InitShaders();
    InitObjects();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


    _worldFront = glm::vec3(0.f,0.f,1.f);
    _worldUp = glm::vec3(0.f,1.f,0.f);
    _worldRight =  glm::vec3(1.f,0.f,0.f);
    
    _cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
    _cameraFront = _worldFront;
    _cameraUp = _worldUp;
    _cameraRight = _worldRight;

    _cameraSpeed = 10.f;

    glfwSetCursorPosCallback(_window, MouseMoveCallback);
    glfwSetScrollCallback(_window, MouseScrollCallback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    while (!glfwWindowShouldClose(_window))
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
        }
        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        {
            _cameraPos += _cameraFront * _deltaTime * _cameraSpeed;
        }
        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        {
            _cameraPos -= _cameraFront * _deltaTime * _cameraSpeed;
        }
        if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            _cameraPos += _cameraUp * _deltaTime * _cameraSpeed;
        }
        if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            _cameraPos -= _cameraUp * _deltaTime * _cameraSpeed;
        }
        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        {
            _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _deltaTime * _cameraSpeed;
        }
        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        {
            _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * _deltaTime * _cameraSpeed;
        }
        _deltaTime = (float)(glfwGetTime() - _lastTime);
        _lastTime = glfwGetTime();
        Update();
        Draw();
        std::string title ="FisrtProject3D ";
        
        title.append(std::to_string((int)round(1/_deltaTime)));
        glfwSetWindowTitle(_window, title.data() );
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
