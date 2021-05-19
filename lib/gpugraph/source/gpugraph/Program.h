#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "opengl.h"

namespace gpugraph
{

    class Program
    {
    public:
        class Shader;
        template<typename T>
        class Uniform;
        class Attribute;
        class VertexShader;
        class FragmentShader;
        class Usage;

        Program(std::vector<Shader> shader);
        ~Program();

        Usage use();

    private:
        void bind() const;
        void release() const;

        GLint attribute(const std::string& name);
        GLint uniform(const std::string& name);

        Program();
        std::vector<GLint> _attributes;

        GLuint _handle;

        void link();
    };

    class Program::Usage
    {
    public:
        Usage(Program&);
        ~Usage();

        Usage(const Usage&) = default;
        Usage(Usage&&) = delete;

        Usage& operator=(const Usage&) = delete;
        Usage& operator=(Usage&&) = default;

    private:
        GLint _last;
        Program& _program;
    };

    class Program::Shader
    {
    public:
        Shader(std::string source, GLenum type);
        Shader(Shader&&) = default;
        Shader(const Shader&);
        ~Shader();

        Shader& operator=(const Shader&) = delete;

        void compile();
        GLuint handle();

    private:
        std::string _source;
        GLenum _type;
        bool _compiled = false;
        GLuint _handle;
        std::vector<GLuint> _attributes;
        friend class Program;
    };

    class Program::VertexShader : public Program::Shader
    {
    public:
        VertexShader(std::string source);
    };

    class Program::FragmentShader : public Program::Shader
    {
    public:
        FragmentShader(std::string source);
    };

    class Program::Attribute
    {
    public:
        Attribute(Program*, std::string name);
        GLuint index() const;
        void set(GLint size, GLenum type, bool normalized, std::size_t stride, std::size_t offset);

    private:
        class Implementation;
        std::string _name;
        std::shared_ptr<Implementation> _pimpl;
    };

    template<typename T>
    class Program::Uniform
    {
    public:
        Uniform(Program*, std::string name);
        Uniform& operator=(T const&);

    private:
        class Implementation;
        std::shared_ptr<Implementation> _pimpl;
    };

    template<typename T>
    class Program::Uniform<T>::Implementation
    {
    public:
        Implementation(Program&, std::string name);
        void assign(T const& value);

    private:
        std::string _name;
        GLuint _loc;
    };

    template<typename T>
    Program::Uniform<T>::Implementation::Implementation(Program& program, std::string name)
        : _name(std::move(name))
    {
        program.bind();
        _loc = program.uniform(_name);
    }

    template<typename T>
    Program::Uniform<T>::Uniform(Program* program, std::string name)
        : _pimpl(std::make_shared<Implementation>(*program, std::move(name)))
    {
    }

    template<typename T>
    Program::Uniform<T>& Program::Uniform<T>::operator=(T const& value)
    {
        _pimpl->assign(value);
        return *this;
    }

}
