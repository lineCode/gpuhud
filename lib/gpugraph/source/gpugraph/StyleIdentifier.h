#pragma once

namespace gpugraph
{

    // maybe std::uint16_t some day..
    enum class StyleIdentifier : std::uint8_t
    {
        Display = 0,
        Position,

        Left,
        Top,
        Right,
        Bottom,

        Width,
        Height,

        FontSize,
        FontFamily,
        Color,
    };

}
