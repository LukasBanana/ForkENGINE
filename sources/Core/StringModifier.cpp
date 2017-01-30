/*
 * String modifier file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/StringModifier.h"

#include <algorithm>
#include <limits>


namespace Fork
{



/* === Templates === */

template <typename T> inline std::string ToStrTmpl(const T& val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

template <typename T> inline std::string ToStrTmplPrec(const T& val)
{
    std::stringstream stream;
    stream.precision(std::numeric_limits<T>::digits10);
    stream << val;
    return stream.str();
}

template <typename T> inline std::string ToStrTmplPrec(const T& val, int precision)
{
    std::stringstream stream;
    if (precision != defaultPrecision)
    {
        stream.precision(precision);
        stream << std::fixed << val;
    }
    else
        stream << val;
    return stream.str();
}


/* === Functions === */

FORK_EXPORT std::wstring UTF8toUTF16(const std::string& str)
{
    std::wstring wstr;
    wstr.resize(str.size());

    for (size_t i = 0, n = str.size(); i < n; ++i)
        wstr[i] = static_cast<wchar_t>(static_cast<unsigned char>(str[i]));

    return wstr;
}

FORK_EXPORT std::string UTF16toUTF8(const std::wstring& wstr)
{
    std::string str;
    str.resize(wstr.size());

    for (size_t i = 0, n = str.size(); i < n; ++i)
        str[i] = static_cast<char>(static_cast<unsigned char>(wstr[i]));

    return str;
}

/* --- Number to string functions --- */

FORK_EXPORT std::string ToStr(bool val)
{
    return std::string(val ? "true" : "false");
}

FORK_EXPORT std::string ToStr(char val)
{
    return std::string(1, val);
}

FORK_EXPORT std::string ToStr(short val)
{
    return ToStrTmpl(val);
}
FORK_EXPORT std::string ToStr(unsigned short val)
{
    return ToStrTmpl(val);
}

FORK_EXPORT std::string ToStr(int val)
{
    return ToStrTmpl(val);
}
FORK_EXPORT std::string ToStr(unsigned int val)
{
    return ToStrTmpl(val);
}

FORK_EXPORT std::string ToStr(long val)
{
    return ToStrTmpl(val);
}
FORK_EXPORT std::string ToStr(unsigned long val)
{
    return ToStrTmpl(val);
}

FORK_EXPORT std::string ToStr(long long int val)
{
    return ToStrTmpl(val);
}
FORK_EXPORT std::string ToStr(unsigned long long int val)
{
    return ToStrTmpl(val);
}

FORK_EXPORT std::string ToStr(float val)
{
    return ToStrTmplPrec(val);
}
FORK_EXPORT std::string ToStr(double val)
{
    return ToStrTmplPrec(val);
}
FORK_EXPORT std::string ToStr(long double val)
{
    return ToStrTmplPrec(val);
}

/* --- Number to string functions (with precision) --- */

FORK_EXPORT std::string ToStr(bool val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(char val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(short val, int unused)
{
    return ToStr(val);
}
FORK_EXPORT std::string ToStr(unsigned short val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(int val, int unused)
{
    return ToStr(val);
}
FORK_EXPORT std::string ToStr(unsigned int val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(long val, int unused)
{
    return ToStr(val);
}
FORK_EXPORT std::string ToStr(unsigned long val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(long long int val, int unused)
{
    return ToStr(val);
}
FORK_EXPORT std::string ToStr(unsigned long long int val, int unused)
{
    return ToStr(val);
}

FORK_EXPORT std::string ToStr(float val, int precision)
{
    return ToStrTmplPrec(val, precision);
}
FORK_EXPORT std::string ToStr(double val, int precision)
{
    return ToStrTmplPrec(val, precision);
}
FORK_EXPORT std::string ToStr(long double val, int precision)
{
    return ToStrTmplPrec(val, precision);
}

/* --- Other string modification functions --- */

template <typename T> std::basic_string<T> ExtractFileNameTmpl(
    const std::basic_string<T>& filename, const T* searchString)
{
    /* Find position with filename only */
    auto pos = filename.find_last_of(searchString);

    if (pos == std::basic_string<T>::npos)
        return filename;

    /* Return filename only */
    return filename.substr(pos + 1);
}

FORK_EXPORT std::string ExtractFileName(const std::string& filename)
{
    return ExtractFileNameTmpl(filename, "/\\");
}

FORK_EXPORT std::wstring ExtractFileName(const std::wstring& filename)
{
    return ExtractFileNameTmpl(filename, L"/\\");
}

template <typename T> std::basic_string<T> ExtractFilePathTmpl(
    const std::basic_string<T>& filename, const T* searchString, const T* defaultString)
{
    /* Return file path only */
    auto pos = filename.find_last_of(searchString);
    return pos != std::basic_string<T>::npos ? filename.substr(0, pos) : defaultString;
}

FORK_EXPORT std::string ExtractFilePath(const std::string& filename)
{
    return ExtractFilePathTmpl(filename, "/\\", ".");
}

FORK_EXPORT std::wstring ExtractFilePath(const std::wstring& filename)
{
    return ExtractFilePathTmpl(filename, L"/\\", L".");
}

template <typename T> std::basic_string<T> ExtractFileExtTmpl(const std::basic_string<T>& filename, const T& searchChar)
{
    /* Return file extension only */
    auto pos = filename.find_last_of(searchChar);

    if (pos == std::basic_string<T>::npos)
        return filename;

    return filename.substr(pos + 1, filename.size() - pos - 1);
}

FORK_EXPORT std::string ExtractFileExt(const std::string& filename)
{
    return ExtractFileExtTmpl(filename, '.');
}

FORK_EXPORT std::wstring ExtractFileExt(const std::wstring& filename)
{
    return ExtractFileExtTmpl(filename, L'.');
}

template <typename T> void ReplaceStringTmpl(
    std::basic_string<T>& subject, const std::basic_string<T>& search, const std::basic_string<T>& replace)
{
    size_t pos = 0;

    while ( ( pos = subject.find(search, pos) ) != std::string::npos )
    {
        subject.replace(pos, search.size(), replace);
        pos += replace.size();
    }
}

FORK_EXPORT void ReplaceString(
    std::string& subject, const std::string& search, const std::string& replace)
{
    ReplaceStringTmpl(subject, search, replace); 
}

FORK_EXPORT void ReplaceString(
    std::wstring& subject, const std::wstring& search, const std::wstring& replace)
{
    ReplaceStringTmpl(subject, search, replace); 
}

FORK_EXPORT bool IsWhiteSpace(char chr)
{
    return chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n';
}

FORK_EXPORT bool HasWhiteSpaces(const std::string& str)
{
    for (const auto& chr : str)
    {
        if (IsWhiteSpace(chr))
            return true;
    }
    return false;
}

FORK_EXPORT void RemoveWhiteSpaces(std::string& str)
{
    auto it = std::remove_if(
        str.begin(), str.end(),
        [](char chr)
        {
            return IsWhiteSpace(chr);
        }
    );
    str.erase(it, str.end());
}

FORK_EXPORT std::string NumberOffset(
    size_t num, size_t maxNum, const char fillChar, const size_t base)
{
    if (num > maxNum)
        return ToStr(num);

    const size_t numOrig = num;

    /* Find number of numerics */
    size_t maxNumerics = 0, numerics = 0;

    while (maxNum >= base)
    {
        maxNum /= base;
        ++maxNumerics;
    }

    while (num >= base)
    {
        num /= base;
        ++numerics;
    }

    /* Return string with offset and number */
    return
        std::string(maxNumerics - numerics, fillChar) +
        (base > 10 ? NumToHex(numOrig, false) : ToStr(numOrig));
}

FORK_EXPORT char ToLower(char chr)
{
    if (chr >= 'A' && chr <= 'Z')
        chr += 'a' - 'A';
    return chr;
}

FORK_EXPORT char ToUpper(char chr)
{
    if (chr >= 'a' && chr <= 'z')
        chr -= 'a' - 'A';
    return chr;
}

FORK_EXPORT void MakeLower(std::string& str)
{
    for (char& Chr : str)
    {
        if (Chr >= 'A' && Chr <= 'Z')
            Chr += 'a' - 'A';
    }
}

FORK_EXPORT void MakeUpper(std::string& str)
{
    for (char& Chr : str)
    {
        if (Chr >= 'a' && Chr <= 'z')
            Chr -= 'a' - 'A';
    }
}

FORK_EXPORT std::string ToLower(std::string str)
{
    MakeLower(str);
    return str;
}

FORK_EXPORT std::string ToUpper(std::string str)
{
    MakeUpper(str);
    return str;
}

FORK_EXPORT bool CiStringsEqual(const std::string& a, const std::string& b)
{
    if (a.size() != b.size())
        return false;
    
    for (size_t i = 0, n = a.size(); i < n; ++i)
    {
        if (ToLower(a[i]) != ToLower(b[i]))
            return false;
    }

    return true;
}


} // /namespace Fork



// ========================