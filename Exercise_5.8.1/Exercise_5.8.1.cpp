// Exercise_5.8.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "GLWindow.h"

int main()
{
    float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.0f,  0.5f, 0.0f  // top   
    };


    GLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    GladInitiator gInit;
    auto _window = glWindow.GetWindow();
    
    TriaVertexShader triaShader(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices);
    FragmentShader   fragShader;

    ShaderProgram sprogram(triaShader.GetID(),fragShader.GetID());


    // Render loop
    while (!glfwWindowShouldClose(_window))
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(_window, true);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprogram.Use();
        glBindVertexArray(triaShader.VAO());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    glfwTerminate();

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
