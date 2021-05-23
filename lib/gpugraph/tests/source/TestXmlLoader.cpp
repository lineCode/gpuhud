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
#include <gpugraph/XmlLoader.h>

TEST(XmlLoader, test_tree_parse)
{
    std::string input = R"~(
        <div class="xyz">
            <div id="a"></div>
        </div>
    )~";
    auto graph = gpugraph::XmlLoader::load(input);
    ASSERT_NE(graph, nullptr);
    ASSERT_TRUE(graph->class_set().value().count("xyz") > 0);
    ASSERT_EQ(graph->size(), 1);
    ASSERT_EQ(graph->at(0)->attribute("id").value(), "a");
}

TEST(XmlLoader, test_whitespace_gets_trimmed_in_class_attribute)
{
    std::string input = R"~(
        <div class=" aaa bbb  cccc">
        </div>
    )~";
    auto graph = gpugraph::XmlLoader::load(input);
    ASSERT_NE(graph, nullptr);
    ASSERT_EQ(graph->class_set().value().size(), 3);
}
