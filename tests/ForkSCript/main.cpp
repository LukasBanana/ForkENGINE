
// ForkENGINE: ForkSCript Test
// 28/02/2014

#include <fengine/core.h>
#include <fengine/using.h>
#include <iostream>

using namespace std;
using namespace Fork;

int main()
{
    // FSC interpreter
    Lang::FSCInterpreter interpreter;
    interpreter.RunScriptFromFile("FSCTestScript.fsc");

    auto var = interpreter.Fetch("mat1");

    // XML parser
    Lang::XMLParser xmlParser;
    auto xmlTags = xmlParser.ParseXMLFile("TestXML.xml");

    // XML writer
    std::ofstream outFile("TestXMLOut.xml");
    Lang::XMLWriter::WriteXMLTagsToFile(outFile, xmlTags);

    using Lang::AbstractSyntaxTrees::XMLTagPtr;
    using Lang::AbstractSyntaxTrees::XMLAttributePtr;
    using Lang::AbstractSyntaxTrees::XMLContentPtr;

    class XMLVisitor : public Lang::AbstractSyntaxTrees::Visitor
    {
        
        public:
            
            DeclVisitProc(XMLTag)
            {
                IO::Log::Message("XML Tag \"" + ast->ident + "\"");
                IO::Log::ScopedIndent indent;

                for (auto attrib : ast->attributes)
                    attrib->Visit(this);
                for (auto node : ast->nodes)
                    node->Visit(this);
            }

            DeclVisitProc(XMLAttribute)
            {
                IO::Log::Message("XML Attribute \"" + ast->ident + "\", value: \"" + ast->value + "\"");
            }

            DeclVisitProc(XMLContent)
            {
                IO::Log::Message("XML Content \"" + ast->value + "\"");
            }

    };

    XMLVisitor xmlVisitor;
    
    for (auto tag : xmlTags)
        tag->Visit(&xmlVisitor);
    
    IO::Console::Wait();

    return 0;
}

