/*
 * XML tag header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_TAG_H__
#define __FORK_XML_TAG_H__


#include "Lang/XMLParser/XMLNode.h"
#include "Lang/XMLParser/XMLAttribute.h"
#include "Lang/XMLParser/XMLContent.h"

#include <vector>


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/**
XML tag AST class.
\ingroup xml_parser
*/
class FORK_EXPORT XMLTag : public XMLNode
{
    
    public:
        
        XMLTag(const SyntaxAnalyzer::SourcePosition& sourcePos, const std::string& ident) :
            XMLNode { sourcePos },
            ident   { ident     }
        {
        }

        DEF_AST_VISIT_PROC(XMLTag);

        Types Type() const
        {
            return Types::Tag;
        }

        std::string ident;                          //!< Tag identifier.
        std::vector<XMLAttributePtr> attributes;    //!< Attribute list.

        std::vector<XMLNodePtr> nodes;              //!< Sub AST nodes (either XMLContentPtr or XMLTagPtr).

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================