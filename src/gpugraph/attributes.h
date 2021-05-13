#pragma once
#include <variant>


namespace gpugraph
{

    //
    // box-sizing

    struct ContentBox {};
    struct BorderBox {};

    inline auto const content_box = ContentBox();
    inline auto const border_box = BorderBox();

    //
    // width

    struct Automatic {};
    struct MinWidth {};
    struct MaxWidth {};

    struct Pixels { struct Suffix {}; float value; };
    struct Percentage { struct Suffix {}; float value; };
    struct Em { struct Suffix {}; float value; };
    struct Rem { struct Suffix {}; float value; };

    inline static Automatic const auto_ = Automatic{};
    inline static Pixels::Suffix const px = Pixels::Suffix{};
    inline static Percentage::Suffix const percent = Percentage::Suffix{};

    using Sizing = std::variant<Automatic, MinWidth, MaxWidth, Pixels, Percentage, Em, Rem>;

    template<typename T>
    Sizing operator |(T value, Pixels::Suffix const&)
    {
        return Pixels{ static_cast<float>(value) };
    }

    template<typename T>
    Sizing operator |(T value, Percentage::Suffix const&)
    {
        return Percentage{ static_cast<float>(value) };
    }

    template<typename T>
    Sizing operator |(T value, Em::Suffix const&)
    {
        return Em{ static_cast<float>(value) };
    }

    struct Flex
    {
        std::tuple<std::variant<Automatic, Sizing>> value;
    };

}
