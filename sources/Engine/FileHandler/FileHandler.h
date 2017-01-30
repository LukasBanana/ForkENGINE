/*
 * Engine file handler header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FILE_HANDLER_H__
#define __FORK_ENGINE_FILE_HANDLER_H__


#include "FileHeader.h"
#include "IO/FileSystem/File.h"

#include <stack>


namespace Fork
{

namespace Engine
{

namespace Format
{


//! Engine format file handler.
class FileHandler
{
    
    public:
        
        FileHandler(IO::File& file);
        virtual ~FileHandler();

        FileHandler(const FileHandler&) = delete;
        FileHandler& operator = (const FileHandler&) = delete;

        //! Returns the current stream position.
        std::streampos StreamPos() const;
        //! Seeks the stream to the specified position.
        void StreamPos(std::streampos pos);

        //! Pushes the stream position onto the stack and returns the previous stream position.
        std::streampos PushPos(std::streampos pos);
        //! Pops the stream position from the stack.
        void PopPos();

        //! Returns the format version as string (e.g. version 103 is "1.03").
        std::string VersionString() const;

        //! Returns the file header.
        inline const FileHeader& GetHeader() const
        {
            return header_;
        }

        //! Returns the file resource.
        inline IO::File& GetFile() const
        {
            return file_;
        }

    protected:
        
        FileHeader header_;

    private:
        
        IO::File& file_;

        std::stack<std::streampos> streamPosStack_;

};


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================