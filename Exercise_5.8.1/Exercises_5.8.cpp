// Exercise_5.8.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "GLWindow.h"


void RunExercise1()
{
    std::cout << " RUNNING EXERCISE 5.8.1 DRAW 2 TRIANGLES USING THE SAME VAO AND VBO" << std::endl;
    try {
        GLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        float vertices_tria[] = {
        -1.0f, -0.5f, 0.0f,  // triangle 1 left  
         0.0f, -0.5f, 0.0f,  // triangle 1 right 
         -0.5f,  0.5f, 0.0f, // triangle 1 top           
         0.0f, -0.5f, 0.0f,  // triangle 2 left
         1.0, -0.5f, 0.0f,   // triangle 2 right
         0.5f, 0.5f, 0.0f    // triangle 2 top
        };

        const int verticesNum = sizeof(vertices_tria) / sizeof(vertices_tria[0]);

        VertexShader::Definition triaDefinition{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria,
                                                 verticesNum,
                                                 GL_TRIANGLES };

        FragmentShader::Definition orange({ 1.0f, 0.5f, 0.0f });
        ShaderProgram sprogram(triaDefinition, orange);
        glWindow.Run({ &sprogram });
    }
    catch (...)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}



void RunExercise2()
{
    std::cout << " RUNNING EXERCISE 5.8.2 DRAW 2 TRIANGLES USING THE DIFFERENT VAO AND VBO" << std::endl;
    try
    {
        GLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        float vertices_tria[] = {
        -1.0f, -0.5f, 0.0f,  // triangle 1 left  
         0.0f, -0.5f, 0.0f,  // triangle 1 right 
         -0.5f,  0.5f, 0.0f, // triangle 1 top           

        };

        const int verticesNum = sizeof(vertices_tria) / sizeof(vertices_tria[0]);

        VertexShader::Definition triaDefinition{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria,
                                                 verticesNum,
                                                 GL_TRIANGLES };

        FragmentShader::Definition orange({ 1.0f, 0.5f, 0.0f });
        float vertices_tria2[] = {
            0.0f, -0.5f, 0.0f,  // triangle 2 left
            1.0, -0.5f, 0.0f,   // triangle 2 right
            0.5f, 0.5f, 0.0f    // triangle 2 top
        };

        const int verticesNum2 = sizeof(vertices_tria2) / sizeof(vertices_tria2[0]);
        VertexShader::Definition triaDefinition2{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria2,
                                                 verticesNum2,
                                                 GL_TRIANGLES };

        ShaderProgram sprogram(triaDefinition, orange);
        sprogram.AddVertexShader(triaDefinition2);

        glWindow.Run({ &sprogram});
    }
    catch (...)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}


void RunExercise3()
{
    std::cout << " RUNNING EXERCISE 5.8.3 DRAW 2 TRIANGLES USING THE DIFFERENT VAO AND VBO AND DIFFERENT SHARED PROGRAMMS" << std::endl;

    try {
        GLWindow glWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        float vertices_tria[] = {
        -1.0f, -0.5f, 0.0f,  // triangle 1 left  
         0.0f, -0.5f, 0.0f,  // triangle 1 right 
         -0.5f,  0.5f, 0.0f, // triangle 1 top           

        };

        const int verticesNum = sizeof(vertices_tria) / sizeof(vertices_tria[0]);

        VertexShader::Definition triaDefinition{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria,
                                                 verticesNum,
                                                 GL_TRIANGLES };
        // ORANGE TRIA DEFINITION
        FragmentShader::Definition orange({ 1.0f, 0.5f, 0.0f });
        ShaderProgram sprogram(triaDefinition, orange);
        float vertices_tria2[] = {
            0.0f, -0.5f, 0.0f,  // triangle 2 left
            1.0, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f
        };

        // YELLOW TRIA DEFINITION
        const int verticesNum2 = sizeof(vertices_tria2) / sizeof(vertices_tria2[0]);
        FragmentShader::Definition yellow(1.0f, 1.0f, 0.0f);
        VertexShader::Definition triaDefinition2{ GL_ARRAY_BUFFER,
                                                 GL_STATIC_DRAW,
                                                 vertices_tria2,
                                                 verticesNum2,
                                                 GL_TRIANGLES };

        ShaderProgram sprogram2(triaDefinition2, yellow);

        glWindow.Run({ &sprogram,&sprogram2 });
    }
    catch (...)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

int main()
{
    std::cout << "Select the program id to run (1-3):";
    unsigned int programID;
    std::cin >> programID;
    switch (programID)
    {
        case 1:
            RunExercise1();
        break;
        case 2:
            RunExercise2();
        break;
        case 3:
            RunExercise3();
        break;
        default:
            break;
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
