/*
 * Source-file header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SYNTAX_ANALYZER_SOURCE_FILE_H__
#define __FORK_SYNTAX_ANALYZER_SOURCE_FILE_H__


#include "Lang/SyntaxAnalyzer/SourceCode.h"

#include <fstream>


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


class SourceFile;

typedef std::shared_ptr<SourceFile> SourceFilePtr;

class FORK_EXPORT SourceFile : public SourceCode
{
    
    public:
        
        /* === Functions === */
        
        bool ReadFile(const std::string& filename);

        char Next();

        /* === Static functinos === */

        //! Opens the specified file and returns the shared pointer or a null pointer if file reading failed.
        static SourceFilePtr Open(const std::string& filename);

        /* === Inline functions === */

        //! Returns the filename of this file stream.
        inline const std::string& Filename() const
        {
            return filename_;
        }

    private:
        
        /* === Members === */

        std::ifstream stream_;  //!< File stream.
        std::string filename_;  //!< Source file name.

};


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork


#endif



// ========================