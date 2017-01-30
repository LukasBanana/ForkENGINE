/*
 * Filename header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_FILENAME_H__
#define __FORK_IO_FILENAME_H__


#include "Core/Export.h"

#include <string>
#include <utility>


namespace Fork
{

namespace IO
{


/**
Wrapper class for filename strings.
\remarks The engine only allows ASCII filenames (no Unicode) for portability.
*/
class FORK_EXPORT Filename
{

    public:
        
        /**
        Sub-path range structure. 'first' points to the first element and 'second'
        points after the last element, i.e. it is a half open interval: [first .. second).
        */
        typedef std::pair<size_t, size_t> PathRange;

        Filename() = default;
        Filename(const std::string& filename);
        Filename(std::string&& filename);
        Filename(Filename&& other);

        //! Returns the file path part. Default value is ".".
        std::string Path() const;
        //! Returns the file name part (without extension).
        std::string Name() const;
        //! Returns the file extension part.
        std::string Ext() const;
        //! Returns the file name part (with extension).
        std::string NameExt() const;

        //! Changes the file path part.
        void ChangePath(const std::string& path);
        //! Changes the file name part (without extension).
        void ChangeName(const std::string& name);
        //! Changes the file extension part.
        void ChangeExt(const std::string& ext);
        //! Changes the file name part (with extension).
        void ChangeNameExt(const std::string& nameExt);

        //! Adds the specified path to this filename.
        void AddPath(const std::string& path);
        //! Removes the specified path from this filename (if possible).
        void RemovePath(const std::string& path);

        //! Pops the deepest sub-path and returns true if that was possible, e.g. "/foo/bar" --> "/foo".
        bool PopPath();

        /**
        Cleans the path by replacing all path separators with the specified character.
        \param[in] sep Specifies the path separator. Must be '/' or '\\\\'.
        \throws InvalidArgumentException If 'sep' is invalid.
        */
        void CleanPath(char sep);

        //! Returns the position to the next sub-path or std::string::npos if there is no further sub-path.
        static bool NextSubPath(const std::string& path, PathRange& range, size_t startPos = 0);
        //! Returns the position to the previous sub-path or std::string::npos if there is no further sub-path.
        static bool PrevSubPath(const std::string& path, PathRange& range, size_t endPos = std::string::npos);

        //! Returns the actual filename string.
        inline const std::string& Get() const
        {
            return filename_;
        }

        //! Operator for implicit conversion to STL string.
        inline operator const std::string& () const
        {
            return filename_;
        }

    private:
        
        std::string filename_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================