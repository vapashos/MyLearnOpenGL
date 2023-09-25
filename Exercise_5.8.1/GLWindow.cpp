#include "GLWindow.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLWindow::GLWindow(const int WIDTH, const int HEIGHT, const char* title)
    :_width(WIDTH), _height(HEIGHT), _title(title)
{
    pInitGLFWOpenGLCore(OPENGL_VERSION);
    _window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

const GLFWwindow* GLWindow::GetWindowConst() const
{
    return _window;
}


GLFWwindow* GLWindow::GetWindow() const
{
    return _window;
}

void GLWindow::Run(const ShaderProgram& program)
{
    if (_window)
    {
        while (!glfwWindowShouldClose(_window))
        {
            pProcessInput();
            
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
        program.Clear();
        glfwTerminate();
    }
}

void GLWindow::pProcessInput()
{
    if (_window)
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);
    }
}

void GLWindow::pInitGLFWOpenGLCore(float versionOpenGL) const
{
    int versionMajor = (int)(OPENGL_VERSION / 1);
    int versionMinor = (((int)(OPENGL_VERSION * 10.0f)) % 10);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);       //Set OpenGL Version Major
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);       //Set OpenGL Version Minor
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


