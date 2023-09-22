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

VertexShader::VertexShader(const GLenum glEnumTarget, const GLenum glUsage)
    : Shader("#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0", GL_VERTEX_SHADER), _glTarget(glEnumTarget), _glUsage(glUsage)
{
    _id_vbo = ++sVertexShaderID;
    pCreate();
}

VertexShader::~VertexShader()
{
    if (sVertexShaderID > 0)
        sVertexShaderID--;
    else
        assert(0);
}

unsigned int VertexShader::VAO() const
{
    return _VAO;
}

void VertexShader::BindVertices(const float* vertices,const int numofvertices)
{
#if 0
    const unsigned int _id_vao = _id_vbo;
    glGenBuffers(_id_vbo, &_VBO);
    //// VAO Generate and Bind
    glGenVertexArrays(_id_vao, &_VAO);
    glBindVertexArray(_VAO);
     
    // VBO Generate and Bind
    glBindBuffer(_glTarget, _VBO);
    glBufferData(_glTarget, sizeof(vertices), vertices, _glUsage);

    // Number of vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif
    unsigned int id_vbo = 1;
    unsigned int id_vao = 1;
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(id_vao, &VAO);
    glBindVertexArray(VAO);
    // --------------------------------------------------------------------------
    glGenBuffers(id_vbo, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // --------------------------------------------------------------------------
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _VBO = VBO;
    _VAO = VAO;
}

void VertexShader::Draw() const
{
    glBindVertexArray(_VAO);
    glDrawArrays(Mode(), 0, VerticesNum());
    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------------------------------------------
// TriaVertexShader 
// ---------------------------------------------------------------------------------------------------------------

TriaVertexShader::TriaVertexShader(const GLenum glEnumTarget, const GLenum glUsage)
    :VertexShader(glEnumTarget, glUsage)
{
}

GLenum TriaVertexShader::Mode() const
{
    return GL_TRIANGLES;
}

GLsizei TriaVertexShader::VerticesNum() const
{
    return 3;
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

ShaderProgram::ShaderProgram(const GLenum vShaderTarget, const GLenum vShaderUsage,const float* vertices, int numofvertices)
    : _id(0)
{
    _vShader = std::make_unique<TriaVertexShader>(vShaderTarget, vShaderUsage);
    _fShader = std::make_unique<FragmentShader>();

    _id = glCreateProgram();

    if (_vShader->IsOk() && _fShader->IsOk())
    {
        glAttachShader(_id, _vShader->GetID());  // Attach Vertex   Shader.
        glAttachShader(_id, _fShader->GetID());  // Attach Fragment Shader.
        _status = pLink();
        _status &= pValidate();
        //glDeleteShader(_vShader->GetID());
        //glDeleteShader(_fShader->GetID());
    }
    //_vShader->BindVertices(vertices, numofvertices);
}

void ShaderProgram::Draw() const
{
    if (_status)
        _vShader->Draw();
}

unsigned int ShaderProgram::GetID() const
{
    return _id;
}

void ShaderProgram::Clear() const
{
    unsigned int VAO,VBO;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(_id);
}

void ShaderProgram::Use() const
{
    if(_status)
        glUseProgram(_id);
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