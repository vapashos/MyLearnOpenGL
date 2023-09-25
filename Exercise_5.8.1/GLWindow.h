#pragma once
#include "Shaders.h"

const float OPENGL_VERSION  = 3.3f;
const int   WINDOW_WIDTH    = 800;
const int   WINDOW_HEIGHT   = 600;

class GLWindow
{
public:
    GLWindow(const int WIDTH, const int HEIGHT, const char* title = "LearnOpenGL");
    ~GLWindow() = default;

    const GLFWwindow* GetWindowConst() const;
    GLFWwindow* GetWindow() const;
    void Run(ShaderProgram& program);

private:

    const int   _width;
    const int   _height;
    const char* _title;
    GLFWwindow* _window{ nullptr };

    void pProcessInput();
    void pInitGLFWOpenGLCore(float versionOpenGL) const;

};