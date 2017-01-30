/*
 * Filename file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/Filename.h"
#include "Core/StringModifier.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace IO
{


static const char* pathSep = "/\\";

// Returns true if 'chr' is a path separator (i.e. equal to '/' or '\').
static bool IsPathSep(char chr)
{
    return chr == '/' || chr == '\\';
}

Filename::Filename(const std::string& filename) :
    filename_{ filename }
{
}
Filename::Filename(std::string&& filename) :
    filename_{ std::move(filename) }
{
}
Filename::Filename(Filename&& other) :
    filename_{ std::move(other.filename_) }
{
}

std::string Filename::Path() const
{
    /* Return left part of filename ending with the last appearance of '/' or '\' */
    auto pos = filename_.find_last_of(pathSep);
    if (pos != std::string::npos && pos > 0)
        return filename_.substr(0, pos);
    return ".";
}

std::string Filename::Name() const
{
    /* Get beginning of name part */
    auto begin = filename_.find_last_of(pathSep);
    if (begin == std::string::npos)
        begin = 0;
    else
        ++begin;

    /* Get end of name part */
    auto end = filename_.find_last_of('.');
    if (end != std::string::npos)
        return (end < begin) ? filename_.substr(begin) : filename_.substr(begin, end - begin);

    return filename_;
}

std::string Filename::Ext() const
{
    /* Return right part of filename beginning with the last appearance of '.' */
    auto pos = filename_.find_last_of('.');
    if (pos != std::string::npos && pos > 0 && pos + 1 < filename_.size())
        return filename_.substr(pos + 1, filename_.size() - pos - 1);
    return filename_;
}

std::string Filename::NameExt() const
{
    /* Return right part of filename beginning with the last appearance of '/' or '\' */
    auto pos = filename_.find_last_of(pathSep);
    if (pos != std::string::npos && pos + 1 < filename_.size())
        return filename_.substr(pos + 1);
    return filename_;
}

void Filename::ChangePath(const std::string& path)
{
    auto pos = filename_.find_last_of(pathSep);
    if (pos != std::string::npos)
    {
        /* Replace path */
        if (path.empty())
            filename_.erase(0, pos + 1);
        else if (IsPathSep(path.back()))
            filename_.replace(0, pos + 1, path);
        else
            filename_.replace(0, pos, path);
    }
    else if (!path.empty())
    {
        /* Filename current has no path -> insert at beginning */
        if (IsPathSep(path.back()))
            filename_.insert(0, path);
        else
        {
            filename_.insert(0, 1, '/');
            filename_.insert(0, path);
        }
    }
}

void Filename::ChangeName(const std::string& name)
{
    /* Find range between '/' (or '\') and '.' characters */
    auto startPos = filename_.find_last_of(pathSep);
    auto endPos = filename_.find_last_of('.');

    /* Replace name between the range */
    if (startPos != std::string::npos && endPos != std::string::npos && endPos > startPos + 1 && startPos + 1 < filename_.size())
        filename_.replace(startPos + 1, endPos - startPos - 1, name);
}

void Filename::ChangeExt(const std::string& ext)
{
    /* Replace extension after the last '.' character */
    auto pos = filename_.find_last_of('.');
    if (pos != std::string::npos && pos + 1 < filename_.size())
        filename_.replace(pos + 1, std::string::npos, ext);
}

void Filename::ChangeNameExt(const std::string& nameExt)
{
    /* Replace name and extension after the last '/' or '\' character */
    auto pos = filename_.find_last_of(pathSep);
    if (pos != std::string::npos && pos + 1 < filename_.size())
        filename_.replace(pos + 1, std::string::npos, nameExt);
}

/*
Adds the specified path and removes all "../" or "..\\" sub-paths wherever possible:
"/foo/bar" + "../test" = "/foo/test"
*/
void Filename::AddPath(const std::string& path)
{
    PathRange range;
    size_t startPos = 0;

    while (Filename::NextSubPath(path, range, startPos))
    {
        auto subPath = path.substr(range.first, range.second - range.first);
        
        if (subPath == "..")
        {
            /* Move up in the path hierarchy */
            PopPath();
        }
        else if (subPath != ".")
        {
            /* Insert sub-path into filename */
            auto pos = filename_.find_last_of(pathSep);
            if (pos != std::string::npos)
            {
                filename_.insert(pos, subPath);
                filename_.insert(pos, 1, '/');
            }
        }

        startPos = range.second + 1;
    }
}

/*
Removes all sub-paths wherever possible:
"/foo/bar/test/file.txt" - "foo/bar/test2/file.txt" = "test/file.txt"
*/
void Filename::RemovePath(const std::string& path)
{
    PathRange rangeSrc, rangeDst;
    size_t startPosSrc = 0;
    std::string subPathSrc, subPathDst;

    while (Filename::NextSubPath(path, rangeSrc, startPosSrc))
    {
        /* Get next sub-path from this filename */
        size_t startPosDst = 0;
        subPathDst.clear();

        while (subPathDst.empty() && Filename::NextSubPath(filename_, rangeDst, startPosDst))
        {
            subPathDst = filename_.substr(rangeDst.first, rangeDst.second - rangeDst.first);
            startPosDst = rangeDst.second + 1;
        }

        /* Remove sub-path (if possible) */
        subPathSrc = path.substr(rangeSrc.first, rangeSrc.second - rangeSrc.first);

        if (CiStringsEqual(subPathSrc, subPathDst))
            filename_.erase(0, startPosDst);
        else
            break;

        startPosSrc = rangeSrc.second + 1;
    }
}

/*
Removes the deepest sub-path:
"/foo/bar/test.txt" --> "/foo/test.txt"
*/
bool Filename::PopPath()
{
    PathRange range;
    if (Filename::PrevSubPath(filename_, range) && range.first >= 2)
    {
        if (Filename::PrevSubPath(filename_, range, range.first) && range.first >= 1)
        {
            filename_.erase(range.first - 1, range.second - range.first + 1);
            return true;
        }
    }
    return false;
}

void Filename::CleanPath(char sep)
{
    if (sep != '/' && sep != '\\')
        throw InvalidArgumentException(__FUNCTION__, "sep", "Path separator must be either '/' or '\\'");

    for (auto& chr : filename_)
    {
        if (chr == '/' || chr == '\\')
            chr = sep;
    }
}

/*
Searches the next sub-path:
 "/foo/bar" --> "/foo/bar"
   ^              [  )
startPos         return
*/
bool Filename::NextSubPath(const std::string& path, PathRange& range, size_t startPos)
{
    if (path.size() > startPos)
    {
        range.first = startPos;
        range.second = path.find_first_of(pathSep, startPos);
        if (range.second == std::string::npos)
            range.second = path.size();
        return true;
    }
    return false;
}

/*
Searches the previous sub-path:
"/foo/bar" --> "/foo/bar"
      ^          [  )
   endPos       return
*/
bool Filename::PrevSubPath(const std::string& path, PathRange& range, size_t endPos)
{
    if (endPos == std::string::npos)
    {
        if (!path.empty())
        {
            range.first = path.find_last_of(pathSep);
            range.second = path.size();
            
            if (range.first == std::string::npos)
                range.first = 0;
            else
                ++range.first;

            return true;
        }
    }
    else if (endPos >= 2 && path.size() + 2 > endPos)
    {
        range.first = path.find_last_of(pathSep, endPos - 2);
        range.second = endPos - 1;
        
        if (range.first == std::string::npos)
            range.first = 0;
        else
            ++range.first;

        return true;
    }
    return false;
}


} // /namespace IO

} // /namespace Fork



// ========================