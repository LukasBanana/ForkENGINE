/*
 * Source-file file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/SourceFile.h"
#include "IO/Core/Log.h"
#include "IO/Core/Console.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


bool SourceFile::ReadFile(const std::string& filename)
{
    /* Open file and store filename */
    filename_ = filename;
    stream_.open(filename, std::ios_base::in);

    if (!stream_.good())
        return false;

    return true;
}

char SourceFile::Next()
{
    if (!stream_.is_open())
        return 0;

    /* Check if reader is at end-of-line */
    while (sourcePos_.Column() >= line_.size())
    {
        /* Check if end-of-file is reached */
        if (stream_.eof())
            return 0;

        /* Read new line in source file */
        std::getline(stream_, line_);
        line_ += '\n';
        sourcePos_.IncRow();
    }

    /* Increment column and return current character */
    char chr = line_[sourcePos_.Column()];
    sourcePos_.IncColumn();

    return chr;
}

SourceFilePtr SourceFile::Open(const std::string& filename)
{
    /* Create new source-file object and start reading */
    auto srcFile = std::make_shared<SourceFile>();
    return srcFile->ReadFile(filename) ? srcFile : nullptr;
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================