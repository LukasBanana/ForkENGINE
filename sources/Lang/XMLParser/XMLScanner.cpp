/*
 * XML scanner file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XMLScanner.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Lang
{


using namespace SyntaxAnalyzer;

TokenPtr XMLScanner::Next()
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
                case '<':
                {
                    TakeIt();
                    if (Chr() == '!')
                    {
                        TakeIt();
                        if (Chr() == '-')
                        {
                            TakeIt();
                            if (Chr() == '-')
                            {
                                TakeIt();
                                IgnoreMultiLineComment();
                            }
                            else
                                return MakeToken(Token::Types::SumOp, "-", false);
                        }
                        else
                            return MakeToken(Token::Types::NegationOp, "!", false);
                    }
                    else if (Chr() == '/')
                    {
                        TakeIt();
                        return MakeToken(Token::Types::RelationOp, "</", false);
                    }
                    else
                        return MakeToken(Token::Types::RelationOp, "<", false);
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

void XMLScanner::IgnoreMultiLineComment()
{
    while (true)
    {
        if (Chr() == 0)
            return;

        /* Scan comment block ending */
        if (Chr() == '-')
        {
            TakeIt();
            if (Chr() == '-')
            {
                TakeIt();
                if (Chr() == '>')
                {
                    TakeIt();
                    return;
                }
            }
        }
        TakeIt();
    }
}

TokenPtr XMLScanner::ScanToken()
{
    /* Scan string literal */
    if (Chr() == '\"')
        return ScanString();

    /* Scan identifier */
    if (IsLetter() || Chr() == '_')
    {
        auto spell = ScanIdentifier();
        return MakeToken(Token::Types::Identifier, spell, false);
    }

    /* Scan number */
    if (IsDecDigit())
        return ScanNumber(true);

    /* Scan XML special character */
    if (Chr() == '>')
    {
        TakeIt();
        return MakeToken(Token::Types::RelationOp, ">", false);
    }

    if (Chr() == '/')
    {
        TakeIt();
        if (Chr() == '>')
        {
            TakeIt();
            return MakeToken(Token::Types::RelationOp, "/>", false);
        }
        return MakeToken(Token::Types::ProductOp, "/", false);
    }

    if (Chr() == ':')
        return MakeToken(Token::Types::Colon, true);
    if (Chr() == '-')
        return MakeToken(Token::Types::SumOp, true);
    if (Chr() == '=')
        return MakeToken(Token::Types::EqualityOp, true);

    if (Chr() == '&')
    {
        std::string spell;

        while (Chr() != ';')
            spell += TakeIt();
        spell += TakeIt();

        return MakeToken(Token::Types::Keyword, spell, false);
    }

    /* Scan other characters */
    return MakeToken(Token::Types::Other, true);
}


} // /namespace Lang

} // /namespace Fork



// ========================