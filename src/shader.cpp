#include "shader.h"

std::string Shader::path = "../data/shaders";

Shader::Shader() : program(0)
{
}

Shader::~Shader()
{
    deleteShader();
}

GLuint Shader::loadShader(const std::string& name)
{
    deleteShader();
    this->name = name;

    const std::string fragPath = path + "/" + name + ".frag";
    const std::string vertPath = path + "/" + name + ".vert";

    GLuint fragid = loadShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint vertid = loadShader(GL_VERTEX_SHADER, vertPath);

    if( fragid == 0 )
    {
        klog.e("shader") << "Fragment shader failed!";
        // TODO: throw exception
    }

    if( vertid == 0 )
    {
        klog.e("shader") << "Vertex shader failed!";
        // TODO: throw exception
    }

    if( fragid == 0 || vertid == 0 )
    {
        // TODO: throw exception
        return 0;
    }

    std::vector<GLuint> shaders;
    shaders.push_back(fragid);
    shaders.push_back(vertid);

    program = createProgram(shaders);

    if( program > 0 )
    {
        klog.i("shader") << "Shader program successfully created.";
    }
    else
    {
        klog.e("shader") << "Shader program creation failed!";
    }

    return program;
}

bool Shader::deleteShader()
{
    if( program > 0)
    {
        glDeleteProgram(program);

        //TOOD: check for GL_INVALID_VALUE
    }

    return true;
}

void Shader::use()
{
    glUseProgram(program);
}

void Shader::unuse()
{
    glUseProgram(0);
}

GLuint Shader::getProgram() const
{
    return program;
}

GLuint Shader::loadShader(GLuint type, const std::string& path)
{
    std::fstream in;
    in.open(path.c_str(), std::ios_base::in);

    std::string data;
    if( in.is_open() )
    {
        std::string line;
        while( in.good() )
        {
            std::getline(in, line);
            data += (line + "\n");
        }

        in.close();
    }
    else
    {
        klog.e("shader") << "File not opened: " << path;
        in.close();
        return 0;
    }


    klog.i("shader") << "Shader read from" << path;

    return uploadShader(type, data.c_str());
}

GLuint Shader::uploadShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, NULL);
    glCompileShader(shader);

    // TODO: check GL errors

    return shader;
}

GLuint Shader::createProgram(const std::vector<GLuint>& shaders)
{
    GLuint _program = glCreateProgram();
    for(std::vector<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
        GLuint shader = (GLuint) *it;
        glAttachShader(_program, shader);
    }

    glLinkProgram(_program);

    for(std::vector<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
        GLuint shader = (GLuint) *it;
        glDetachShader(_program, shader);
        glDeleteShader(shader);
    }

    return _program;
}

GLint Shader::addAttribute(const std::string& attr)
{
    const GLchar* _attr = attr.c_str();

    if( program != 0 )
    {
        if( attributes.count(_attr) > 0 )
            return attributes[_attr];

        GLint attrloc = glGetAttribLocation(program, _attr);
        // TODO: check GL_INVALID_OPERATION
        attributes[_attr] = attrloc;

        return attrloc;
    }

    return -1;
}

GLint Shader::addUniform(const std::string& unifrm)
{
    const GLchar* _unifrm = unifrm.c_str();

    if( program != 0 )
    {
        if( uniforms.count(_unifrm) > 0 )
            return uniforms[_unifrm];

        GLint uniformloc = glGetUniformLocation(program, _unifrm);
        // TODO: check GL_INVALID_OPERATION
        uniforms[(const char*)_unifrm] = uniformloc;

        return uniformloc;
    }

    return -1;
}

GLint Shader::attribute(const std::string& attr)
{
    return addAttribute(attr);
}

GLint Shader::uniform(const std::string& unifrm)
{
    return addUniform(unifrm);
}



