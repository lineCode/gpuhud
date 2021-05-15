#include "XmlLoader.h"

#include <tinyxml2.h>
#include <list>
#include <iostream>

#include "Node.h"
#include "string_utils.h"

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
                auto node = gpugraph::Node::create();
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
                    else if(name == "id")
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
