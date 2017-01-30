/*
 * FSC scanner file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FSCScanner.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Lang
{


using namespace SyntaxAnalyzer;

FSCScanner::FSCScanner() :
    Scanner()
{
    EstablishKeywords();
}
FSCScanner::~FSCScanner()
{
}

TokenPtr FSCScanner::Next()
{
    try
    {
        bool noCommentary = false;

        do
        {
            IgnoreWhiteSpaces();

            /* Check for end-of-file character */
            if (Chr() == 0)
                return MakeToken(Token::Types::EndOfFile);

            /* Scan commentaries */
            switch (Chr())
            {
                case '/':
                {
                    TakeIt();

                    if (Chr() == '/')
                        IgnoreLine();
                    else if (Chr() == '*')
                        IgnoreMultiLineComment();
                    else
                        return MakeToken(Token::Types::ProductOp, "/", false);
                }
                break;

                default:
                {
                    noCommentary = true;
                }
                break;
            }
        }
        while (!noCommentary);

        /* Scan next token */
        return ScanToken();
    }
    catch (const IO::Error& err)
    {
        IO::Log::Error(err);
    }

    return nullptr;
}


/*
 * ======= Private: =======
 */

void FSCScanner::EstablishKeywords()
{
    /* Establish all FSC keywords */
    keywords_["if"      ] = Token::Types::Keyword;
    keywords_["elif"    ] = Token::Types::Keyword;
    keywords_["else"    ] = Token::Types::Keyword;
    keywords_["include" ] = Token::Types::Keyword;
    keywords_["for"     ] = Token::Types::Keyword;
    keywords_["var"     ] = Token::Types::Keyword;
    keywords_["new"     ] = Token::Types::Keyword;
    keywords_["not"     ] = Token::Types::Keyword;
}

void FSCScanner::IgnoreMultiLineComment()
{
    while (true)
    {
        if (Chr() == 0)
            return;

        /* Scan comment block ending */
        if (Chr() == '*')
        {
            TakeIt();
            if (Chr() == '/')
            {
                TakeIt();
                return;
            }
        }
        TakeIt();
    }
}

TokenPtr FSCScanner::ScanToken()
{
    /* Scan string literal */
    if (Chr() == '\"')
        return ScanString();

    /* Scan identifier */
    if (IsLetter() || Chr() == '_')
        return ScanIdentifierAndCheck();

    /* Scan number */
    if (IsDecDigit())
        return ScanNumber(true);

    /* Scan special character and verbatim string literals */
    if (Chr() == '@')
    {
        TakeIt();

        /* Scan verbatim string literal */
        if (Chr() == '\"')
            return ScanVerbatimString();

        return MakeToken(Token::Types::At);
    }
    
    /* Scan punctuation, special characters, operators and brackets */
    if (Chr() == '.')
    {
        TakeIt();
        if (Chr() == '.')
            return MakeToken(Token::Types::RangeSep, true);
        return MakeToken(Token::Types::Dot);
    }

    switch (Chr())
    {
        case '=': return MakeToken(Token::Types::EqualityOp,    true); break;
        case '+': return MakeToken(Token::Types::SumOp,         true); break;
        case '-': return MakeToken(Token::Types::SumOp,         true); break;
        case '*': return MakeToken(Token::Types::ProductOp,     true); break;
        case '/': return MakeToken(Token::Types::ProductOp,     true); break;
        case '%': return MakeToken(Token::Types::ProductOp,     true); break;
        case ':': return MakeToken(Token::Types::Colon,         true); break;
        case ';': return MakeToken(Token::Types::Semicolon,     true); break;
        case ',': return MakeToken(Token::Types::Comma,         true); break;
        case '(': return MakeToken(Token::Types::LBracket,      true); break;
        case ')': return MakeToken(Token::Types::RBracket,      true); break;
        case '{': return MakeToken(Token::Types::LCurly,        true); break;
        case '}': return MakeToken(Token::Types::RCurly,        true); break;
        case '[': return MakeToken(Token::Types::LParen,        true); break;
        case ']': return MakeToken(Token::Types::RParen,        true); break;
    }

    /* No suitable token found -> quit with error */
    ErrorUnexpected();

    return nullptr;
}

TokenPtr FSCScanner::ScanIdentifierAndCheck()
{
    /* Scan identifier */
    auto spell = ScanIdentifier();

    /* Check for reserved literal identifiers */
    if (spell == "true" || spell == "false")
        return MakeToken(Token::Types::BoolLiteral, spell, false);
    else if (spell == "null")
        return MakeToken(Token::Types::PointerLiteral, spell, false);

    /* Check for FSC keywords */
    auto it = keywords_.find(spell);

    if (it != keywords_.end())
        return MakeToken(it->second, spell, false);

    /* Create identifier token */
    return MakeToken(Token::Types::Identifier, spell, false);
}


} // /namespace Lang

} // /namespace Fork



// ========================