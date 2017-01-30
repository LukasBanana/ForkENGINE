/*
 * Script error header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SCRIPT_ERROR_H__
#define __FORK_SYNTAX_ANALYZER_SCRIPT_ERROR_H__


#include "IO/Core/Error.h"
#include "Core/Export.h"
#include "Lang/SyntaxAnalyzer/SourcePosition.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


class FORK_EXPORT ScriptError : public IO::Error
{
    
    public:
        
        ScriptError(const IO::ErrorTypes type, const std::string& message);
        ScriptError(
            const IO::ErrorTypes type, const SourcePosition &sourcePos, const std::string &message
        );
        ScriptError(
            const IO::ErrorTypes type, const SourcePosition &sourcePos,
            const std::string &message, const std::string& line, const std::string& marker
        );
        ~ScriptError();

        /* === Static functions === */

        static std::string ConstructMessage(
            const IO::ErrorTypes type, const SourcePosition& sourcePos, const std::string& message
        );

        /* === Inline functions === */

        inline const std::string& Line() const
        {
            return line_;
        }
        inline const std::string& Marker() const
        {
            return marker_;
        }

        inline bool HasLineWithMarker() const
        {
            return !line_.empty() && !marker_.empty();
        }

    private:
        
        /* === Members === */

        std::string line_;      //!< Optional source line where the error has been occured.
        std::string marker_;    //!< Optional marker to show where the error has been occured.

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================