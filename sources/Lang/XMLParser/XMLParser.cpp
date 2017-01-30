/*
 * XML parser file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/XMLParser/XMLParser.h"
#include "Lang/SyntaxAnalyzer/SourceString.h"
#include "XMLScanner.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Lang
{


using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;

XMLParser::XMLParser() :
    Parser(std::make_shared<XMLScanner>())
{
}

XMLParser::XMLTagList XMLParser::ParseXMLFile(const std::string& filename)
{
    IO::Log::Message("Read XML file: \"" + filename + "\"");
    IO::Log::ScopedIndent indent;

    XMLTagList xmlTags;

    try
    {
        /* Open source file */
        sourceFile_ = SourceFile::Open(filename);
        if (!sourceFile_)
            throw IO::Error(IO::ErrorTypes::FileNotFound, "Reading XML file failed");

        /* Start scanning source */
        if (!scanner_->ScanSource(sourceFile_))
            throw IO::Error(IO::ErrorTypes::Default, "Scanning source failed");

        /* Parse XMl file */
        AcceptIt();

        std::string tagEnd;
        while (TokenType() == Token::Types::RelationOp && tkn_->Spell() == "<")
            xmlTags.push_back(ParseTag(tagEnd));
    }
    catch (const IO::Error& err)
    {
        IO::Log::Error(err);
    }

    return xmlTags;
}

XMLParser::XMLTagList XMLParser::ParseXMLString(const std::string& xmlContent)
{
    XMLTagList xmlTags;

    try
    {
        /* Open source file */
        auto sourceString = std::make_shared<SourceString>(xmlContent);

        /* Start scanning source */
        if (!scanner_->ScanSource(sourceString))
            throw IO::Error(IO::ErrorTypes::Default, "Scanning source failed");

        /* Parse XMl file */
        AcceptIt();

        std::string tagEnd;
        while (TokenType() == Token::Types::RelationOp && tkn_->Spell() == "<")
            xmlTags.push_back(ParseTag(tagEnd));
    }
    catch (const IO::Error& err)
    {
        IO::Log::Error(err);
    }

    return xmlTags;
}


/*
 * ======= Private: =======
 */

bool XMLParser::IsTokenContent() const
{
    return
        TokenType() == Token::Types::Other         ||
        TokenType() == Token::Types::Keyword       ||
        TokenType() == Token::Types::IntLiteral    ||
        TokenType() == Token::Types::Identifier    ||
        TokenType() == Token::Types::EqualityOp    ||
        TokenType() == Token::Types::SumOp         ||
        TokenType() == Token::Types::StringLiteral ||
        TokenType() == Token::Types::ProductOp     ||
        TokenType() == Token::Types::Colon;
}

XMLTagPtr XMLParser::ParseTag(std::string& tagEnd)
{
    /* Parse XML tag identifier */
    Accept(Token::Types::RelationOp, "<");
    auto ident = Accept(Token::Types::Identifier);

    tagEnd = ident->Spell();

    auto tagAST = std::make_shared<XMLTag>(ident->Pos(), ident->Spell());

    /* Parse optional attributes */
    while (TokenType() == Token::Types::Identifier)
    {
        auto attrib = ParseAttribute();
        tagAST->attributes.push_back(attrib);
    }

    /* Parse XML tag ending */
    auto tknTagEnd = Accept(Token::Types::RelationOp);

    if (tknTagEnd->Spell() == "/>")
        return tagAST;
    else if (tknTagEnd->Spell() != ">")
        ErrorUnexpected("Tag closing character '>' or '/>'");

    /* Parse XML tag content */
    while (true)
    {
        if (TokenType() == Token::Types::RelationOp)
        {
            if (tkn_->Spell() == "</")
            {
                /* Parse XML tag ending */
                AcceptIt();
                auto identEnd = Accept(Token::Types::Identifier);
                
                Accept(Token::Types::RelationOp, ">");

                if (identEnd->Spell() != ident->Spell())
                {
                    IO::Log::Warning(
                        "Invalid XML tag ending \"" + identEnd->Spell() +
                        "\" (Expected \"" + ident->Spell() + "\")"
                    );
                }

                tagEnd = identEnd->Spell();

                return tagAST;
            }
            else if (tkn_->Spell() == "<")
            {
                /* Pares XML sub tag */
                std::string subTagEnd;
                auto subTag = ParseTag(subTagEnd);
                tagAST->nodes.push_back(subTag);
                
                /* Check if tag was ended correctly */
                if (subTag->ident != subTagEnd)
                {
                    /* Check if the tag ending was meant for this tag */
                    if (subTagEnd == tagAST->ident)
                        return tagAST;
                    else
                        tagEnd = subTagEnd;

                    return tagAST;
                }

                continue;
            }
        }
        
        if (!IsTokenContent())
            break;

        tagAST->nodes.push_back(ParseContent());
    }

    return tagAST;
}

XMLAttributePtr XMLParser::ParseAttribute()
{
    /* Parse attribute identifier */
    auto tkn = Accept(Token::Types::Identifier);
    auto ident = tkn->Spell();

    while (TokenType() == Token::Types::Colon || TokenType() == Token::Types::SumOp)
    {
        AcceptIt();
        ident += tkn_->Spell();

        tkn = Accept(Token::Types::Identifier);
        ident += tkn->Spell();
    }

    /* Parse attribute value */
    Accept(Token::Types::EqualityOp, "=");
    auto attribValue = Accept(Token::Types::StringLiteral);

    /* Create XML attribute AST node */
    return std::make_shared<XMLAttribute>(
        tkn_->Pos(), ident, attribValue->Spell()
    );
}

XMLContentPtr XMLParser::ParseContent()
{
    /* Check for special character sequence */
    if (TokenType() == Token::Types::Keyword)
    {
        auto tkn = AcceptIt();
        auto spell = tkn->Spell();

             if (spell == "&lt;"  ) spell = "<";
        else if (spell == "&gt;"  ) spell = ">";
        else if (spell == "&amp;" ) spell = "&";
        else if (spell == "&quot;") spell = "\"";
        else if (spell == "&apos;") spell = "'";
        else if (spell == "&nbsp;") spell = " ";

        return std::make_shared<XMLContent>(tkn->Pos(), spell);
    }

    /* Parse default content */
    auto tkn = AcceptIt();
    return std::make_shared<XMLContent>(tkn->Pos(), tkn->Spell());
}



} // /namespace Lang

} // /namespace Fork



// ========================