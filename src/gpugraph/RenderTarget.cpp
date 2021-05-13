#include "RenderTarget.h"

namespace gpuhud
{

    TiledRenderTarget::TiledRenderTarget(
        std::uint32_t width, 
        std::uint32_t height, 
        std::uint32_t tile_width,
        std::uint32_t overlap)
    {
    }

    TiledRenderTarget::~TiledRenderTarget()
    {
        clear();
    }

    std::size_t TiledRenderTarget::to_index(std::uint32_t x, std::uint32_t y)
    {
        return _fbos.at(y * _tile_count_x + x);
    }

    void TiledRenderTarget::resize(std::uint32_t width, std::uint32_t height)
    {
        if (_width == width && _height == height)
            return;

        clear();

        if (width * height == 0)
            return;

        _tile_count_x = (width + _overlap) / _tile_width;
        _tile_count_y = (height + _overlap) / _tile_width;

        _fbos.resize(static_cast<std::size_t>(_tile_count_x * _tile_count_y));
        glGenBuffers(static_cast<GLsizei>(_fbos.size()), _fbos.data());

        _textures.resize(static_cast<std::size_t>(_tile_count_x * _tile_count_y));
        glGenTextures(static_cast<GLsizei>(_textures.size()), _textures.data());
        for (auto const texture : _textures)
        {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(_tile_width),
                static_cast<GLsizei>(_tile_width), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }

    void TiledRenderTarget::clear()
    {
        glDeleteFramebuffers(static_cast<GLsizei>(_fbos.size()), _fbos.data());
        glDeleteTextures(static_cast<GLsizei>(_textures.size()), _textures.data());
        _fbos.clear();
        _textures.clear();
        _tile_count_x = _tile_count_y = 0;
    }

}
