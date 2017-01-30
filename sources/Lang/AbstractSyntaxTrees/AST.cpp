/*
 * AST file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/AbstractSyntaxTrees/AST.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Lang
{

namespace AbstractSyntaxTrees
{


AST::AST(const SyntaxAnalyzer::SourcePosition& sourcePos) :
    sourcePos_(sourcePos)
{
}
AST::~AST()
{
}

void AST::Print(const std::string& text)
{
    IO::Log::Debug(text);
}


} // /namespace AbstractSyntaxTrees

} // /namespace Lang

} // /namespace Fork



// ========================