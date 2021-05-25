#pragma once

#include <memory>
#include <variant>

#include <css/parser.hpp>

#include "StyleIdentifier.h"

namespace gpugraph
{

    struct ComputedStyle
    {
    public:
        virtual ~ComputedStyle() = default;

        struct Display;
        struct Position;

        struct Left;
        struct Top;
        struct Right;
        struct Bottom;
        
        class Width;
        class Height;

        class FontSize;
        class FontFamily;
        class FontStyle;
        class Color;
        class BackgroundColor;

        virtual std::string to_string() const {
            return std::string();
        }
    };

    struct ComputedStyle::Display : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Display;
        Display(css::display value) : value(std::move(value)) {}
        css::display value;
    };

    struct ComputedStyle::Position : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Position;
        Position(css::position value) : value(std::move(value)) {}
        css::position value;
    };

    struct ComputedStyle::Left : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Left;
        Left(css::distance value) : value(std::move(value)) {}
        css::distance value;
    };

    struct ComputedStyle::Top : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Top;
        Top(css::distance value) : value(std::move(value)) {}
        css::distance value;
    };

    struct ComputedStyle::Right : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Right;
        Right(css::distance value) : value(std::move(value)) {}
        css::distance value;
    };

    struct ComputedStyle::Bottom : public ComputedStyle
    {
        inline static StyleIdentifier Identifier = StyleIdentifier::Bottom;
        Bottom(css::distance value) : value(std::move(value)) {}
        css::distance value;
    };

    class ComputedStyle::Width : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Width;
        Width(css::distance value) : _value(std::move(value)) {}

        css::distance const& value() const { return _value; }

    private:
        css::distance _value;
    };

    class ComputedStyle::Height : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Height;
        Height(css::distance value) : _value(std::move(value)) {}

        css::distance const& value() const { return _value; }
    private:
        css::distance _value;
    };

    class ComputedStyle::FontSize : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::FontSize;
        FontSize(css::font_size value) : _value(std::move(value)) {}

        css::font_size const& value() const { return _value; }
    private:
        css::font_size _value;
    };

    class ComputedStyle::FontFamily : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Height;
        FontFamily(css::font_family value) : _value(std::move(value)) {}

        css::font_family const& value() const { return _value; }
    private:
        css::font_family _value;
    };

    class ComputedStyle::FontStyle : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Height;
        FontStyle(css::font_style value) : _value(std::move(value)) {}

        css::font_style const& value() const { return _value; }
    private:
        css::font_style _value;
    };

    class ComputedStyle::Color : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Height;
        Color(css::color value) : _value(std::move(value)) {}

        css::color const& value() const { return _value; }
    private:
        css::color _value;
    };

    class ComputedStyle::BackgroundColor : public ComputedStyle
    {
    public:
        inline static StyleIdentifier Identifier = StyleIdentifier::Height;
        BackgroundColor(css::background_color value) : _value(std::move(value)) {}

        css::background_color const& value() const { return _value; }
    private:
        css::background_color _value;
    };
}
