/*
 * AST Visitor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AST_VISITOR_H__
#define __FORK_AST_VISITOR_H__


#include "Core/Export.h"
#include "Lang/XMLParser/XMLDecl.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


#define DEF_DUMMY_VISIT_PROC(n)                             \
    virtual void Visit##n(n* node, void* args = nullptr)    \
    {                                                       \
        /* Dummy */                                         \
    }


/**
AST (Abstract Syntax Tree) visitor interface.
\todo REMOVE THIS!
*/
class FORK_EXPORT Visitor
{
    
    public:
        
        virtual ~Visitor()
        {
        }

        /* === XML AST nodes === */

        DEF_DUMMY_VISIT_PROC( XMLTag       )
        DEF_DUMMY_VISIT_PROC( XMLAttribute )
        DEF_DUMMY_VISIT_PROC( XMLContent   )

    protected:
        
        Visitor() = default;

};

#undef DEF_DUMMY_VISIT_PROC


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================