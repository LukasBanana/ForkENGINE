/*
 * FSC interpreter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/FSCInterpreter/FSCInterpreter.h"
#include "Lang/SyntaxAnalyzer/SourceString.h"
#include "FSCScanner.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Lang
{


using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;

FSCInterpreter::FSCInterpreter() :
    Parser(std::make_shared<FSCScanner>())
{
}
FSCInterpreter::~FSCInterpreter()
{
}

bool FSCInterpreter::RunScriptFromFile(const std::string& filename)
{
    IO::Log::Message("Run ForkSCript: \"" + filename + "\"");
    IO::Log::ScopedIndent indent;

    try
    {
        /* Open initial file and begin with parsing */
        ReadSourceFile(filename);
        AcceptIt();

        /* Parse script */
        ParseScript();
    }
    catch (const ScriptError& err)
    {
        IO::Log::Error(err);
        return false;
    }
    catch (const IO::Error& err)
    {
        IO::Log::Error(err);
        return false;
    }
    catch (const std::string& err)
    {
        IO::Log::Error(err);
        return false;
    }

    return true;
}

bool FSCInterpreter::RunScript(const std::string& sourceCode)
{
    IO::Log::Message("Run ForkSCript");
    IO::Log::ScopedIndent indent;

    try
    {
        /* Create source code object */
        auto sourceString = std::make_shared<SourceString>(sourceCode);

        /* Start scanning source */
        if (!scanner_->ScanSource(sourceString))
            throw IO::Error(IO::ErrorTypes::Default, "Scanning source failed");

        /* Begin with parsing */
        AcceptIt();

        /* Parse script */
        ParseScript();
    }
    catch (const ScriptError& err)
    {
        IO::Log::Error(err);
        return false;
    }
    catch (const IO::Error& err)
    {
        IO::Log::Error(err);
        return false;
    }
    catch (const std::string& err)
    {
        IO::Log::Error(err);
        return false;
    }

    return true;
}

VarObjectPtr FSCInterpreter::Fetch(const std::string& varName)
{
    /* Search variable object only in global scope */
    VarObjectPtr varObject;
    return globalScope_->Fetch(varName, varObject) ? varObject : nullptr;
}


/* 
 * ======= Private: =======
 */

void FSCInterpreter::ReadSourceFile(const std::string& filename)
{
    /* Open file for reading */
    sourceFile_ = SourceFile::Open(filename);
    if (!sourceFile_)
        throw IO::Error(IO::ErrorTypes::FileNotFound, "Reading file \"" + filename + "\" failed");

    /* Start scanning source */
    if (!scanner_->ScanSource(sourceFile_))
        throw IO::Error(IO::ErrorTypes::Default, "Scanning source failed");
}

/* --- Parsing functions --- */

void FSCInterpreter::ParseScript()
{
    ParseStatementList();
}

void FSCInterpreter::ParseStatementList()
{
    scopeMngr_.PushScope();
    {
        /* Store global scope reference */
        if (scopeMngr_.ScopeDepth() == 1)
            globalScope_ = scopeMngr_.TopScope();

        /* Parse all single statements */
        while (TokenType() != Token::Types::EndOfFile && TokenType() != Token::Types::RCurly)
            ParseSingleStatement();
    }
    scopeMngr_.PopScope();
}

void FSCInterpreter::ParseSingleStatement()
{
    switch (TokenType())
    {
        case Token::Types::Identifier:
            ParseStructNameStmnt();
            break;
        case Token::Types::Keyword:
            ParseSingleStmntKeyword();
            break;
        case Token::Types::RCurly:
            break;
        default:
            ErrorUnexpected("<statement>");
            break;
    }
}

void FSCInterpreter::ParseSingleStmntKeyword()
{
    const auto& spell = tkn_->Spell();

    if (spell == "var")
        ParseVarDeclStmnt();
    else if (spell == "include")
        ParseIncludeStmnt();
    else if (spell == "for")
        ParseForStmnt();
    else if (spell == "if")
        ParseIfStmnt();
    else
        ErrorUnexpected("Variable Declaration-, Include-, For- or If- statement");
}

void FSCInterpreter::ParseArgumentList()
{
    /* Parse argument list opening '(' character */
    Accept(Token::Types::LBracket);

    /* Parse arguments */
    if (TokenType() != Token::Types::RBracket)
    {
        ParseSingleArgument();

        while (TokenType() == Token::Types::Comma)
        {
            AcceptIt();
            ParseSingleArgument();
        }
    }

    /* Accept argument list closing ')' character */
    Accept(Token::Types::RBracket);
}

void FSCInterpreter::ParseSingleArgument()
{
    ParseExpression();
}

void FSCInterpreter::ParseInitializerList()
{
    /* Parse initializer list opening '{' character */
    Accept(Token::Types::LCurly);

    /* Parse initializers */
    if (TokenType() != Token::Types::RCurly)
    {
        ParseSingleInitializer();

        while (TokenType() == Token::Types::Comma)
        {
            AcceptIt();
            ParseSingleInitializer();
        }
    }

    /* Accept initializer list closing '}' character */
    Accept(Token::Types::RCurly);
}

void FSCInterpreter::ParseSingleInitializer()
{
    /* Parse attribute identifier */
    auto identTkn = Accept(Token::Types::Identifier);
    Accept(Token::Types::Colon);

    /* Parse attribute initialization expression */
    ParseExpression();
}

void FSCInterpreter::ParseArrayEntryList()
{
    /* Parse array entry list opening '{' character */
    Accept(Token::Types::LCurly);

    /* Parse array entries */
    ParseSingleArrayEntry();

    while (TokenType() == Token::Types::Comma)
    {
        AcceptIt();
        ParseSingleArrayEntry();
    }

    /* Accept array entry list closing '}' character */
    Accept(Token::Types::RCurly);
}

void FSCInterpreter::ParseSingleArrayEntry()
{
    ParseExpression();
}

void FSCInterpreter::ParseArrayAccessList()
{
    while (TokenType() == Token::Types::LParen)
        ParseSingleArrayAccess();
}

void FSCInterpreter::ParseSingleArrayAccess()
{
    Accept(Token::Types::LParen);
    ParseExpression();
    Accept(Token::Types::RParen);
}

void FSCInterpreter::ParseStructNameStmnt()
{
    auto snIdent = ParseStructNameIdent();

    switch (TokenType())
    {
        case Token::Types::LBracket:
        {
            ParseArgumentList();
        }
        break;

        case Token::Types::EqualityOp:
        {
            try
            {
                scopeMngr_.Fetch(snIdent->ident);
            }
            catch (const std::string& err)
            {
                throw ScriptError(IO::ErrorTypes::Context, snIdent->Pos(), err);
            }

            ParseAssignment();
        }
        break;

        default:
        {
            ErrorUnexpected("Assignment or Argument-List");
        }
        break;
    }
}

void FSCInterpreter::ParseVarDeclStmnt()
{
    /* Parse variable declaration keyword and variable identififer */
    Accept(Token::Types::Keyword, "var");
    
    const auto ident = ParseIdentifier();

    /* Register new variable */
    try
    {
        scopeMngr_.Add(ident->Spell(), nullptr);

        #if 1//!!!
        IO::Log::Debug("Declare Variable \"" + ident->Spell() + "\"");
        #endif
    }
    catch (const std::string& err)
    {
        throw ScriptError(IO::ErrorTypes::Context, ident->Pos(), err);
    }

    /* Parse optional varaible initialization */
    if (TokenType() == Token::Types::EqualityOp && tkn_->Spell() == "=")
        ParseAssignment();
}

void FSCInterpreter::ParseIncludeStmnt()
{
    Accept(Token::Types::Keyword, "include");
    
    /* Parse include filename */
    auto tkn = Accept(Token::Types::StringLiteral);
    auto filename = tkn->Spell();

    /* Include file */
    //!TODO! ...
}

void FSCInterpreter::ParseForStmnt()
{
    Accept(Token::Types::Keyword, "for");

    /* Parse for-loop iteration variable */
    auto varIdent = ParseIdentifier();

    /* Parse loop range */
    Accept(Token::Types::Colon);

    ParseExpression();

    Accept(Token::Types::RangeSep);

    ParseExpression();

    /* Parse loop block */
    ParseBlockStmnt();
}

void FSCInterpreter::ParseIfStmnt()
{
    /* Parse condition expression */
    Accept(Token::Types::Keyword, "if");

    ParseExpression();

    /* Parse IF block */
    ParseBlockStmnt();

    /* Parse optional ELSE blocks */
    if (TokenType() == Token::Types::Keyword)
    {
        if (tkn_->Spell() == "elif")
            ParseElIfStmnt();
        else if (tkn_->Spell() == "else")
            ParseElseStmnt();
    }
}

void FSCInterpreter::ParseElIfStmnt()
{
    /* Parse condition expression */
    Accept(Token::Types::Keyword, "elif");

    ParseExpression();

    /* Parse IF block */
    ParseBlockStmnt();

    /* Parse optional ELSE blocks */
    if (TokenType() == Token::Types::Keyword)
    {
        if (tkn_->Spell() == "elif")
            ParseElIfStmnt();
        else if (tkn_->Spell() == "else")
            ParseElseStmnt();
    }
}

void FSCInterpreter::ParseElseStmnt()
{
    /* Parse condition expression */
    Accept(Token::Types::Keyword, "else");

    /* Parse ELSE block */
    ParseBlockStmnt();
}

void FSCInterpreter::ParseBlockStmnt()
{
    Accept(Token::Types::LCurly);
    ParseStatementList();
    Accept(Token::Types::RCurly);
}

void FSCInterpreter::ParseExpression()
{
    ParseSumExpr();
}

void FSCInterpreter::ParseSumExpr()
{
    ParseProductExpr();

    if (TokenType() == Token::Types::SumOp)
    {
        AcceptIt();
        ParseSumExpr();
    }
}

void FSCInterpreter::ParseProductExpr()
{
    ParseValueExpr();

    if (TokenType() == Token::Types::ProductOp)
    {
        AcceptIt();
        ParseProductExpr();
    }
}

void FSCInterpreter::ParseValueExpr()
{
    switch (TokenType())
    {
        case Token::Types::LBracket:
            ParseBracketExpr();
            break;
        case Token::Types::Identifier:
            ParseStructNameExpr();
            break;
        case Token::Types::IntLiteral:
        case Token::Types::FloatLiteral:
        case Token::Types::StringLiteral:
            ParseLiteralExpr();
            break;
        default:
            if (TokenType() == Token::Types::SumOp && tkn_->Spell() == "-")
                ParseNegationExpr();
            else if (TokenType() == Token::Types::Keyword && tkn_->Spell() == "not")
                ParseNegationExpr();
            else
                ErrorUnexpected("Bracket-, Function-Call-, Assignment-, Negation-, Object- or Literal expression");
            break;
    }
}

void FSCInterpreter::ParseStructNameExpr()
{
    ParseStructNameIdent();

    switch (TokenType())
    {
        case Token::Types::LBracket:
            ParseArgumentList();
            break;
        case Token::Types::EqualityOp:
            ParseAssignment();
            break;
        default:
            break;
    }
}

void FSCInterpreter::ParseNegationExpr()
{
    if (TokenType() == Token::Types::SumOp && tkn_->Spell() == "-")
    {
        AcceptIt();
        //...
    }
    else if (TokenType() == Token::Types::Keyword && tkn_->Spell() == "not")
    {
        AcceptIt();
        //...
    }

    ParseValueExpr();
}

void FSCInterpreter::ParseBracketExpr()
{
    Accept(Token::Types::LBracket);
    ParseExpression();
    Accept(Token::Types::RBracket);
}

void FSCInterpreter::ParseLiteralExpr()
{
    switch (TokenType())
    {
        case Token::Types::IntLiteral:
            ParseIntegerLiteral();
            break;
        case Token::Types::FloatLiteral:
            ParseFloatLiteral();
            break;
        case Token::Types::StringLiteral:
            ParseStringLiteral();
            break;
        default:
            ErrorUnexpected("Assignment or Argument-List");
            break;
    }
}

int FSCInterpreter::ParseIntegerLiteral()
{
    /* Parse integer literal */
    auto tkn = Accept(Token::Types::IntLiteral);
    return StrToNum<int>(tkn->Spell());
}

float FSCInterpreter::ParseFloatLiteral()
{
    /* Parse float literal */
    auto tkn = Accept(Token::Types::FloatLiteral);
    return StrToNum<float>(tkn->Spell());
}

std::string FSCInterpreter::ParseStringLiteral()
{
    /* Parse string literal */
    auto tkn = Accept(Token::Types::StringLiteral);
    return tkn->Spell();
}

void FSCInterpreter::ParseAssignment()
{
    Accept(Token::Types::EqualityOp, "=");

    switch (TokenType())
    {
        case Token::Types::LParen:
            ParseNullListAsmnt();
            break;
        case Token::Types::LCurly:
            ParseListAsmnt();
            break;
        case Token::Types::Keyword:
            ParseAllocAsmnt();
            break;
        default:
            ParseExpression();
            break;
    }
}

void FSCInterpreter::ParseNullListAsmnt()
{
    Accept(Token::Types::LParen);
    Accept(Token::Types::RParen);
}

void FSCInterpreter::ParseListAsmnt()
{
    ParseArrayEntryList();
}

void FSCInterpreter::ParseAllocAsmnt()
{
    Accept(Token::Types::Keyword, "new");
    ParseObjectAllocation();
}

void FSCInterpreter::ParseExpressionAsmnt()
{
    ParseExpression();
}

TokenPtr FSCInterpreter::ParseIdentifier()
{
    /* Parse identifier token */
    return Accept(Token::Types::Identifier);
}

FSCStructNameIdentPtr FSCInterpreter::ParseStructNameIdent()
{
    /* Parse identifier */
    auto ident = ParseIdentifier();

    auto snIdent = std::make_shared<FSCStructNameIdent>(ident->Pos(), ident->Spell());

    /* Parse optional array-access list */
    if (TokenType() == Token::Types::LParen)
        ParseArrayAccessList();

    /* Parse optional further struct-name identifiers */
    if (TokenType() == Token::Types::Dot)
    {
        AcceptIt();
        snIdent->next = ParseStructNameIdent();
    }

    return snIdent;
}

void FSCInterpreter::ParseObjectAllocation()
{
    /* Parse object identifier */
    auto ident = ParseIdentifier();

    switch (TokenType())
    {
        case Token::Types::LBracket:
            ParseArgumentList();
            break;
        case Token::Types::LCurly:
            ParseInitializerList();
            break;
        default:
            ErrorUnexpected("Argument- or Initializer List");
            break;
    }
}


} // /namespace Lang

} // /namespace Fork



// ========================