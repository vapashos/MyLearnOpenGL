// Exercise_5.8.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "GLWindow.h"

int main()
{
    try {
        GLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        float vertices_tria[] = {
        -1.0f, -0.5f, 0.0f,  // triangle 1 left  
         0.0f, -0.5f, 0.0f,  // triangle 1 right 
         -0.5f,  0.5f, 0.0f, // triangle 1 top           
         0.0f, -0.5f, 0.0f,  // triangle 2 left
         1.0,-0.5f, 0.0f,
         0.5f,0.5f,0.0f
        };

        const int verticesNum = sizeof(vertices_tria)/sizeof(vertices_tria[0]);

        VertexShader::Definition triaDefinition{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria,
                                                 verticesNum,
                                                 GL_TRIANGLES};


        ShaderProgram sprogram(triaDefinition);

        glWindow.Run(sprogram);
    }
    catch (...)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

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
