/*
 * Token header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_TOKEN_H__
#define __FORK_SYNTAX_ANALYZER_TOKEN_H__


#include "Core/Export.h"
#include "Lang/SyntaxAnalyzer/SourcePosition.h"

#include <memory>


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


class Token;

typedef std::shared_ptr<Token> TokenPtr;

//! This is the base class for all tokens in a scanner (also lexical analyzer) and parser.
class FORK_EXPORT Token
{
    
    public:
        
        /* === Enumerations === */

        enum class Types
        {
            /* --- Identifiers --- */
            Identifier = 0,     //!< <any identifier>

            /* --- Literals --- */
            BoolLiteral,        //!< true, false
            IntLiteral,         //!< <number>
            FloatLiteral,       //!< <number.number>
            StringLiteral,      //!< <"any string">
            HexLiteral,         //!< <hex-literal>
            OctLiteral,         //!< <oct-literal>
            BinLiteral,         //!< <bin-literal>
            PointerLiteral,     //!< null

            /* --- Operators --- */
            EqualityOp,         //!< =, !=
            RelationOp,         //!< <, >, <=, >=
            ConcatOp,           //!< &, |, &&, ||
            ShiftOp,            //!< <<, >>
            SumOp,              //!< +, -
            ProductOp,          //!< *, /, %
            AssignOp,           //!< :=, +=, -=, *=, /=, %=, <<=, >>=, |=, &=, ^=
            NegationOp,         //!< !, ~

            /* --- Punctuation --- */
            Dot,                //!< .
            Colon,              //!< :
            Semicolon,          //!< ;
            Comma,              //!< ,
            RangeSep,           //!< .. (Range separator, e.g. "1 .. 10")

            /* --- Brackets --- */
            LBracket,           //!< (
            RBracket,           //!< )
            LCurly,             //!< {
            RCurly,             //!< }
            LParen,             //!< [
            RParen,             //!< ]

            /* --- Reservered words --- */
            Keyword,            //!< Abstract type for reserved keywords

            /* --- Special characters --- */
            At,                 //!< @ (Smart pointer)
            Other,              //!< Any other character
            EndOfFile,          //!< End of file
        };

        /* === Constructor & destructor === */

        Token(const Types& type, const SourcePosition& sourcePos);
        Token(const Types& type, const SourcePosition& sourcePos, const std::string& spell);
        ~Token();

        /* === Functions === */

        static std::string Spell(const Token::Types type);

        /* === Inline functions === */

        inline const SourcePosition& Pos() const
        {
            return sourcePos_;
        }
        inline const Types& Type() const
        {
            return type_;
        }
        inline const std::string& Spell() const
        {
            return spell_;
        }

    private:
        
        /* === Members === */

        Types type_;                //!< Token type.
        SourcePosition sourcePos_;  //!< Source position.
        std::string spell_;         //!< Spelling, e.g. "+", "while", "17" etc.

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================