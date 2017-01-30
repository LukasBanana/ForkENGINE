/*
 * Source position header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/SourcePosition.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


SourcePosition::SourcePosition() :
    row_    (0),
    column_ (0)
{
}
SourcePosition::SourcePosition(unsigned int row, unsigned int column) :
    row_    (row    ),
    column_ (column )
{
}
SourcePosition::~SourcePosition()
{
}

std::string SourcePosition::GetString() const
{
    return "(" + ToStr(row_) + ":" + ToStr(column_) + ")";
}

void SourcePosition::IncRow()
{
    ++row_;
    column_ = 0;
}
void SourcePosition::IncColumn()
{
    ++column_;
}

bool SourcePosition::IsValid() const
{
    return row_ > 0 && column_ > 0;
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================