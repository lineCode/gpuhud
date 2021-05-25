/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include <gtest/gtest.h>
#include <gpugraph/Node.h>
#include <gpugraph/StyleCollection.h>

namespace gpugraph
{

    TEST(TestStyling, test_styling_is_compiled)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div {
                left: 3px;
            }
        )~");
        auto node = Node::create("div");
        node->set_style_collection(style_collection);
        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_FALSE(node->computed_style_set().contains<ComputedStyle::Right>());
        ASSERT_TRUE(std::holds_alternative<css::length>(node->computed_style_set().get<ComputedStyle::Left>()->value));
        ASSERT_EQ(std::get<css::length>(node->computed_style_set().get<ComputedStyle::Left>()->value), 3 | css::length_unit::px);
    }

    TEST(TestStyling, test_initial_values)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div {
                position: initial;
                left: initial;
            }
        )~");
        auto node = Node::create("div");
        node->set_style_collection(style_collection);
        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_TRUE(std::holds_alternative<css::auto_>(node->computed_style_set().get<ComputedStyle::Left>()->value));

        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Position>());
        ASSERT_EQ(node->computed_style_set().get<ComputedStyle::Position>()->value, css::position::static_);
    }

    TEST(TestStyling, test_inherit_defaults_to_initial)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div {
                position: inherit;
                left: inherit;
            }
        )~");
        auto node = Node::create("div");
        node->set_style_collection(style_collection);
        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_EQ(ComputedStyleSet::Initial.get<ComputedStyle::Left>(), node->computed_style_set().get<ComputedStyle::Left>());

        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Position>());
        ASSERT_EQ(node->computed_style_set().get<ComputedStyle::Position>()->value, css::position::static_);
    }

    TEST(TestStyling, can_style_inner_div_with_greater_selector)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div > div{
                left: 3px;
            }
        )~");
        auto node = Node::create("div");
        auto child = Node::create("div");
        node->add(child);
        node->set_style_collection(style_collection);
        ASSERT_FALSE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_TRUE(child->computed_style_set().contains<ComputedStyle::Left>());
    }

    TEST(TestStyling, can_unset_with_cascade_rule)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div > div{
                left: 3px;
            }
            div > div.a{
                left: unset;
            }
        )~");
        auto node = Node::create("div");
        auto child = Node::create("div");
        child->add_class("a");
        node->add(child);
        node->set_style_collection(style_collection);
        ASSERT_FALSE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_FALSE(child->computed_style_set().contains<ComputedStyle::Left>());
    }

    TEST(TestStyling, can_inherit_with_cascade_rule)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div.a{
                left: 3px;
            }
            div > div{
                left: inherit;
            }
        )~");
        auto node = Node::create("div");
        auto child = Node::create("div");
        node->add_class("a");
        node->add(child);
        node->set_style_collection(style_collection);
        ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Left>());
        ASSERT_TRUE(child->computed_style_set().contains<ComputedStyle::Left>());
    }

    TEST(TestStyling, can_override_styles_with_style_attribute)
    {
        auto style_collection = std::make_shared<StyleCollection>();
        style_collection->compile(R"~(
            div{
                width: auto;
                height: auto;
            }
        )~");
        auto node = Node::create("div");
        node->set_style_collection(style_collection);
        node->set_style("width: 10px; height: 11px");
        {
            ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Width>());
            auto& value = node->computed_style_set().get<ComputedStyle::Width>()->value();
            ASSERT_TRUE(std::holds_alternative<css::length>(value));
            ASSERT_EQ(std::get<css::length>(value), 10 | css::length_unit::px);
        }
        {
            ASSERT_TRUE(node->computed_style_set().contains<ComputedStyle::Height>());
            auto& value = node->computed_style_set().get<ComputedStyle::Height>()->value();
            ASSERT_TRUE(std::holds_alternative<css::length>(value));
            ASSERT_EQ(std::get<css::length>(value), 11 | css::length_unit::px);
        }
    }

}
