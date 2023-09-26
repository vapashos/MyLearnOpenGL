#include "Shaders.h"
#include <cassert>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------
// Shader 
// ---------------------------------------------------------------------------------------------------------------

Shader::Shader(const char* shaderStr, GLenum type):
    _shaderStr(shaderStr),_type(type)
{
}

unsigned int Shader::GetID() const
{
    return _id;
}

bool Shader::IsOk() const
{
    return _status;
}

void Shader::pCreate()
{
    // Create the shader
    _id = glCreateShader(_type);

    // Attach shader's source code to the shader object and compile the shader
    glShaderSource(_id, 1, &_shaderStr, NULL);

    // At the end compile shader's code.
    _status = pCompile();
}

bool Shader::pCompile() const
{
    // Compile shader
    glCompileShader(_id);

    int success;
    char infoLog[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        assert(success);
        glGetShaderInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------
// VertexShader 
// ---------------------------------------------------------------------------------------------------------------
int VertexShader::sVertexShaderID = 0;

VertexShader::VertexShader(const GLenum glEnumTarget, const GLenum glUsage, const float* vertices,const int verticesNum)
    : Shader("#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0", GL_VERTEX_SHADER), _glTarget(glEnumTarget), _glUsage(glUsage), _verticesNum(verticesNum)
{
    _id_vbo = ++sVertexShaderID;
    pCreate();
    pBindVertices(vertices, verticesNum);
    
}

VertexShader::~VertexShader()
{
    if (sVertexShaderID > 0)
        sVertexShaderID--;
    else
        assert(0);
    
    glDeleteVertexArrays(_id, &_VAO);
    glDeleteBuffers(_id, &_VBO);
}

unsigned int VertexShader::VAO() const
{
    return _VAO;
}

void VertexShader::pBindVertices(const float* vertices, const int verticesNum)
{
    const unsigned int _id_vao = _id_vbo;

    glGenVertexArrays(_id_vao, &_VAO);
    glGenBuffers(_id_vbo, &_VBO);
    //// VAO Generate and Bind
    glBindVertexArray(_VAO);
     
    // VBO Generate and Bind
    glBindBuffer(_glTarget, _VBO);
    glBufferData(_glTarget, verticesNum*sizeof(vertices[0]), vertices, _glUsage);

    // Number of vertices
    const GLuint location_index = 0;
    glVertexAttribPointer(location_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(_glTarget, 0);
    glBindVertexArray(0);

}

void VertexShader::Draw(const GLenum mode) const
{
    glBindVertexArray(_VAO);
    glDrawArrays(mode, 0, _verticesNum);
    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------------------------------------------
// FragmentShader 
// ---------------------------------------------------------------------------------------------------------------
int FragmentShader::sFragmentShaderID = 0;

FragmentShader::FragmentShader()
    : Shader("#version 330 core\n"
             "out vec4 FragColor;\n"
             "void main()\n"
             "{\n"
             "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
             "}\n\0", GL_FRAGMENT_SHADER)
{
    _id_vbo = ++sFragmentShaderID;
    pCreate();
}

FragmentShader::~FragmentShader()
{
    if (sFragmentShaderID > 0)
        sFragmentShaderID--;
    else
        assert(0);
}

ShaderProgram::ShaderProgram(const VertexShader::Definition& vDefinition)
    : _id(0), _vShaderPtr(new VertexShader(vDefinition.TARGET, vDefinition.USAGE, vDefinition.VERTICES, vDefinition.VERTICES_NUM)),
      _fShaderPtr(new FragmentShader()), _drawMode(vDefinition.DRAW_MODE)
{
    _id = glCreateProgram();

    const unsigned int vShaderID = _vShaderPtr->GetID();
    const unsigned int fShaderID = _fShaderPtr->GetID();

    if (fShaderID * vShaderID)
    {
        glAttachShader(_id, vShaderID);  // Attach Vertex   Shader.
        glAttachShader(_id, fShaderID);  // Attach Fragment Shader.
        _status = pLink();
        _status &= pValidate();
    }
}

ShaderProgram::~ShaderProgram()
{
    delete _vShaderPtr;
    delete _fShaderPtr;
    glDeleteProgram(_id);
}

unsigned int ShaderProgram::GetID() const
{
    return _id;
}

void ShaderProgram::Reset()
{
    if (_status)
    {
        delete _vShaderPtr;
        delete _fShaderPtr;

        _vShaderPtr = nullptr;
        _fShaderPtr = nullptr;
    }
    _status = false;
    glDeleteProgram(_id);
}

void ShaderProgram::Execute() const
{
    if (_vShaderPtr)
    {
        glUseProgram(_id);
        _vShaderPtr->Draw(_drawMode);
    }
}

bool ShaderProgram::pLink() const
{
    glLinkProgram(_id);                     // Link shader program
    int success;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPOGRAM::LINKAGE_FAILED\n" << infoLog << std::endl;
        assert(0);
    }
    return success;
}

bool ShaderProgram::pValidate() const
{
    GLint result;
    GLchar eLog[1024] = { 0 };

    glValidateProgram(_id);
    glGetProgramiv(_id, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(_id, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return false;
    }
    return true;
}
