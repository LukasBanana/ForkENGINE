/*
 * FSC scanner header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LANG_FSC_SCANNER_H__
#define __FORK_LANG_FSC_SCANNER_H__


#include "Lang/SyntaxAnalyzer/Scanner.h"

#include <map>


namespace Fork
{

namespace Lang
{


class FSCScanner : public SyntaxAnalyzer::Scanner
{
    
    public:
        
        FSCScanner();
        ~FSCScanner();

        SyntaxAnalyzer::TokenPtr Next();

    private:
        
        /* === Functions === */

        void EstablishKeywords();

        void IgnoreMultiLineComment();

        SyntaxAnalyzer::TokenPtr ScanToken();
        SyntaxAnalyzer::TokenPtr ScanIdentifierAndCheck();

        /* === Members === */

        std::map<std::string, SyntaxAnalyzer::Token::Types> keywords_;

};


} // /namespace Lang

} // /namespace Fork


#endif



// ========================