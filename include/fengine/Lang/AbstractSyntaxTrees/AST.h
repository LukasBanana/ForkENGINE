/*
 * AST header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AST_H__
#define __FORK_AST_H__


#include "Visitor.h"
#include "Lang/AbstractSyntaxTrees/ASTDecl.h"
#include "Lang/SyntaxAnalyzer/SourcePosition.h"
#include "Core/Export.h"

#include <memory>
#include <functional>


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


#define DEF_AST_VISIT_PROC(n)                       \
    void Visit(Visitor* vst, void* args = nullptr)  \
    {                                               \
        vst->Visit##n(this, args);                  \
    }


//! AST (Abstract Syntax Tree) base class.
class FORK_EXPORT AST
{
    
    public:
        
        virtual ~AST();

        /**
        Virutal visitor function.
        \param[in] vst Specifies the AST visitor raw pointer.
        */
        virtual void Visit(Visitor* vst, void* args = nullptr)
        {
            /* Dummy */
        }

        //! Views debugging information with recursive node calls.
        virtual void PrintAST()
        {
            /* Dummy */
        }

        //! Returns the source position of this AST node.
        inline const SyntaxAnalyzer::SourcePosition& Pos() const
        {
            return sourcePos_;
        }

    protected:
        
        AST(const SyntaxAnalyzer::SourcePosition& sourcePos);

        void Print(const std::string& text);

    private:

        SyntaxAnalyzer::SourcePosition sourcePos_;

};


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================