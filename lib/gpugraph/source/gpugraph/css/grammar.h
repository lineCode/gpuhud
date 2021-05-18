#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/std_tuple.hpp>
// #include <boost/fusion/include/std_vector.hpp>

#include "length.h"

namespace x3 = boost::spirit::x3;

namespace gpugraph::css::grammar
{

    using x3::eps;
    using x3::lit;
    using x3::_val;
    using x3::_attr;
    using x3::float_;
    using x3::char_;

    struct length_unit_ : x3::symbols<Length::Unit>
    {
        length_unit_()
        {
            add
                ("px", Length::Unit::Pixel)
                ("mm", Length::Unit::Millimeter)
                ("cm", Length::Unit::Centimeter)
                ("in", Length::Unit::Inch)
                ("pc", Length::Unit::Pica)
                ("pt", Length::Unit::Point)
                ("vw", Length::Unit::ViewportWidth)
                ("vh", Length::Unit::ViewportHeight)
                ("vmin", Length::Unit::ViewportMin)
                ("vmax", Length::Unit::ViewportMax)
                ("em", Length::Unit::FontSize)
                ("rem", Length::Unit::RootFontSize)
                ("ex", Length::Unit::HeightOfLowerLetterX)
                ("ch", Length::Unit::CharacterHorizontal)
                ;
        }

    } length_unit;

    auto const length = x3::rule<class length, std::tuple<float, Length::Unit>>() =  (float_ >> length_unit);
    auto const identifier = x3::lexeme[(x3::alpha | x3::char_('_') | x3::char_('-')) >> *(x3::alnum | x3::char_('_') | x3::char_('-'))];

}