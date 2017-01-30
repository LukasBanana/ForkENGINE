/*
 * Source position header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SOURCE_POSITION_H__
#define __FORK_SYNTAX_ANALYZER_SOURCE_POSITION_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


//! Source position class for script parsing.
class FORK_EXPORT SourcePosition
{
    
    public:
        
        SourcePosition();
        SourcePosition(unsigned int row, unsigned int column);
        ~SourcePosition();

        /* === Functions === */

        //! Returns the source position as string, e.g. "(75:10)".
        std::string GetString() const;

        void IncRow();
        void IncColumn();

        //! Returns ture if this is a valid source position. False if row and column are 0.
        bool IsValid() const;

        /* === Inline functions === */

        //! Returns the row of the source position, beginning with 1.
        inline unsigned int Row() const
        {
            return row_;
        }
        //! Returns the colummn of the source position, beginning with 1.
        inline unsigned int Column() const
        {
            return column_;
        }

    private:
        
        /* === Members === */

        unsigned int row_, column_;

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================