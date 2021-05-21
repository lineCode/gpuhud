#include "RenderTarget.h"

#include <iostream>
#include <glm/glm.hpp>

#include "log.h"
#include "Program.h"


namespace gpugraph
{

    RenderTarget::BlitProgram::BlitProgram()
        : Program({
        VertexShader(R"source(
            #version 330
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
            #version 330
            uniform sampler2D texture;
            in vec2 tex;
            void main()
            {
                gl_FragColor = texture2D(texture, tex); 
            }
        )source") })
        , texture(this, "texture")
        , transform(this, "transform")
        , position(this, "position")
        , texture_coord(this, "texture_coord")
    {
        log_debug("created blit program");
    }

    std::unique_ptr<RenderTarget::Tile> RenderTarget::create_tile(std::size_t base_index, rect rect)
    {
        return std::make_unique<Tile>(this, base_index, std::move(rect));
    }

    RenderTarget::RenderTarget(
        std::size_t width,
        std::size_t height,
        std::size_t tile_width,
        std::size_t overlap)
        : _tile_width(tile_width)
        , _overlap(overlap)
        , _blit_program(std::make_unique<BlitProgram>())
    {
        glGenBuffers(1, &_vertex_buffer);
        set_size(width, height);
    }

    RenderTarget::~RenderTarget()
    {
        clear();
        glDeleteBuffers(1, &_vertex_buffer);
    }

    std::size_t RenderTarget::to_index(std::size_t x, std::size_t y)
    {
        return y * _tile_count_x + x;
    }

    void RenderTarget::set_size(std::size_t width, std::size_t height)
    {
        if (_width == width && _height == height)
            return;

        glPushAttrib(GL_TEXTURE_BIT);
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

        clear();

        _width = width;
        _height = height;

        if (width * height == 0)
            return;
        auto tw = _tile_width - _overlap * 2;
        _tile_count_x = (width + tw - 1) / tw;
        _tile_count_y = (height + tw - 1) / tw;

        auto tile_count = static_cast<std::size_t>(_tile_count_x * _tile_count_y);
        _framebuffer_objects.resize(tile_count);
        _texture_attachments.resize(tile_count);
        _depth_attachments.resize(tile_count);
        _tiles.resize(tile_count);

        glGenFramebuffers(static_cast<GLsizei>(tile_count), _framebuffer_objects.data());
        glGenTextures(static_cast<GLsizei>(tile_count), _texture_attachments.data());
        glGenRenderbuffers(static_cast<GLsizei>(tile_count), _depth_attachments.data());

        for (std::size_t i = 0; i < _framebuffer_objects.size(); ++i)
            _build_framebuffer(i);

        std::vector<GLfloat> vertices;
        std::vector<GLushort> indices;

        //
        // vertices & texture coords combined. the "2" because
        // we're using 2D-arrays of 4 vertices and 4 texture
        // coordinates per tile.
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

                _tiles[index] = create_tile(index, rect(x1, y1, x2, y2));
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
        if (vertices.size() > 0)
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPopClientAttrib();
        glPopAttrib();
    }

    void RenderTarget::_build_framebuffer(std::size_t index)
    {
        auto texture_id = _texture_attachments.at(index);
        auto fbo_id = _framebuffer_objects.at(index);
        auto depth_id = _depth_attachments.at(index);

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
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_id);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("failed to create render target, framebuffer incomplete");
        }
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
    }

    void RenderTarget::blit(glm::mat4 const& mat)
    {
        glPushAttrib(GL_TEXTURE_BIT);
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

        for (auto& tile : _tiles)
            tile->blit(mat);

        glPopClientAttrib();
        glPopAttrib();
    }

    std::vector<GLuint> const& RenderTarget::framebuffer_objects() const
    {
        return _framebuffer_objects;
    }

    std::size_t RenderTarget::overlap() const
    {
        return _overlap;
    }

    void RenderTarget::clear()
    {
        glDeleteFramebuffers(static_cast<GLsizei>(_framebuffer_objects.size()), _framebuffer_objects.data());
        glDeleteTextures(static_cast<GLsizei>(_texture_attachments.size()), _texture_attachments.data());
        glDeleteRenderbuffers(static_cast<GLsizei>(_depth_attachments.size()), _depth_attachments.data());

        _tiles.clear();
        _framebuffer_objects.clear();
        _texture_attachments.clear();
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

    std::size_t RenderTarget::tile_count_x() const
    {
        return _tile_count_x;
    }

    std::size_t RenderTarget::tile_count_y() const
    {
        return _tile_count_y;
    }

    std::vector<std::unique_ptr<RenderTarget::Tile>> const& RenderTarget::tiles() const
    {
        return _tiles;
    }
    
    void RenderTarget::Tile::render(std::function<void()> f)
    {
        //glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        auto fbo_id = _render_target->_framebuffer_objects.at(_base_index);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        glViewport(0, 0,
            static_cast<GLsizei>(_rectangle.width()),
            static_cast<GLsizei>(_rectangle.height()));
        glClearColor(1.0f, 0.2f, 0.0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Tile::blit(glm::mat4 const& transform)
    {
        auto& program = *_render_target->_blit_program;
        auto usage = program.use();

        program.transform = transform;
        program.texture = 0;
        glBindBuffer(GL_ARRAY_BUFFER, _render_target->_vertex_buffer);
        program.position.set(2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
        program.texture_coord.set(2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

        glActiveTexture(GL_TEXTURE0);
        auto texture_id = _render_target->_texture_attachments.at(_base_index);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<GLint>(_base_index) * 4, 4);        

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    RenderTarget::Tile::Tile(RenderTarget* render_target, std::size_t base_index, rect rectangle)
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
