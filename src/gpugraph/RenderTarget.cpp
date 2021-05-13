#include "RenderTarget.h"

#include <iostream>
#include <glm/glm.hpp>

#include "GpuProgram.h"

namespace gpugraph
{


    class BlitProgram : public GpuProgram
    {
    public:
        // TODO: move this later to RenderState or whatever
        static BlitProgram& instance()
        {
            static std::shared_ptr<BlitProgram> instance;
            if (!instance)
                instance = std::shared_ptr<BlitProgram>(new BlitProgram);
            return *instance;
        }

    private:
        BlitProgram() : GpuProgram({
            VertexShader(R"source(
                uniform mat4 transform;

                in vec2 position;
                in vec2 texture_coord;
                
                out vec2 tex;

                void main()
                {
                    tex = texture_coord;
                    gl_Position = transform * vec4(position.x, position.y, 0, 1.0);
                }
            )source"),
            FragmentShader(R"source(
                uniform sampler2D texture;
                in vec2 tex;
                void main()
                {
                   gl_FragColor = texture2D(texture, tex); 
                }
            )source") })
        {
        }
    };


    RenderTarget::RenderTarget(
        std::size_t width,
        std::size_t height,
        std::size_t tile_width,
        std::size_t overlap)
        : _tile_width(tile_width)
        , _overlap(overlap)
    {
        set_size(width, height);
    }

    RenderTarget::~RenderTarget()
    {
        clear();
        glDeleteBuffers(1, &_vbo);
    }

    std::size_t RenderTarget::to_index(std::size_t x, std::size_t y)
    {
        return y * _tile_count_x + x;
    }

    void RenderTarget::set_size(std::size_t width, std::size_t height)
    {
        if (_width == width && _height == height)
            return;

        clear();

        _width = width;
        _height = height;

        if (width * height == 0)
            return;
        auto tw = _tile_width - _overlap * 2;
        _tile_count_x = (width + tw - 1) / tw;
        _tile_count_y = (height + tw - 1) / tw;

        auto tile_count = static_cast<std::size_t>(_tile_count_x* _tile_count_y);
        _fbos.resize(tile_count);
        _textures.resize(tile_count);
        _depth.resize(tile_count);

        glGenFramebuffers(static_cast<GLsizei>(tile_count), _fbos.data());
        glGenTextures(static_cast<GLsizei>(tile_count), _textures.data());
        glGenRenderbuffers(static_cast<GLsizei>(tile_count), _depth.data());
        for (std::size_t i = 0; i < _fbos.size(); ++i)
        {
            auto texture_id = _textures.at(i);
            auto fbo_id = _fbos.at(i);
            auto depth_id = _depth.at(i);
            
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA,
                static_cast<GLsizei>(_tile_width),
                static_cast<GLsizei>(_tile_width),
                0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
	        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 
                static_cast<GLsizei>(_tile_width), 
                static_cast<GLsizei>(_tile_width));
	        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth.at(i));

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                throw std::runtime_error("failed to create render target, framebuffer incomplete");
            }
        }

//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _tiles.resize(_fbos.size());
        std::vector<GLfloat> vertices;
        std::vector<GLushort> indices;
        // 1 quad for each tile..
        vertices.resize(2 * (4 + 4) * _tiles.size());

        auto vertex = vertices.data();
        std::uint16_t count = 0;
        for (std::size_t y = 0; y < _tile_count_y; ++y)
        {
            for (std::size_t x = 0; x < _tile_count_x; ++x)
            {
                auto index = to_index(x, y);
                auto tw = _tile_width - 2 * _overlap;
                auto x1 = static_cast<float>(x) * tw - _overlap;
                auto y1 = static_cast<float>(y) * tw - _overlap;
                auto x2 = static_cast<float>(x) * tw + _tile_width - _overlap;
                auto y2 = static_cast<float>(y) * tw + _tile_width - _overlap;
                *vertex++ = x1; *vertex++ = y1;
                *vertex++ = 0.f; *vertex++ = 0.f;
                
                *vertex++ = x2; *vertex++ = y1;
                *vertex++ = 1.f; *vertex++ = 0.f;
                
                *vertex++ = x1; *vertex++ = y2;
                *vertex++ = 0.f; *vertex++ = 1.f;
                
                *vertex++ = x2; *vertex++ = y2;
                *vertex++ = 1.f; *vertex++ = 1.f;

                _tiles[index] = std::make_unique<Tile>(this, index,
                    rect(x1, y1, x2, y2));
            }
        }
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        if(vertices.size() > 0)
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void RenderTarget::_debug_draw()
    {
        for (auto const& tile : _tiles)
        {
            auto const& r = tile->rectangle();
            glColor4f(0.f, 0.f, 1.f, 1.f);
            glBegin(GL_LINE_LOOP);
            glVertex3f(r.x1, r.y1, 0.0f);
            glVertex3f(r.x1, r.y2, 0.0f);
            glVertex3f(r.x2, r.y2, 0.0f);
            glVertex3f(r.x2, r.y1, 0.0f);
            glEnd();
        }
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(2, 2, 0.0f);
        glVertex3f(2, 102, 0.0f);
        glVertex3f(102, 102, 0.0f);
        glVertex3f(102, 2, 0.0f);
        glEnd();
    }

    void RenderTarget::blit(glm::mat4 const& mat)
    {
        for (auto& tile : _tiles)
            tile->blit(mat);
    }

    void RenderTarget::clear()
    {
        glDeleteFramebuffers(static_cast<GLsizei>(_fbos.size()), _fbos.data());
        glDeleteTextures(static_cast<GLsizei>(_textures.size()), _textures.data());
        glDeleteRenderbuffers(static_cast<GLsizei>(_depth.size()), _depth.data());
        _tiles.clear();
        _fbos.clear();
        _textures.clear();
        _depth.clear();
        _tile_count_x = _tile_count_y = 0;
    }

    std::array<std::size_t, 2> RenderTarget::size() const
    {
        return { _width, _height };
    }

    RenderTarget::Tile& RenderTarget::tile(std::size_t x, std::size_t y)
    {
        return *_tiles.at(to_index(x, y));
    }

    std::size_t RenderTarget::tile_count() const
    {
        return _tiles.size();
    }

    std::vector<std::unique_ptr<RenderTarget::Tile>> const& RenderTarget::tiles() const
    {
        return _tiles;
    }

    void RenderTarget::Tile::render(std::function<void()> f)
    {
	    glDisable(GL_TEXTURE_2D);
        auto fbo_id = _render_target->_fbos.at(_base_index);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        glViewport(0, 0,
            static_cast<GLsizei>(_rectangle.width()),
            static_cast<GLsizei>(_rectangle.height()));
        glClearColor(0.1f, 0.2f, 0.0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Tile::blit(glm::mat4 const& transform)
    {
        auto& program = BlitProgram::instance();
        program.bind();
        glBindBuffer(GL_ARRAY_BUFFER, _render_target->_vbo);
        program.set_attribute("position", 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
        program.set_attribute("texture_coord", 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);
        program.set_uniform("transform", transform);
        auto texture_id = _render_target->_textures.at(_base_index);
        program.set_uniform("texture", static_cast<int>(0));
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<GLint>(_base_index)*4, 4);
        program.release();

        glDisable(GL_TEXTURE_2D);
        glDisableVertexAttribArray(program.attribute("position"));
        glDisableVertexAttribArray(program.attribute("texture_coord"));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    RenderTarget::Tile::Tile(RenderTarget *render_target, std::size_t base_index, rect rectangle)
        : _render_target(render_target)
        , _base_index(base_index)
        , _rectangle(std::move(rectangle))
    {
    }

    rect const& RenderTarget::Tile::rectangle() const
    {
        return _rectangle;
    }

}
