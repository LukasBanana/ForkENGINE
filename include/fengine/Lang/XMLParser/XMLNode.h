/*
 * XML node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_NODE_H__
#define __FORK_XML_NODE_H__


#include "Lang/AbstractSyntaxTrees/AST.h"
#include "Lang/AbstractSyntaxTrees/Visitor.h"
#include "Lang/XMLParser/XMLDecl.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/**
XML node AST base class.
\ingroup xml_parser
*/
class FORK_EXPORT XMLNode : public AST
{
    
    public:
        
        //! XML node types.
        enum class Types
        {
            Tag,    //!< XML tag.
            Content //!< XML content.
        };

        XMLNode(const SyntaxAnalyzer::SourcePosition& sourcePos) :
            AST{ sourcePos }
        {
        }
        virtual ~XMLNode()
        {
        }

        virtual Types Type() const = 0;

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================