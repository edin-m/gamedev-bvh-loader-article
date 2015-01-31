#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>

#include "klib/logger.h"

class Shader
{
    static std::string path;

    GLuint program;
    std::string name;

    std::map<std::string, GLint> attributes;
    std::map<std::string, GLint> uniforms;

    GLuint loadShader(GLuint type, const std::string& path);
    GLuint uploadShader(GLenum type, const char* source);

    GLuint createProgram(const std::vector<GLuint>& shaders);
    GLint addAttribute(const std::string& attr);
    GLint addUniform(const std::string& unifrm);
public:
    Shader();
    ~Shader();

    GLuint loadShader(const std::string& name);
    bool deleteShader();

    void use();
    void unuse();
    GLuint getProgram() const;


    GLint attribute(const std::string& attr);
    GLint uniform(const std::string& unifrm);

};


#endif // SHADER_H
