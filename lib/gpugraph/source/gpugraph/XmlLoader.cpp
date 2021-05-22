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
#include "XmlLoader.h"

#include <tinyxml2.h>
#include <list>
#include <iostream>

#include "Node.h"
#include "string_utils.h"
#include "log.h"

namespace gpugraph
{

    namespace
    {

        class Builder :public tinyxml2::XMLVisitor
        {
        public:
            bool VisitEnter(const tinyxml2::XMLDocument& document) override
            {
                return true;
            }

            bool VisitExit(const tinyxml2::XMLDocument&) override
            {
                return true;
            }

            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override
            {
                auto node = gpugraph::Node::create(element.Name());
                log_debug("created node \"" << element.Name() << "\"");
                if (_root_node)
                    _stack.back()->add(node);
                else
                    _root_node = node;
                _stack.push_back(node);
                while (attribute)
                {
                    std::string name = attribute->Name();
                    if (name == "class")
                    {
                        std::string value = attribute->Value();
                        for (auto& class_ : tokenize(value, "\\s+"))
                            node->add_class(std::move(class_));
                    }
                    else if (name == "id")
                    {
                        node->set_id(attribute->Value());
                    }
                    attribute = attribute->Next();
                }
                return true;
            }

            bool VisitExit(const tinyxml2::XMLElement& element) override
            {
                _stack.pop_back();
                return true;
            }

            virtual bool Visit(const tinyxml2::XMLDeclaration& declaration)
            {
                // ?
                return true;
            }

            virtual bool Visit(const tinyxml2::XMLText& text)
            {
                _stack.back()->set_text_content(text.Value());
                return true;
            }

            bool Visit(const tinyxml2::XMLComment&) override
            {
                return true;
            }

            bool Visit(const tinyxml2::XMLUnknown&) override
            {
                return true;
            }

            std::shared_ptr<Node> root_node() const
            {
                return _root_node;
            }

        private:
            std::shared_ptr<Node> _root_node;
            std::list < std::shared_ptr<Node> >_stack;
        };
    }

    std::shared_ptr<Node> XmlLoader::load(std::istream& stream)
    {
        std::string buffer((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
        return load(buffer);
    }

    std::shared_ptr<Node> XmlLoader::load(std::string const& buffer)
    {
        tinyxml2::XMLDocument document;
        document.Parse(buffer.c_str(), buffer.size());
        Builder builder;
        document.Accept(&builder);
        return builder.root_node();
    }

}
