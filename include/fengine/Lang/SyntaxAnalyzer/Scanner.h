/*
 * Scanner header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SCANNER_H__
#define __FORK_SYNTAX_ANALYZER_SCANNER_H__


#include "Lang/SyntaxAnalyzer/SourceCode.h"
#include "Lang/SyntaxAnalyzer/Token.h"
#include "Core/Export.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


//! Common scanner class. A parser for a specific language uses a derived scanner class.
class FORK_EXPORT Scanner
{
    
    public:
        
        virtual ~Scanner();

        /* === Functions === */

        /**
        Starts scanning the specified source code.
        \param[in] source Specifies the source code which is to be scaned.
        \param[in] initScan Specifies whether scanning is to be initialized
        (if the source code has just opened) or continued. By default true.
        \see SourceCode
        \see SourceFile
        */
        bool ScanSource(const SourceCodePtr& source, bool initScan = true);

        /**
        Scans the next token and returns the previous token.
        \see Token
        */
        virtual TokenPtr Next() = 0;

        /**
        Returns the current source position.
        \see SourcePosition
        */
        SourcePosition Pos() const;

        /* === Inline functions === */

        inline const SourceCodePtr& Source() const
        {
            return source_;
        }

    protected:
        
        Scanner() = default;

        /* === Functions === */

        //! Takes the current character and scans the next character.
        char TakeIt();
        //! Takes the current character if it matches the specified character.
        char Take(char chr);

        /* --- Error handling --- */

        /**
        Throws a syntax error exception.
        \throws ScriptError
        */
        void Error(const std::string& message);
        //! \see Error
        void ErrorUnexpected(char chr);
        //! \see Error
        void ErrorUnexpected();
        //! \see Error
        void ErrorEOF();

        /* --- Main scanning functions --- */

        //! Scans an identifier.
        virtual std::string ScanIdentifier();
        //! Scans a number.
        virtual TokenPtr ScanNumber(bool convertToDecimal = false);
        
        //! Scans a standard (ANSI-C like) string literal. Multi-line string literals are allowed.
        virtual TokenPtr ScanString();
        //! Scans a verbatim (C# like) string literal. Multi-line string literals are allowed. Those string literals begin with an '@' character.
        virtual TokenPtr ScanVerbatimString();

        /* --- Other scanning functions --- */

        //! Ignores all white spaces.
        void IgnoreWhiteSpaces();
        //! Ignores all characters until the end of the current line.
        void IgnoreLine();

        TokenPtr MakeToken(const Token::Types& type, bool takeChr = false);
        TokenPtr MakeToken(const Token::Types& type, const std::string& spell, bool takeChr);

        void ScanBinNumber(std::string& spell, Token::Types& type);
        void ScanOctNumber(std::string& spell, Token::Types& type);
        void ScanDecNumber(std::string& spell, Token::Types& type);
        void ScanHexNumber(std::string& spell, Token::Types& type);

        //! Returns true if the current character is a white space (' ', '\\t', '\\n', '\\r').
        bool IsWhiteSpace() const;

        //! Returns true if the current character is a letter (upper and lower case included).
        bool IsLetter() const;

        //! Returns true if the current character is part of an identifier (letter, digit, '_').
        bool IsIdentifierChar() const;

        /**
        Returns true if the current character is an (ANSI-C) escape character (e.g. '\\t', '\\0' etc.).
        \see http://en.wikipedia.org/wiki/Escape_sequences_in_C
        */
        bool IsEscapeChar() const;

        //! Returns true if the current character is a binary digit.
        bool IsBinDigit() const;
        //! Returns true if the current character is a octal digit.
        bool IsOctDigit() const;
        //! Returns true if the current character is a decimal digit.
        bool IsDecDigit() const;
        //! Returns true if the current character is a hexa-decimal digit.
        bool IsHexDigit() const;

        //! Returns the current character. This is a shortcut for "GetChr()", since it may be used very often.
        inline char Chr() const
        {
            return chr_;
        }

    private:
        
        SourceCodePtr source_;

        char chr_ = 0;

};

typedef std::shared_ptr<Scanner> ScannerPtr;


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================