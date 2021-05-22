/***************************************************************************//*/

 Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

/******************************************************************************/
#include "Program.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Program.h"

#include "log.h"

namespace gpugraph
{

    Program::Usage::Usage(Program& program)
        : _program(program)
    {
        glGetIntegerv(GL_CURRENT_PROGRAM, &_last);
        _program.bind();
        for(auto index : _program._attributes)
            glEnableVertexAttribArray(index);
    }

    Program::Usage::~Usage()
    {
        for(auto index : _program._attributes)
            glDisableVertexAttribArray(index);
        glUseProgram(_last);
        //_program.release();
    }

    Program::Usage Program::use()
    {
        return Usage(*this);
    }

    class Program::Attribute::Implementation
    {
    public:
        Implementation(Program&, std::string const&);
        GLuint index() const;
    private:
        GLuint _index;
    };

    Program::Attribute::Attribute(Program* program, std::string  name)
        : _name(std::move(name))
        , _pimpl(std::make_shared<Implementation>(*program, _name))
    {
    }

    GLuint Program::Attribute::index() const
    {
        return _pimpl->index();
    }

    void Program::Attribute::set(GLint size, GLenum type, bool normalized, std::size_t stride, std::size_t offset)
    {
        auto temp = static_cast<GLintptr>(offset);
        glVertexAttribPointer(index(), 
            size, 
            type, 
            normalized ? GL_TRUE : GL_FALSE, 
            static_cast<GLsizei>(stride),
            reinterpret_cast<GLvoid *>(temp));
    }

    Program::Attribute::Implementation::Implementation(Program& program, std::string const& name)
    {
        program.bind();
        _index = program.attribute(name);
        program._attributes.push_back(_index);
    }

    GLuint Program::Attribute::Implementation::index() const
    {
        return _index;
    }


    Program::Shader::Shader(std::string source, GLenum type)
        : _source(std::move(source))
        , _type(type)
    {
    }

    void Program::Shader::compile()
    {
        if (_handle)
            return;

        GLuint handle = glCreateShader(_type);
        if (handle == 0)
            throw std::runtime_error("failed to create shader");

        auto text = reinterpret_cast<const GLchar*>(&_source[0]);
        glShaderSource(handle, 1, &text, NULL);

        glCompileShader(handle);

        GLint compile_status;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE) {
            GLsizei logsize = 0;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logsize);

            char* log = new char[static_cast<std::size_t>(logsize) + 1];
            glGetShaderInfoLog(handle, logsize, &logsize, log);

            log_fatal("failed to compile shader: " << std::endl << log << std::endl << "shader source:" << std::endl << text << std::endl)
        }
        else 
        {
            _handle = handle;
        }
    }

    GLuint Program::Shader::handle()
    {
        if (!_compiled)
        {
            compile();
            _compiled = true;
        }
        return _handle;
    }

    Program::Shader::Shader(const Shader& shader)
        : _source(shader._source)
        , _type(shader._type)
    {
    }

    Program::Shader::~Shader() 
    {
        if(_compiled)
            glDeleteShader(_handle);
    }

    Program::Program() 
    {
        _handle = glCreateProgram();
        if (!_handle)
            throw std::runtime_error("could not create program");
    }

    Program::Program(std::vector<Shader> shaderList)
        : Program() 
    {
        for (auto& s : shaderList)
        {
            auto handle = s.handle();
            glAttachShader(_handle, s.handle());
        }
        link();
    }

    void Program::link() {
        glLinkProgram(_handle);
        GLint result;
        glGetProgramiv(_handle, GL_LINK_STATUS, &result);
        if (result != GL_TRUE) 
        {
            GLsizei logsize = 0;
            glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &logsize);

            char* log = new char[logsize];
            glGetProgramInfoLog(_handle, logsize, &logsize, log);

            log_fatal("failed to link program: " << std::endl << log)
        }
    }

    GLint Program::uniform(const std::string& name) 
    {
        GLint r = glGetUniformLocation(_handle, name.c_str());
        if (r == GL_INVALID_OPERATION || r < 0)
            throw std::runtime_error("uniform " + name + " doesn't exist");
        return r;
    }

    GLint Program::attribute(const std::string& name) {
        GLint attrib = glGetAttribLocation(_handle, name.c_str());
        if (attrib == GL_INVALID_OPERATION || attrib < 0)
            log_error("attribute " << name << " doesn't exist in program");

        return attrib;
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

    Program::FragmentShader::FragmentShader(std::string source)
        : Shader(std::move(source), GL_FRAGMENT_SHADER)
    {
    }

    Program::VertexShader::VertexShader(std::string source)
        : Shader(std::move(source), GL_VERTEX_SHADER)
    {
    }

    template<>
    void Program::Uniform<int>::Implementation::assign(int const& value)
    {
        glUniform1i(_loc, value);
    }

    template<>
    void Program::Uniform<float>::Implementation::assign(float const& value)
    {
        glUniform1f(_loc, value);
    }

    template<>
    void Program::Uniform<glm::vec3>::Implementation::assign(glm::vec3 const& value)
    {
        glUniform3fv(_loc, 1, glm::value_ptr(value));
    }

    template<>
    void Program::Uniform<glm::mat3>::Implementation::assign(glm::mat3 const& value)
    {
        glUniformMatrix3fv(_loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    template<>
    void Program::Uniform<glm::mat4>::Implementation::assign(glm::mat4 const& value)
    {
        glUniformMatrix4fv(_loc, 1, GL_FALSE, glm::value_ptr(value));
    }

}
