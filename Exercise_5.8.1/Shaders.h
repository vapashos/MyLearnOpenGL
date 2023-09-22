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
    bool         _status       { false };       // status of compilation
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
    // Constructor - Destructor
    VertexShader(const GLenum glEnumTarget,const GLenum glUsage);
    virtual ~VertexShader();

    // virtual functions
    virtual GLenum  Mode()          const = 0;
    virtual GLsizei VerticesNum()  const = 0;
    
    // function
    unsigned int    VAO() const;
    void            BindVertices(const float* vertices, const int numofvertices);
    void            Draw() const;
    
    
    // Static members
    static int sVertexShaderID;
    
protected:
    // Protected member variables
    unsigned int _glTarget;
    unsigned int _glUsage;      // GL_STREAM_DRAW , GL_STATIC_DRAW, GL_DYNAMIC_DRAW
    unsigned int _VAO{0};       // Vertex Array Object
    // Protected member functions
    

};

class TriaVertexShader : public VertexShader
{
public:
    TriaVertexShader(const GLenum glEnumTarget, const GLenum glUsage);
    virtual ~TriaVertexShader() = default;

    virtual GLenum  Mode()        const override;       // return GL_TRIANGLES
    virtual GLsizei VerticesNum() const override;       // return GL_TRIANGLES
    
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
    ShaderProgram(const GLenum vShaderTarget, const GLenum vShaderUsage,const float* vertices, int numofvertices);
    void         Draw()  const;
    unsigned int GetID() const;
    void         Clear() const;
    void         Use()   const;
    const std::unique_ptr<VertexShader>& getVertexShader() const { return _vShader; };
    const std::unique_ptr<FragmentShader>& getFragmentShader() const { return _fShader; };
private:
    
    bool         _status{ false };
    unsigned int _id;
    
    std::unique_ptr<VertexShader>   _vShader;
    std::unique_ptr<FragmentShader> _fShader;

    bool pLink()        const;
    bool pValidate()    const;
};

