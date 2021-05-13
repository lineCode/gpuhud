#pragma once

#include <vector>
#include <cstdint>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gpuhud
{

    class RenderTarget
    {
    public:
        RenderTarget(
            std::uint32_t width=0, 
            std::uint32_t height=0,
            std::uint32_t tile_width=512,
            std::uint32_t overlap=1);

        ~RenderTarget();

        void resize(std::uint32_t width, std::uint32_t height);

        template<typename F>
        void render_to(std::uint32_t x, std::uint32_t y, F&&);

    private:
        void clear();

        std::size_t to_index(std::uint32_t x, std::uint32_t y);

        std::uint32_t _width = 0;
        std::uint32_t _height = 0;
        std::uint32_t _tile_width = 0;
        std::uint32_t _overlap = 0;
        std::uint32_t _tile_count_x = 0;
        std::uint32_t _tile_count_y = 0;
        std::vector<GLuint> _fbos;
        std::vector<GLuint> _textures;
    };

    template<typename F>
    void RenderTarget::render_to(std::uint32_t x, std::uint32_t y, F&&)
    {
        fbo = to_index(x, y);
        glBindFramebuffer(fbo);
        glViewport(0, 0, _tile_width, _tile_width);
        glOrtho(left, right, bottom, top, 0.0, 1.0);
        f();
    }

}
