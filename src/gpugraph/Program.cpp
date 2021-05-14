/**
 * Derived from version of Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Program.h"


using namespace std;
using namespace glm;

namespace gpugraph
{

    template<>
    Program::Uniform<int>& Program::Uniform<int>::operator=(int const& value)
    {
        glUniform1i(_loc, value);
        _value = value;
        return *this;
    }

    template<>
    Program::Uniform<float>& Program::Uniform<float>::operator=(float const& value)
    {
        glUniform1f(_loc, value);
        _value = value;
        return *this;
    }

    template<>
    Program::Uniform<vec3>& Program::Uniform<vec3>::operator=(vec3 const& value)
    {
        glUniform3fv(_loc, 1, value_ptr(value));
        _value = value;
        return *this;
    }

    template<>
    Program::Uniform<mat3>& Program::Uniform<mat3>::operator=(mat3 const& value)
    {
        glUniformMatrix3fv(_loc, 1, GL_FALSE, value_ptr(value));
        _value = value;
        return *this;
    }

    template<>
    Program::Uniform<mat4>& Program::Uniform<mat4>::operator=(mat4 const& value)
    {
        glUniformMatrix4fv(_loc, 1, GL_FALSE, value_ptr(value));
        _value = value;
        return *this;
    }

    std::string read_file(const char* filename) 
    {
        std::ifstream file(filename, std::ios_base::binary);
        std::string buffer;
        if (!file.good())
        {
            throw std::invalid_argument(string("The file ") + filename +
                " doesn't exists");
        }
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios_base::beg);
        buffer.resize(static_cast<std::size_t>(size));
        file.read(&buffer[0], size);
        buffer += '\0';
        return buffer;
    }

    Program::Shader::Shader(std::string source, GLenum type) 
    {
        _handle = glCreateShader(type);
        if (_handle == 0)
            throw std::runtime_error("[Error] Impossible to create a new Shader");

        auto text = reinterpret_cast<const GLchar*>(&source[0]);
        glShaderSource(_handle, 1, &text, NULL);

        glCompileShader(_handle);

        GLint compile_status;
        glGetShaderiv(_handle, GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE) {
            GLsizei logsize = 0;
            glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &logsize);

            char* log = new char[logsize + 1];
            glGetShaderInfoLog(_handle, logsize, &logsize, log);

            // TODO: throw with designed exception
            std::cout << "failed to compile shader: " << endl;
            std::cout << log << endl;

            exit(EXIT_FAILURE);
        }
        else 
        {
            std::cout << "[Info] Shader compiled successfully" << endl;
        }
    }

    GLuint Program::Shader::handle() const 
    {
        return _handle;
    }

    Program::Shader::~Shader() {}

    Program::Program() 
    {
        _handle = glCreateProgram();
        if (!_handle)
            throw std::runtime_error("Impossible to create a new shader program");
    }

    Program::Program(std::vector<Shader> shaderList)
        : Program() 
    {
        for (auto& s : shaderList)
            glAttachShader(_handle, s.handle());
        link();
    }

    void Program::link() {
        glLinkProgram(_handle);
        GLint result;
        glGetProgramiv(_handle, GL_LINK_STATUS, &result);
        if (result != GL_TRUE) 
        {
            cout << "[Error] linkage error" << endl;

            GLsizei logsize = 0;
            glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &logsize);

            char* log = new char[logsize];
            glGetProgramInfoLog(_handle, logsize, &logsize, log);

            cout << log << endl;
        }
    }

    GLint Program::uniform(const std::string& name) 
    {
        auto it = _uniforms.find(name);
        if (it == _uniforms.end()) 
        {
            GLint r = glGetUniformLocation(_handle, name.c_str());
            if (r == GL_INVALID_OPERATION || r < 0)
                throw std::runtime_error("uniform " + name + " doesn't exist");
            _uniforms[name] = r;
            return r;
        }
        else
            return it->second;
    }

    GLint Program::attribute(const std::string& name) {
        GLint attrib = glGetAttribLocation(_handle, name.c_str());
        if (attrib == GL_INVALID_OPERATION || attrib < 0)
            cout << "[Error] Attribute " << name << " doesn't exist in program" << endl;

        return attrib;
    }

    void Program::set_attribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset)
    {
        GLint loc = attribute(name);
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, size, type, normalized, stride, reinterpret_cast<void*>(offset));
    }

    Program::~Program() 
    {
        glDeleteProgram(_handle);
    }

    void Program::bind() const 
    {
        glUseProgram(_handle);
    }
    
    void Program::release() const 
    {
        glUseProgram(0);
    }

    GLuint Program::handle() const 
    {
        return _handle;
    }

}
