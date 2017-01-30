/*
 * Source-string file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/SourceString.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


SourceString::SourceString(const std::string& sourceCode) :
    sourceCode_(sourceCode)
{
}

char SourceString::Next()
{
    /* Check if end-of-source is reached */
    if (pos_ >= sourceCode_.size())
        return 0;

    /* Get current character and increment reading position */
    char chr = sourceCode_[pos_++];

    /* Check for new-line character */
    if (chr == '\n')
        sourcePos_.IncRow();

    sourcePos_.IncColumn();

    return chr;
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================