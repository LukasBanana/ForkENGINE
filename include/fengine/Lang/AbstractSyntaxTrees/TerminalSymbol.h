/*
 * AST terminal header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AST_TERMINAL_SYMBOL_H__
#define __FORK_AST_TERMINAL_SYMBOL_H__


#include "Lang/AbstractSyntaxTrees/AST.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


//! Base class for all AST terminal symbols.
class FORK_EXPORT TerminalSymbol : public AST
{
    
    public:
        
        TerminalSymbol(const SyntaxAnalyzer::SourcePosition& sourcePos, const std::string& spelling) :
            AST     { sourcePos },
            spell   { spelling  }
        {
        }
        ~TerminalSymbol()
        {
        }

        std::string spell; //!< Terminal symbol spelling.

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================