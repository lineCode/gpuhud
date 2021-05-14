#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <functional>

#include "Rectangle.h"
#include "Program.h"
#include "opengl.h"


namespace gpugraph
{

    // TODO: optimize with "npot" (some day)
    class RenderTarget
    {
    public:
        class BlitProgram : public Program
        {
        public:
            BlitProgram();

            Uniform<int> texture;
            Uniform<glm::mat4> transform;

            Attribute position;
            Attribute texture_coord;
        };

        struct Tile
        {
        public:
            /// :-)
            Tile(RenderTarget *, std::size_t base_index, rect x);

            /// tile vertices
            rect const& rectangle() const;
            
            /// render to tile (gl)
            void render(std::function<void()> render_func);

            /// draw tile with some transformation applied
            void blit(glm::mat4 const&);

        private:
            RenderTarget* _render_target;
            std::size_t _base_index;
            rect _rectangle;
        };

        /// :-)
        RenderTarget(
            std::size_t width=0, 
            std::size_t height=0,
            std::size_t tile_width=256,
            std::size_t overlap=1);
        
        /// :-)
        ~RenderTarget();

        /// resize the target. will only happen if width or height was changed
        void set_size(std::size_t width, std::size_t height);
       
        /// :-)
        std::array<std::size_t, 2> size() const;

        /// get tile by tile position
        Tile& tile(std::size_t index_x, std::size_t index_y);

        /// :-)
        std::vector<std::unique_ptr<Tile>> const& tiles() const;

        /// :-)
        std::size_t tile_count() const;

        /// outputs tile vertices as GL_LINE_LOOP to gl
        void _debug_draw();

        /// draw the whole tiling, which is actually a plane, with some transformation applied
        void blit(glm::mat4 const&);

    private:
        void clear();
        std::size_t to_index(std::size_t x, std::size_t y);
        void _build_framebuffer(std::size_t index);

        std::size_t _width = 0;
        std::size_t _height = 0;
        std::size_t _tile_width = 0;
        std::size_t _overlap = 0;

        std::size_t _tile_count_x = 0;
        std::size_t _tile_count_y = 0;
        
        std::vector<GLuint> _framebuffer_objects;
        std::vector<GLuint> _depth_attachments;
        std::vector<GLuint> _texture_attachments;

        GLuint _vertex_buffer;

        std::vector<std::unique_ptr<Tile>> _tiles;
        std::unique_ptr<BlitProgram> _blit_program;
    };

}
