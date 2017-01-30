/*
 * Source-string header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SOURCE_STRING_H__
#define __FORK_SYNTAX_ANALYZER_SOURCE_STRING_H__


#include "Lang/SyntaxAnalyzer/SourceCode.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


class FORK_EXPORT SourceString : public SourceCode
{
    
    public:
        
        SourceString(const std::string& sourceCode);

        char Next();

    private:
        
        std::string sourceCode_;
        size_t pos_ = 0;

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================