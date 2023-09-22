// Exercise_5.8.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Shaders.h"

const float OPENGL_VERSION  = 3.3f;
const int WINDOW_WIDTH      = 800;
const int WINDOW_HEIGHT     = 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void InitGLFWOpenGLCore(float versionOpenGL)
{
    int versionMajor = (int)(OPENGL_VERSION / 1);
    int versionMinor = (((int)(OPENGL_VERSION * 10.0f)) % 10);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);       //Set OpenGL Version Major
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);       //Set OpenGL Version Minor
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

class GladInitiator
{
public:
    GladInitiator()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw - 1;
        }
    }
};

class OpenGLWindow
{
public:
    OpenGLWindow(const int WIDTH, const int HEIGHT, const char* title = "LearnOpenGL")
        :_width(WIDTH), _height(HEIGHT), _title(title)
    {
        InitGLFWOpenGLCore(OPENGL_VERSION);
        _window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
        if (_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    }

    ~OpenGLWindow() = default;


    bool IsValid() const
    {
        return _window;
    }

    GLFWwindow* Get()
    {
        return _window;
    }

    void Run(const ShaderProgram& program)
    {
        if (_window)
        {
            while (!glfwWindowShouldClose(_window))
            {
                pProcessInput();
                program.Draw();
                glfwSwapBuffers(_window);
                glfwPollEvents();
            }
            program.Clear();
            glfwTerminate();
        }
    }

private:

    const int   _width;
    const int   _height;
    const char* _title;
    GLFWwindow* _window{ nullptr };

    void pProcessInput()
    {
        if (_window)
        {
            if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(_window, true);
        }
    }

};


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void sBindVertices(const float* vertices,unsigned int& vao)
{
    unsigned int VBO;
    unsigned int id_vbo = 1;
    unsigned int id_vao = 1;
    glGenBuffers(id_vbo, &VBO);
    // VAO Generate and Bind
    glGenVertexArrays(id_vao, &vao);
    glBindVertexArray(vao);
    // VBO Generate and Bind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BufferData(const float* vertices)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main()
{
    float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.0f,  0.5f, 0.0f  // top   
    };


#if 0
    try
    {
        OpenGLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        GladInitiator gInit;
        ShaderProgram shader(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices) / sizeof(vertices[0]));
        glWindow.Run(shader);
    }
    catch (...)
    {
        std::cout << " could not load GLAD library" << std::endl;
    }

#else
    OpenGLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    GladInitiator gInit;
    auto _window = glWindow.Get();
    // -----------------------------
    // VertexShader 
    // -----------------------------
    // Create Vertex Shader
#if 0
    auto vShaderID = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader's source code to the shader object and compile the shader
    glShaderSource(vShaderID, 1, &vertexShaderSource, NULL);

    // At the end compile shader's code.
    glCompileShader(vShaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        //assert(success);
        glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
#endif
    /*auto _vShader = std::make_unique<TriaVertexShader>(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER);
    unsigned int vShaderID = _vShader->GetID();*/
    // --------------------------------
    // FragmentShader 
    // --------------------------------

    /*auto _fShader = std::make_unique<FragmentShader>();
    auto fShaderID = _fShader->GetID();*/


    // --------------------------------
    // Shader Program
    // --------------------------------
    
    ShaderProgram sprogram(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices) / sizeof(vertices[0]));

    // Render loop
    while (!glfwWindowShouldClose(_window))
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprogram.Use();
        sprogram.Draw();
        glUseProgram(0);
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(sprogram.GetID());*/
    glfwTerminate();


#endif
    //system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
