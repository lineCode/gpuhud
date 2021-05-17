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
    ASSERT_TRUE(graph->class_set().count("xyz") > 0);
    ASSERT_EQ(graph->size(), 1);
    ASSERT_EQ(graph->at(0)->id(), "a");
}

TEST(XmlLoader, test_whitespace_gets_trimmed_in_class_attribute)
{
    std::string input = R"~(
        <div class=" aaa bbb  cccc">
        </div>
    )~";
    auto graph = gpugraph::XmlLoader::load(input);
    ASSERT_NE(graph, nullptr);
    ASSERT_EQ(graph->class_set().size(), 3);
}
