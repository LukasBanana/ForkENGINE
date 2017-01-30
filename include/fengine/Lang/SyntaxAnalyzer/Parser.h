/*
 * Parser header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_PARSER_H__
#define __FORK_SYNTAX_ANALYZER_PARSER_H__


#include "Lang/SyntaxAnalyzer/Token.h"
#include "Lang/SyntaxAnalyzer/SourceFile.h"
#include "Lang/SyntaxAnalyzer/Scanner.h"
#include "Core/Export.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


/**
Base class for LL(k) with k = 1 parsers. This is just a small interface for the common parser functionality.
\tparam T Specifies the scanner class.
*/
class FORK_EXPORT Parser
{
    
    public:
        
        virtual ~Parser();

        /* === Inline functions === */

        const SourceFilePtr& SourceFile() const
        {
            return sourceFile_;
        }

    protected:
        
        /* === Functions === */

        Parser(const ScannerPtr& scanner);

        /**
        Accepts the current token and scans the next token.
        \throws ScriptError if parsing failed.
        \see ScriptError
        */
        TokenPtr AcceptIt();
        //! Accepts the current token if it matches the specified type.
        TokenPtr Accept(const Token::Types& type);
        //! Accepts the current token if it matches the specified type and spelling.
        TokenPtr Accept(const Token::Types& Type, const std::string& Spell);

        /**
        Throws a syntax error exception.
        \throws ScriptError
        */
        void Error(const std::string& message, bool appendLine = true, TokenPtr tkn = nullptr);
        //! \see Error
        void ErrorUnexpected(const Token::Types& type);
        //! \see Error
        void ErrorUnexpected(const std::string& desc, bool appendExpectedStr = true);
        //! \see Error
        void ErrorUnexpected();
        //! \see Error
        void ErrorEOF();

        //! Returns a string for the error line marker.
        std::string BuildTokenErrorMarker(const std::string& line, const Token& tkn) const;
        //! Returns the input string with replaced tabulators (converted to default spaces).
        std::string BuildTabTruncatedLine(const std::string& line) const;

        /* === Inline functions === */

        inline Token::Types TokenType() const
        {
            return tkn_->Type();
        }

        /* === Members === */

        ScannerPtr scanner_;        //!< Token scanner.
        TokenPtr tkn_;              //!< Token look-ahead.

        SourceFilePtr sourceFile_;  //!< Current source file.

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================