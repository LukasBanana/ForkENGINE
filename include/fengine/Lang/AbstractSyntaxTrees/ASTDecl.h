/*
 * AST declaration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AST_DECL_H__
#define __FORK_AST_DECL_H__


#include "Core/DeclPtr.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/* --- Common AST nodes --- */

DECL_SHR_PTR(AST     );
DECL_SHR_PTR(Visitor );


/* --- ForkSCript AST nodes --- */

DECL_SHR_PTR(FSCStructNameIdent);


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================