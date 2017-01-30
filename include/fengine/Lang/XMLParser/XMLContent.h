/*
 * XML content header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_CONTENT_H__
#define __FORK_XML_CONTENT_H__


#include "Lang/XMLParser/XMLNode.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/**
XML content AST class.
\ingroup xml_parser
*/
class FORK_EXPORT XMLContent : public XMLNode
{
    
    public:
        
        XMLContent(const SyntaxAnalyzer::SourcePosition& sourcePos, const std::string& value) :
            XMLNode { sourcePos },
            value   { value     }
        {
        }

        DEF_AST_VISIT_PROC(XMLContent);

        Types Type() const
        {
            return Types::Content;
        }

        std::string value; //!< Content value.

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================