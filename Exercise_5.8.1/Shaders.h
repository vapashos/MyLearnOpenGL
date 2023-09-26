#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
// ---------------------------------------------------------------------------------------------------------------
// FragmentShader 
// ---------------------------------------------------------------------------------------------------------------
class Shader
{
public:
    Shader(const char* shaderStr, GLenum type);
    unsigned int    GetID() const;
    bool            IsOk()  const;
protected:
    // Member variables
    GLenum       _type;                         // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER 
    const char*  _shaderStr;
    int          _id_vbo        { 0 };          // id of vbo which correspond to the shader's data.
    unsigned int _VBO           { 0 };
    unsigned int _id            { 0 };          // shader's is
    bool         _status        { false };      // status of compilation
    // Member functions 
    void pCreate();
    bool pCompile() const;
    
};

// --------------------------------------------------------------------------------------
// VertexShader 
// --------------------------------------------------------------------------------------
class VertexShader : public Shader
{
public:

    struct Definition
    {
        unsigned int TARGET;
        unsigned int USAGE;
        const float* VERTICES;
        const int    VERTICES_NUM;
        const GLenum DRAW_MODE;
    };

    // Constructor - Destructor
    VertexShader(const GLenum glEnumTarget,const GLenum glUsage, const float* vertices, const int verticesNum);
    ~VertexShader();

    // function
    unsigned int    VAO()                   const;
    void            Draw(const GLenum mode) const;
    
    // Static members
    static int sVertexShaderID;
    
protected:
    // Protected member variables
    unsigned int _glTarget;
    unsigned int _glUsage;          // GL_STREAM_DRAW , GL_STATIC_DRAW, GL_DYNAMIC_DRAW
    unsigned int _VAO{0};           // Vertex Array Object
    int          _verticesNum{ 0 }; // Number of vertices to draw
    // Protected member functions
    void pBindVertices(const float* vertices, const int verticesNum);

};

class FragmentShader : public Shader
{
public:
    // Constructor - Destructor
    FragmentShader();
    ~FragmentShader();

    // Static members
    static int sFragmentShaderID;

protected:
};

class ShaderProgram
{
public:
    ShaderProgram(const VertexShader::Definition& vDefinition);
    ~ShaderProgram();
    unsigned int GetID()     const;
    void         Execute()   const;
    void         Reset();

private:
    VertexShader*   _vShaderPtr;
    FragmentShader* _fShaderPtr;
    bool            _status{ false };
    unsigned int    _id;
    GLenum          _drawMode;
    
    bool pLink()        const;
    bool pValidate()    const;
};

