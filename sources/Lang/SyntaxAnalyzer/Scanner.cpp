/*
 * Scanner file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/Scanner.h"
#include "Lang/SyntaxAnalyzer/ScriptError.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


Scanner::~Scanner()
{
}

bool Scanner::ScanSource(const SourceCodePtr& source, bool initScan)
{
    if (source)
    {
        source_ = source;

        if (initScan)
        {
            /* Get first character */
            TakeIt();
        }

        return true;
    }
    return false;
}

SourcePosition Scanner::Pos() const
{
    return source_ ? source_->Pos() : SourcePosition();
}

char Scanner::TakeIt()
{
    char prevChar = chr_;
    chr_ = source_->Next();
    return prevChar;
}

char Scanner::Take(char chr)
{
    if (chr_ != chr)
        ErrorUnexpected(chr);
    return TakeIt();
}

/* --- Error handling --- */

void Scanner::Error(const std::string& message)
{
    throw ScriptError(IO::ErrorTypes::Syntax, Pos(), message);
}

void Scanner::ErrorUnexpected(char chr)
{
    Error(
        "Unexpected character '" + ToStr(chr_) +
        "' (Expected '" + ToStr(chr) + "')"
    );
}

void Scanner::ErrorUnexpected()
{
    Error("Unexpected character '" + ToStr(chr_) + "'");
}

void Scanner::ErrorEOF()
{
    Error("Unexpected end-of-file");
}

void Scanner::IgnoreWhiteSpaces()
{
    while (IsWhiteSpace())
        TakeIt();
}

void Scanner::IgnoreLine()
{
    while (chr_ != '\n')
        TakeIt();
}

TokenPtr Scanner::MakeToken(const Token::Types& type, bool takeChr)
{
    if (takeChr)
    {
        auto chr = TakeIt();
        return std::make_shared<Token>(type, Pos(), std::string(1, chr));
    }
    return std::make_shared<Token>(type, Pos());
}

TokenPtr Scanner::MakeToken(const Token::Types& type, const std::string& spell, bool takeChr)
{
    if (takeChr)
    {
        auto extSpell = spell;
        extSpell += TakeIt();
        return std::make_shared<Token>(type, Pos(), extSpell);
    }
    return std::make_shared<Token>(type, Pos(), spell);
}

/* --- Pre-defined scanning functions --- */

std::string Scanner::ScanIdentifier()
{
    std::string spell;

    /* Scan identifier start character (letter or '_') */
    if (IsLetter() || chr_ == '_')
        spell += TakeIt();
    else
        ErrorUnexpected();

    /* Scan further identifier characters (letter, '_' or decimal digit) */
    while (IsIdentifierChar())
        spell += TakeIt();

    return spell;
}

TokenPtr Scanner::ScanNumber(bool convertToDecimal)
{
    if (!IsDecDigit())
        Error("Expected digit for number literal");
    
    /* Take first number (literals like ".0" are not allowed) */
    std::string spell;

    const auto startChr = TakeIt();
    spell += startChr;

    Token::Types type = Token::Types::IntLiteral;

    if (startChr == '0')
    {
        switch (chr_)
        {
            case 'b':
                spell += TakeIt();
                ScanBinNumber(spell, type);
                break;
            case 'c':
                spell += TakeIt();
                ScanOctNumber(spell, type);
                break;
            case 'x':
                spell += TakeIt();
                ScanHexNumber(spell, type);
                break;
            default:
                ScanDecNumber(spell, type);
                break;
        }
    }
    else
        ScanDecNumber(spell, type);

    /* Check if conversion to decimal number is required */
    if (convertToDecimal)
    {
        switch (type)
        {
            case Token::Types::HexLiteral:
                return MakeToken(Token::Types::IntLiteral, ToStr(HexToNum<long long int>(spell)), false);
            case Token::Types::OctLiteral:
                return MakeToken(Token::Types::IntLiteral, ToStr(OctToNum<long long int>(spell)), false);
            case Token::Types::BinLiteral:
                return MakeToken(Token::Types::IntLiteral, ToStr(BinToNum<long long int>(spell)), false);
            default:
                break;
        }
    }

    /* Create number token */
    return MakeToken(type, spell, false);
}

TokenPtr Scanner::ScanString()
{
    std::string spell;

    while (true)
    {
        /* Take opening '\"' character */
        Take('\"');

        while (true)
        {
            /* Check for escape characters */
            while (chr_ == '\\')
            {
                TakeIt();

                if (IsEscapeChar())
                {
                    spell += '\\';
                    spell += chr_;
                }
                else
                    ErrorUnexpected();

                TakeIt();
            }

            if (chr_ == 0)
                ErrorEOF();
                
            /* Check for closing '\"' character */
            if (chr_ == '\"')
                break;

            /* Check for new-line character */
            if (chr_ == '\n')
                spell += '\\';

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Take closing '\"' character */
        TakeIt();

        /* Search for next string literal (which will be appended to this token) */
        while (IsWhiteSpace())
            TakeIt();

        if (chr_ != '\"')
            break;

        spell += "\\n";
    }

    /* Return final string literal token */
    return MakeToken(Token::Types::StringLiteral, spell, false);
}

TokenPtr Scanner::ScanVerbatimString()
{
    std::string spell;

    while (true)
    {
        /* Take opening '\"' character */
        Take('\"');

        while (true)
        {
            /* Check for escape characters */
            while (chr_ == '\\')
            {
                TakeIt();
                spell += "\\\\";
            }

            if (chr_ == 0)
                ErrorEOF();
                
            /* Check for closing '\"' character */
            if (chr_ == '\"')
            {
                TakeIt();

                /* Check for double quotes */
                if (chr_ == '\"')
                    spell += "\\";
                else
                    break;
            }

            /* Check for new-line character */
            if (chr_ == '\n')
                spell += '\\';

            /* Append character to string literal */
            spell += TakeIt();
        }

        /* Search for next string literal (which will be appended to this token) */
        while (IsWhiteSpace())
            TakeIt();

        if (chr_ != '\"')
            break;

        spell += "\\n";
    }

    /* Return final string literal token */
    return MakeToken(Token::Types::StringLiteral, spell, false);
}

void Scanner::ScanBinNumber(std::string& spell, Token::Types& type)
{
    type = Token::Types::BinLiteral;

    while (IsBinDigit())
        spell += TakeIt();

    if (IsIdentifierChar())
        ErrorUnexpected();
}

void Scanner::ScanOctNumber(std::string& spell, Token::Types& type)
{
    type = Token::Types::OctLiteral;

    while (IsOctDigit())
        spell += TakeIt();

    if (IsIdentifierChar())
        ErrorUnexpected();
}

void Scanner::ScanDecNumber(std::string& spell, Token::Types& type)
{
    type = Token::Types::IntLiteral;

    while (IsDecDigit() || chr_ == '.')
    {
        /* Check for floating-pointer number */
        if (chr_ == '.')
        {
            switch (type)
            {
                case Token::Types::FloatLiteral:
                    Error("Multiple dots in number");
                    break;
                case Token::Types::IntLiteral:
                    type = Token::Types::FloatLiteral;
                    break;
                default:
                    break;
            }
        }

        /* Append current character */
        spell += TakeIt();
    }

    if (IsIdentifierChar())
        ErrorUnexpected();
}

void Scanner::ScanHexNumber(std::string& spell, Token::Types& type)
{
    type = Token::Types::HexLiteral;

    while (IsHexDigit())
        spell += TakeIt();

    if (IsIdentifierChar())
        ErrorUnexpected();
}

/* --- Character comparision functions --- */

bool Scanner::IsWhiteSpace() const
{
    return chr_ == ' ' || chr_ == '\t' || chr_ == '\n' || chr_ == '\r';
}

bool Scanner::IsLetter() const
{
    return
        ( chr_ >= 'a' && chr_ <= 'z' ) ||
        ( chr_ >= 'A' && chr_ <= 'Z' );
}

bool Scanner::IsIdentifierChar() const
{
    return IsLetter() || IsDecDigit() || chr_ == '_';
}

bool Scanner::IsEscapeChar() const
{
    return
        chr_ == '\\' || chr_ == '\"' || chr_ == '\'' ||
        chr_ == '\0' || chr_ == '?' || chr_ == 'a' ||
        chr_ == 'b' || chr_ == 'f' || chr_ == 'n' ||
        chr_ == 'r' || chr_ == 't' || chr_ == 'v' ||
        chr_ == 'x' || chr_ == 'u' || chr_ == 'U' ||
        IsDecDigit();
}

bool Scanner::IsBinDigit() const
{
    return chr_ == '0' || chr_ == '1';
}

bool Scanner::IsOctDigit() const
{
    return chr_ >= '0' && chr_ <= '7';
}

bool Scanner::IsDecDigit() const
{
    return chr_ >= '0' && chr_ <= '9';
}

bool Scanner::IsHexDigit() const
{
    return
        ( chr_ >= '0' && chr_ <= '7' ) ||
        ( chr_ >=  'a' && chr_ <= 'f' ) ||
        ( chr_ >=  'A' && chr_ <= 'F' );
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================