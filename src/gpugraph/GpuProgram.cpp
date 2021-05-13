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

#include "GpuProgram.h"


using namespace std;
using namespace glm;

namespace gpugraph
{

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

    GpuProgram::Shader::Shader(std::string source, GLenum type) 
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

    GLuint GpuProgram::Shader::handle() const 
    {
        return _handle;
    }

    GpuProgram::Shader::~Shader() {}

    GpuProgram::GpuProgram() 
    {
        _handle = glCreateProgram();
        if (!_handle)
            throw std::runtime_error("Impossible to create a new shader program");
    }

    GpuProgram::GpuProgram(std::vector<Shader> shaderList)
        : GpuProgram() 
    {
        for (auto& s : shaderList)
            glAttachShader(_handle, s.handle());
        link();
    }

    void GpuProgram::link() {
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

    GLint GpuProgram::uniform(const std::string& name) {
        auto it = _uniforms.find(name);
        if (it == _uniforms.end()) {
            // uniform that is not referenced
            GLint r = glGetUniformLocation(_handle, name.c_str());
            if (r == GL_INVALID_OPERATION || r < 0)
                cout << "[Error] uniform " << name << " doesn't exist in program" << endl;
            // add it anyways
            _uniforms[name] = r;

            return r;
        }
        else
            return it->second;
    }

    GLint GpuProgram::attribute(const std::string& name) {
        GLint attrib = glGetAttribLocation(_handle, name.c_str());
        if (attrib == GL_INVALID_OPERATION || attrib < 0)
            cout << "[Error] Attribute " << name << " doesn't exist in program" << endl;

        return attrib;
    }

    void GpuProgram::set_attribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset)
    {
        GLint loc = attribute(name);
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, size, type, normalized, stride, reinterpret_cast<void*>(0));
    }

    void GpuProgram::set_uniform(const std::string& name,
        float x,
        float y,
        float z) {
        glUniform3f(uniform(name), x, y, z);
    }

    void GpuProgram::set_uniform(const std::string& name, const vec3& v) {
        glUniform3fv(uniform(name), 1, value_ptr(v));
    }

    /*
    void GpuProgram::set_uniform(const std::string& name, const dvec3& v) {
        glUniform3dv(uniform(name), 1, value_ptr(v));
    }

    void GpuProgram::set_uniform(const std::string& name, const vec4& v) {
        glUniform4fv(uniform(name), 1, value_ptr(v));
    }

    void GpuProgram::set_uniform(const std::string& name, const dvec4& v) {
        glUniform4dv(uniform(name), 1, value_ptr(v));
    }

    void GpuProgram::set_uniform(const std::string& name, const dmat4& m) {
        glUniformMatrix4dv(uniform(name), 1, GL_FALSE, value_ptr(m));
    }*/

    void GpuProgram::set_uniform(const std::string& name, const mat4& m) {
        glUniformMatrix4fv(uniform(name), 1, GL_FALSE, value_ptr(m));
    }

    void GpuProgram::set_uniform(const std::string& name, const mat3& m) {
        glUniformMatrix3fv(uniform(name), 1, GL_FALSE, value_ptr(m));
    }

    void GpuProgram::set_uniform(const std::string& name, float val) {
        glUniform1f(uniform(name), val);
    }

    void GpuProgram::set_uniform(const std::string& name, int val) {
        glUniform1i(uniform(name), val);
    }

    GpuProgram::~GpuProgram() 
    {
        glDeleteProgram(_handle);
    }

    void GpuProgram::bind() const 
    {
        glUseProgram(_handle);
    }
    
    void GpuProgram::release() const 
    {
        glUseProgram(0);
    }

    GLuint GpuProgram::handle() const 
    {
        return _handle;
    }

}
