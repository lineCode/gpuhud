#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Rectangle.h"

namespace gpugraph
{

    class RenderTarget
    {
    public:
        struct Tile
        {
        public:
            Tile(GLuint fbo, GLuint texture, rect x);
            rect const& rectangle() const;
            
            /// render to tile. binds the fbo and sets
            /// the viewport to rect.width(), rect.height()
            /// before the call to render_func
            void render(std::function<void()> render_func);

        private:
            rect _rectangle;
            GLuint _fbo;
            GLuint _texture;
        };

        RenderTarget(
            std::size_t width=0, 
            std::size_t height=0,
            std::size_t tile_width=512,
            std::size_t overlap=1);

        ~RenderTarget();

        void set_size(std::size_t width, std::size_t height);
        std::array<std::size_t, 2> size() const;

        Tile& tile(std::size_t x, std::size_t y);

        void _debug_draw();

    private:
        void clear();

        std::size_t to_index(std::size_t x, std::size_t y);

        std::size_t _width = 0;
        std::size_t _height = 0;
        std::size_t _tile_width = 0;
        std::size_t _overlap = 0;
        std::size_t _tile_count_x = 0;
        std::size_t _tile_count_y = 0;
        std::vector<GLuint> _fbos;
        std::vector<GLuint> _textures;
        std::vector<std::unique_ptr<Tile>> _tiles;
    };

}
