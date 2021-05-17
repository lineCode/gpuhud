#pragma once

#include <cstdint>
#include <variant>
#include "types.h"

namespace gpugraph
{

    //
    // <length>
    struct Length
    {
        enum class Unit : std::uint8_t
        {
            FontSize = 0,
            RootFontSize = 1,
            HeightOfLowerLetterX = 2,
            CharacterHorizontal = 3 /*of '0'*/,
            ViewportWidth = 4,
            ViewportHeight = 5,
            ViewportMin = 6,
            ViewportMax = 7,
            Pixel = 8,
            Centimeter = 9,
            Millimeter = 10,
            Inch = 11,
            Pica = 12,
            Point = 13
        };
        inline static auto const px = Unit::Pixel;
        inline static auto const mm = Unit::Millimeter;
        inline static auto const cm = Unit::Centimeter;
        inline static auto const in = Unit::Inch;
        inline static auto const pc = Unit::Pica;
        inline static auto const pt = Unit::Point;

        inline static auto const vw = Unit::ViewportWidth;
        inline static auto const vh = Unit::ViewportHeight;
        inline static auto const vmin = Unit::ViewportMin;
        inline static auto const vmax = Unit::ViewportMax;

        inline static auto const em = Unit::FontSize;
        inline static auto const rem = Unit::RootFontSize;
        inline static auto const ex = Unit::HeightOfLowerLetterX;
        inline static auto const ch = Unit::CharacterHorizontal;

        Unit unit = Unit::Pixel;
        real_t value = real_t(0);
    };

    template<typename T>
    Length operator |(T value, Length::Unit unit)
    {
        return Length{ unit, static_cast<float>(value) };
    }

    //
    // css: %
    struct Percentage
    {
        real_t value = real_t(0);
    };

    inline static auto const percentage = Percentage{ real_t(1) };

    //
    // css: width, height
    enum class SizingKeyword
    {
        Automatic,
        Inherit,
        Unset,

        BorderBox,
        ContentBox,
        MaxContent,
        MinContent,
        Available,
        FitContent
    };

    using Sizing = std::variant<SizingKeyword, Length>;

    //
    // css: position
    enum class Position
    {
        Absolute,
        Static
    };

    enum class Display
    {
        Flex,
        Inline
    };

}
