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
    _shaderString = _shaderStr.c_str();
    glShaderSource(_id, 1, &_shaderString, NULL);

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
    {
        sVertexShaderID--;

        glDeleteVertexArrays(_id, &_VAO);
        glDeleteBuffers(_id, &_VBO);
    }
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

FragmentShader::FragmentShader(const Definition& color)
    : Shader("#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor =",GL_FRAGMENT_SHADER)
{
    _shaderStr.append(" vec4(");
    _shaderStr.append(std::to_string(color.RED).substr(0, 3)); // RED
    _shaderStr.append("f, ");
    _shaderStr.append(std::to_string(color.GREEN).substr(0, 3)); // GREEN
    _shaderStr.append("f, ");
    _shaderStr.append(std::to_string(color.BLUE).substr(0, 3)); // BLUE
    _shaderStr.append("f, 1.0);\n}\n");
    
    
    _id_vbo = ++sFragmentShaderID;
    pCreate();
}

FragmentShader::~FragmentShader()
{
    if (sFragmentShaderID > 0)
        sFragmentShaderID--;
}

ShaderProgram::ShaderProgram(const VertexShader::Definition& vDefinition, const FragmentShader::Definition& fDefinition)
    : _id(0),
      _fShaderPtr(new FragmentShader(fDefinition)), _drawMode(vDefinition.DRAW_MODE)
{
    _id = glCreateProgram();
    _vShaderPtrVec.push_back(new VertexShader(vDefinition.TARGET, vDefinition.USAGE, vDefinition.VERTICES, vDefinition.VERTICES_NUM));
    
    const auto& _vShaderPtr = _vShaderPtrVec.back();

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
    for (auto _vShaderPtr : _vShaderPtrVec)
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
        for(auto& _vShaderPtr : _vShaderPtrVec)
        {
            delete _vShaderPtr;
            _vShaderPtr = nullptr;
        }
        delete _fShaderPtr;
        _fShaderPtr = nullptr;
    }
    _status = false;
    glDeleteProgram(_id);
}

void ShaderProgram::AddVertexShader(const VertexShader::Definition& vDefinition)
{
    _vShaderPtrVec.push_back(new VertexShader(vDefinition.TARGET, vDefinition.USAGE, vDefinition.VERTICES, vDefinition.VERTICES_NUM));
    const unsigned int vShaderID = _vShaderPtrVec.back()->GetID();
    glAttachShader(_id, vShaderID);

}

void ShaderProgram::Execute() const
{
    if (!_vShaderPtrVec.empty())
    {
        glUseProgram(_id);
        for(auto _vShaderPtr:_vShaderPtrVec)
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
