/*
 * AST (FSC) struct name identifier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AST_FSC_STRUCT_NAME_IDENT_H__
#define __FORK_AST_FSC_STRUCT_NAME_IDENT_H__


#include "Lang/AbstractSyntaxTrees/AST.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


class FORK_EXPORT FSCStructNameIdent : public AST
{
    
    public:
        
        FSCStructNameIdent(const SyntaxAnalyzer::SourcePosition& sourcePos, const std::string& identName) :
            AST     (sourcePos),
            ident   (identName)
        {
        }
        ~FSCStructNameIdent()
        {
        }

        /**
        Identifier name.
        \note For the FSC we use an abbreviated form and don't save every AST node as separate object.
        */
        std::string ident;

        FSCStructNameIdentPtr next; //!< Optional next struct-name identifier.

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================