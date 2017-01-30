/*
 * XML scanner header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XML_SCANNER_H__
#define __FORK_XML_SCANNER_H__


#include "Lang/SyntaxAnalyzer/Scanner.h"


namespace Fork
{

namespace Lang
{


//! XML scanner (or lexial analyzer).
class XMLScanner : public SyntaxAnalyzer::Scanner
{
    
    public:
        
        SyntaxAnalyzer::TokenPtr Next();

    private:
        
        void IgnoreMultiLineComment();

        SyntaxAnalyzer::TokenPtr ScanToken();

};


} // /namespace Lang

} // /namespace Fork


#endif



// ========================