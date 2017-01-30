/*
 * XML declaration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_DECL_H__
#define __FORK_XML_DECL_H__


#include "Core/DeclPtr.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


/* --- XML AST nodes --- */

DECL_SHR_PTR(XMLTag      );
DECL_SHR_PTR(XMLAttribute);
DECL_SHR_PTR(XMLContent  );
DECL_SHR_PTR(XMLNode     );


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork


#endif



// ========================