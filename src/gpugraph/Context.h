#pragma once

namespace gpugraph
{

    struct None
    {
    } none;

    class Context
    {
    public:
        enum class Edge
        {
            Left = 0,
            Top = 1,
            Right = 2,
            Bottom = 3
        };

        using vec2 = std::array<float, 2>;
        using vec4 = std::array<float, 4>;
        using color = std::array<float, 4>;

        Node* parent;

        color border_color;
        vec4 border_width;
        vec4 border_radius;
    };


}
