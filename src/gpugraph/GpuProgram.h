/**
 * Derived from version of Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>

#include "opengl.h"

namespace gpugraph
{

    class GpuProgram 
    {
    public:
        class Shader;
        class VertexShader;
        class FragmentShader;

        // constructor
        GpuProgram(std::vector<Shader> shader);

        void bind() const;
        void release() const;

        GLuint handle() const;

        GLint attribute(const std::string& name);
        void set_attribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset);

        GLint uniform(const std::string& name);

        void set_uniform(const std::string& name, float x, float y, float z);
        void set_uniform(const std::string& name, const glm::vec3& v);
        /*void set_uniform(const std::string& name, const glm::dvec3& v);
        void set_uniform(const std::string& name, const glm::vec4& v);
        void set_uniform(const std::string& name, const glm::dvec4& v);
        void set_uniform(const std::string& name, const glm::dmat4& m);*/
        void set_uniform(const std::string& name, const glm::mat4& m);
        void set_uniform(const std::string& name, const glm::mat3& m);
        void set_uniform(const std::string& name, float val);
        void set_uniform(const std::string& name, int val);

        ~GpuProgram();

    private:
        GpuProgram();

        std::map<std::string, GLint> _uniforms;
        std::map<std::string, GLint> _attributes;

        GLuint _handle;

        void link();
    };

    class GpuProgram::Shader
    {
    public:
        Shader(std::string source, GLenum type);
        ~Shader();

        GLuint handle() const;

    private:
        GLuint _handle;
        friend class GpuProgram;
    };

    class GpuProgram::VertexShader : public GpuProgram::Shader
    {
    public:
        VertexShader(std::string source)
            : Shader(std::move(source), GL_VERTEX_SHADER)
        {
        }
    };

    class GpuProgram::FragmentShader : public GpuProgram::Shader
    {
    public:
        FragmentShader(std::string source)
            : Shader(std::move(source), GL_FRAGMENT_SHADER)
        {
        }
    };

}
