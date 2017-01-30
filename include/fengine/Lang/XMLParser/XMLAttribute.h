/*
 * XML attribute header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_ATTRIBUTE_H__
#define __FORK_XML_ATTRIBUTE_H__


#include "Lang/XMLParser/XMLNode.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/**
XML attribute AST class.
\ingroup xml_parser
*/
class FORK_EXPORT XMLAttribute : public AST
{
    
    public:
        
        XMLAttribute(const SyntaxAnalyzer::SourcePosition& sourcePos, const std::string& ident, const std::string& value) :
            AST     { sourcePos },
            ident   { ident     },
            value   { value     }
        {
        }

        DEF_AST_VISIT_PROC(XMLAttribute);

        std::string ident; //!< Attribute identifier.
        std::string value; //!< Attribute value.

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================