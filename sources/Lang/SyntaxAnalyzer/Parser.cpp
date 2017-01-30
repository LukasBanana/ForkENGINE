/*
 * Parser file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/Parser.h"
#include "Lang/SyntaxAnalyzer/ScriptError.h"
#include "Core/StringModifier.h"

#include <algorithm>


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


Parser::Parser(const ScannerPtr& scanner) :
    scanner_(scanner)
{
}
Parser::~Parser()
{
}


/*
 * ======= Protected: ========
 */

TokenPtr Parser::AcceptIt()
{
    auto prevTkn = tkn_;

    /* Scan next token and check for error */
    tkn_ = scanner_->Next();

    /* Check if end-of-file has been reached */
    if (!tkn_)
        Error("Scanning token failed");

    return prevTkn;
}

TokenPtr Parser::Accept(const Token::Types& type)
{
    /* Accept token on type match */
    if (tkn_ && TokenType() == type)
        return AcceptIt();
    
    /* Exit with error */
    ErrorUnexpected(type);

    return nullptr;
}

TokenPtr Parser::Accept(const Token::Types& type, const std::string& spell)
{
    /* Accept token on type- and spelling match */
    if (tkn_ && TokenType() == type && tkn_->Spell() == spell)
        return AcceptIt();
    
    /* Exit with error */
    ErrorUnexpected("'" + Token::Spell(type) + "' with spelling '" + spell + "'");

    return nullptr;
}

/* --- Error handling --- */

void Parser::Error(const std::string& message, bool appendLine, TokenPtr tkn)
{
    if (!tkn)
        tkn = tkn_;

    if (appendLine && tkn && TokenType() != Token::Types::EndOfFile)
    {
        /* Setup error message and marker */
        const std::string& line = scanner_->Source()->Line();
        const std::string marker = BuildTokenErrorMarker(line, *tkn);

        /* Throw the syntax error */
        throw ScriptError(
            IO::ErrorTypes::Syntax, scanner_->Pos(), message,
            BuildTabTruncatedLine(line), marker
        );
    }
    else
    {
        /* Throw the syntax error */
        throw ScriptError(IO::ErrorTypes::Syntax, scanner_->Pos(), message);
    }
}

void Parser::ErrorUnexpected(const std::string& desc, bool appendExpectedStr)
{
    /* Setup error message */
    std::string msg = "Unexpected token '" + ToStr(tkn_ ? tkn_->Spell() : "<unknown>");
    msg += (appendExpectedStr ? "' (Expected " : "' (") + desc + ")";

    Error(msg, true);
}

void Parser::ErrorUnexpected(const Token::Types &Type)
{
    ErrorUnexpected("'" + Token::Spell(Type) + "'");
}

void Parser::ErrorUnexpected()
{
    /* Setup error message and marker */
    const auto& line = scanner_->Source()->Line();
    const auto marker = BuildTokenErrorMarker(line, *tkn_);

    const std::string msg = "Unexpected token '" + ToStr(tkn_ ? tkn_->Spell() : "<unknown>") + "'";

    /* Throw the syntax error */
    throw ScriptError(
        IO::ErrorTypes::Syntax, scanner_->Pos(),
        msg, BuildTabTruncatedLine(line), marker
    );
}

void Parser::ErrorEOF()
{
    Error("Unexpected <end-of-file> token");
}

std::string Parser::BuildTokenErrorMarker(const std::string& line, const Token& tkn) const
{
    /* Setup error marker */
    size_t from = 0, to = 0;

    to = tkn.Pos().Column();
    if (to > 0)
        --to;

    const size_t len = tkn.Spell().size();

    if (to > len)
        from = to - len;

    /* Convert tabs to spaces */
    const size_t num = std::min(from, line.size());

    for (size_t i = 0; i < num; ++i)
    {
        if (line[i] == '\t')
            from += 3;
    }

    /* Return final marker */
    return std::string(from, ' ') + std::string(len, '~');
}

std::string Parser::BuildTabTruncatedLine(const std::string& line) const
{
    auto result = line;
    ReplaceString(result, "\t", std::string(4, ' '));
    return result;
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================