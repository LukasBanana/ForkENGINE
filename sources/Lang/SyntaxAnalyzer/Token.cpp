/*
 * Token file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/Token.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


Token::Token(const Types& type, const SourcePosition& sourcePos) :
    type_       (type               ),
    sourcePos_  (sourcePos          ),
    spell_      (Token::Spell(type) )
{
}
Token::Token(const Types& type, const SourcePosition& sourcePos, const std::string& spell) :
    type_       (type       ),
    sourcePos_  (sourcePos  ),
    spell_      (spell      )
{
}
Token::~Token()
{
}

std::string Token::Spell(const Token::Types type)
{
    switch (type)
    {
        case Token::Types::Identifier:      return "<identifier>";

        /* --- Literals --- */
        case Token::Types::BoolLiteral:     return "<boolean-literal>";
        case Token::Types::IntLiteral:      return "<integer-literal>";
        case Token::Types::FloatLiteral:    return "<float-literal>";
        case Token::Types::StringLiteral:   return "<string-literal>";
        case Token::Types::HexLiteral:      return "<hex-literal>";
        case Token::Types::OctLiteral:      return "<octal-literal>";
        case Token::Types::BinLiteral:      return "<binary-literal>";
        case Token::Types::PointerLiteral:  return "<pointer-literal>";

        /* --- Operators --- */
        case Token::Types::EqualityOp:      return "<equality-operator>";
        case Token::Types::RelationOp:      return "<relation-operator>";
        case Token::Types::ShiftOp:         return "<shift-operator>";
        case Token::Types::SumOp:           return "<summation-operator>";
        case Token::Types::ProductOp:       return "<product-operator>";
        case Token::Types::AssignOp:        return "<assign-operator>";

        /* --- Punctuation --- */
        case Token::Types::Dot:             return ".";
        case Token::Types::Colon:           return ":";
        case Token::Types::Semicolon:       return ";";
        case Token::Types::Comma:           return ",";
        case Token::Types::RangeSep:        return "..";

        /* --- Brackets --- */
        case Token::Types::LBracket:        return "(";
        case Token::Types::RBracket:        return ")";
        case Token::Types::LCurly:          return "{";
        case Token::Types::RCurly:          return "}";
        case Token::Types::LParen:          return "[";
        case Token::Types::RParen:          return "]";

        /* --- Reservered words --- */
        case Token::Types::Keyword:         return "<keyword>";

        /* --- Special characters --- */
        case Token::Types::At:              return "@";
        case Token::Types::EndOfFile:       return "<end-of-line>";
    }
    return "";
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================