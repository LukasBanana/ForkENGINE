/*
 * Source code interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SOURCE_CODE_H__
#define __FORK_SYNTAX_ANALYZER_SOURCE_CODE_H__


#include "Core/Export.h"
#include "Lang/SyntaxAnalyzer/SourcePosition.h"

#include <string>
#include <memory>


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


class FORK_EXPORT SourceCode
{
    
    public:
        
        virtual ~SourceCode()
        {
        }

        /* === Functions === */

        //! Returns the next character from the source code or 0 if the end was reached.
        virtual char Next() = 0;

        /* === Inline functions === */

        /**
        Returns the current source position.
        \see SourcePosition
        */
        inline const SourcePosition& Pos() const
        {
            return sourcePos_;
        }
        //! Returns the current line which has just been read from the source.
        inline const std::string& Line() const
        {
            return line_;
        }

    protected:
        
        SourceCode()
        {
        }

        /* === Members === */

        std::string line_;          //!< Current source line.
        SourcePosition sourcePos_;  //!< Current source position.

};

typedef std::shared_ptr<SourceCode> SourceCodePtr;


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================