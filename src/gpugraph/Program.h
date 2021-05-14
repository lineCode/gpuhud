/**
 * Derived from version of Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "opengl.h"

namespace gpugraph
{

    class Program 
    {
    public:
        template<typename T>
        class Uniform
        {
        public:
            Uniform(Program* program, const std::string& name)
            {
                program->bind();
                _loc = program->uniform(name);
            }

            Uniform& operator=(T const&);
            operator T const& () const { return _value; }

        private:
            GLuint _loc;
            T _value;
        };

        class Shader;
        class VertexShader;
        class FragmentShader;

        Program(std::vector<Shader> shader);

        void bind() const;
        void release() const;

        GLuint handle() const;

        GLint attribute(const std::string& name);
        void set_attribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset);

        ~Program();

    private:
        GLint uniform(const std::string& name);
        
        Program();

        std::map<std::string, GLint> _uniforms;
        std::map<std::string, GLint> _attributes;

        GLuint _handle;

        void link();
    };

    class Program::Shader
    {
    public:
        Shader(std::string source, GLenum type);
        ~Shader();

        GLuint handle() const;

    private:
        GLuint _handle;
        friend class Program;
    };

    class Program::VertexShader : public Program::Shader
    {
    public:
        VertexShader(std::string source)
            : Shader(std::move(source), GL_VERTEX_SHADER)
        {
        }
    };

    class Program::FragmentShader : public Program::Shader
    {
    public:
        FragmentShader(std::string source)
            : Shader(std::move(source), GL_FRAGMENT_SHADER)
        {
        }
    };

}
