#include "RenderTarget.h"

#include <iostream>

namespace gpugraph
{

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

        if (width * height == 0)
            return;
        auto tw = _tile_width - _overlap * 2;
        _tile_count_x = (width + tw - 1) / tw;
        _tile_count_y = (height + tw - 1) / tw;

        //        std::cout << "tiles: " << _tile_count_x << "x" << _tile_count_y << std::endl;
        _fbos.resize(static_cast<std::size_t>(_tile_count_x * _tile_count_y));
        glGenBuffers(static_cast<GLsizei>(_fbos.size()), _fbos.data());

        _textures.resize(static_cast<std::size_t>(_tile_count_x * _tile_count_y));
        glGenTextures(static_cast<GLsizei>(_textures.size()), _textures.data());
        for (std::size_t i = 0; i < _textures.size(); ++i)
        {
            auto texture_id = _textures.at(i);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glBindFramebuffer(GL_FRAMEBUFFER, _fbos.at(i));
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, 
                static_cast<GLsizei>(_tile_width),
                static_cast<GLsizei>(_tile_width), 
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                throw std::runtime_error("failed to create render target, framebuffer incomplete");
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _tiles.resize(_fbos.size());
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
                auto pos = rect(x1, y1, x2, y2);
                _tiles[index] = std::make_unique<Tile>(
                    _fbos.at(index),
                    _textures.at(index),
                    std::move(pos));
            }
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
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(2, 2, 0.0f);
        glVertex3f(2, 102, 0.0f);
        glVertex3f(102, 102, 0.0f);
        glVertex3f(102, 2, 0.0f);
        glEnd();
    }

    void RenderTarget::clear()
    {
        glDeleteFramebuffers(static_cast<GLsizei>(_fbos.size()), _fbos.data());
        glDeleteTextures(static_cast<GLsizei>(_textures.size()), _textures.data());
        _tiles.clear();
        _fbos.clear();
        _textures.clear();
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

    void RenderTarget::Tile::render(std::function<void()> f)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glViewport(0, 0,
            static_cast<GLsizei>(_rectangle.width()),
            static_cast<GLsizei>(_rectangle.height()));
        f();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    RenderTarget::Tile::Tile(GLuint fbo, GLuint texture, rect rectangle)
        : _fbo(fbo)
        , _texture(texture)
        , _rectangle(std::move(rectangle))
    {
    }

    rect const& RenderTarget::Tile::rectangle() const
    {
        return _rectangle;
    }

}
