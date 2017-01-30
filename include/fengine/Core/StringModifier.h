/*
 * String modifier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STRING_MODIFIER_H__
#define __FORK_STRING_MODIFIER_H__


#include "Export.h"

#include <string>
#include <sstream>


namespace Fork
{


/* === Functions === */

//! Converts the UTF-8 string into a UTF-16 string.
FORK_EXPORT std::wstring UTF8toUTF16(const std::string& str);
//! Converts the UTF-16 string into a UTF-8 string.
FORK_EXPORT std::string UTF16toUTF8(const std::wstring& wstr);

/* --- Number to string functions --- */

FORK_EXPORT std::string ToStr( bool val                   );

FORK_EXPORT std::string ToStr( char val                   );

FORK_EXPORT std::string ToStr( short val                  );
FORK_EXPORT std::string ToStr( unsigned short val         );

FORK_EXPORT std::string ToStr( int val                    );
FORK_EXPORT std::string ToStr( unsigned int val           );

FORK_EXPORT std::string ToStr( long val                   );
FORK_EXPORT std::string ToStr( unsigned long val          );

FORK_EXPORT std::string ToStr( long long int val          );
FORK_EXPORT std::string ToStr( unsigned long long int val );

FORK_EXPORT std::string ToStr( float val                  );
FORK_EXPORT std::string ToStr( double val                 );
FORK_EXPORT std::string ToStr( long double val            );

/* --- Number to string functions (with precision) --- */

static const int defaultPrecision = -1;

FORK_EXPORT std::string ToStr( bool val,                   int precision );

FORK_EXPORT std::string ToStr( char val,                   int precision );

FORK_EXPORT std::string ToStr( short val,                  int precision );
FORK_EXPORT std::string ToStr( unsigned short val,         int precision );

FORK_EXPORT std::string ToStr( int val,                    int precision );
FORK_EXPORT std::string ToStr( unsigned int val,           int precision );

FORK_EXPORT std::string ToStr( long val,                   int precision );
FORK_EXPORT std::string ToStr( unsigned long val,          int precision );

FORK_EXPORT std::string ToStr( long long int val,          int precision );
FORK_EXPORT std::string ToStr( unsigned long long int val, int precision );

FORK_EXPORT std::string ToStr( float val,                  int precision );
FORK_EXPORT std::string ToStr( double val,                 int precision );
FORK_EXPORT std::string ToStr( long double val,            int precision );

/* === Inline functions === */

inline std::string ToStr(const std::string &str)
{
    return str;
}

inline std::string ToStr(const std::wstring &str)
{
    return UTF16toUTF8(str);
}

inline std::string ToStr(const char* str)
{
    return std::string(str);
}

inline std::string ToStr(const wchar_t* str)
{
    return UTF16toUTF8(std::wstring(str));
}

/* --- Other string modification functions --- */

//! Returns the name of the specified filename (e.g. from "C:/Program Files/Programming/App.txt" to "App.txt").
FORK_EXPORT std::string ExtractFileName(const std::string& filename);
//! Returns the name of the specified filename (e.g. from L"C:/Program Files/Programming/App.txt" to L"App.txt").
FORK_EXPORT std::wstring ExtractFileName(const std::wstring& filename);

//! Returns the path of the specified filename (e.g. from "C:/Program Files/Programming/App.txt" to "C:/Program Files/Programming").
FORK_EXPORT std::string ExtractFilePath(const std::string& filename);
//! Returns the path of the specified filename (e.g. from L"C:/Program Files/Programming/App.txt" to L"C:/Program Files/Programming").
FORK_EXPORT std::wstring ExtractFilePath(const std::wstring& filename);

//! Returns the extension part of the specified filename (e.g. from "C:/Program Files/Programming/App.txt" to "txt").
FORK_EXPORT std::string ExtractFileExt(const std::string& filename);
//! Returns the extension part of the specified filename (e.g. from L"C:/Program Files/Programming/App.txt" to L"txt").
FORK_EXPORT std::wstring ExtractFileExt(const std::wstring& filename);

//! Replaces all occurrences of 'search' string by 'replace' string and stores it in 'subject'.
FORK_EXPORT void ReplaceString(
    std::string& subject, const std::string& search, const std::string& replace
);
//! Replaces all occurrences of 'search' string by 'replace' string and stores it in 'subject'.
FORK_EXPORT void ReplaceString(
    std::wstring& subject, const std::wstring& search, const std::wstring& replace
);

//! Returns true if 'chr' is an element of { ' ', '\t', '\r', '\n' };
FORK_EXPORT bool IsWhiteSpace(char chr);

/**
Returns true if the specified string has white spaces.
\see IsWhiteSpace
*/
FORK_EXPORT bool HasWhiteSpaces(const std::string& str);

/**
Removes all white spaces from the specified string.
\see IsWhiteSpace
*/
FORK_EXPORT void RemoveWhiteSpaces(std::string& str);

/**
Converts the specified number 'num' into a string with some character offset.
\param[in] num Specifies the number which is to be converted into a string.
\param[in] maxNum Specifies the maximal number until no further characters will be filled.
If your number should be converted from 3 to "03" for instance, 'maxNum' must be 99.
\param[in] fillChar Specifies the character to fill the string. By default ' '.
\param[in] base Specifies the number base. For decimal numbers this must be 10;
For hex numbers this must be 16; etc.
\todo Refactor this function, particular 'maxNum'!
*/
FORK_EXPORT std::string NumberOffset(
    size_t num, size_t maxNum, const char fillChar = ' ', const size_t base = 10
);

//! Returns the specified character in lower case.
FORK_EXPORT char ToLower(char chr);
//! Returns the specified character in upper case.
FORK_EXPORT char ToUpper(char chr);

//! Changes the specified string to lower case.
FORK_EXPORT void MakeLower(std::string& str);
//! Changes the specified string to upper case.
FORK_EXPORT void MakeUpper(std::string& str);

//! Returns the specified string in lower case.
FORK_EXPORT std::string ToLower(std::string str);
//! Returns the specified string in upper case.
FORK_EXPORT std::string ToUpper(std::string str);

//! Returns true if the specified (case insensitive) strings are equal.
FORK_EXPORT bool CiStringsEqual(const std::string& a, const std::string& b);


/* === Templates === */

template <typename T> T StrToNum(const std::string& str)
{
    T val = T(0);
    std::istringstream stream(str);
    stream >> val;
    return val;
}

template <typename T> std::string NumToHex(T number, const size_t size, bool prefix = true)
{
    static const char* hexAlphabet = "0123456789abcdef";

    std::string str;
    
    if (prefix)
    {
        for (int i = 2*size - 1; i >= 0; --i)
        {
            str += ToStr(
                hexAlphabet[(number >> i*4) & 0xF]
            );
        }
    }
    else
    {
        do
        {
            str = ToStr(hexAlphabet[number & 0xF]) + str;
            number >>= 4;
        }
        while (number > 0);
    }

    return str;
}

template <typename T> std::string NumToHex(const T& number, bool prefix = true)
{
    return NumToHex(number, sizeof(T), prefix);
}

template <typename T> std::string NumToOct(T number)
{
    std::string str;
    
    do
    {
        str = ToStr("01234567"[number & 0x7]) + str;
        number >>= 3;
    }
    while (number > 0);
    
    return str;
}

template <typename T> std::string NumToBin(T number)
{
    std::string str;
    
    do
    {
        str = ((number & 0x1) != 0 ? "1" : "0") + str;
        number >>= 1;
    }
    while (number > 0);
    
    return str;
}

template <typename T> T HexToNum(const std::string& str)
{
    T num = T(0);
    std::stringstream stream;
    
    stream << std::hex << str;
    stream >> num;

    return num;
}

template <typename T> T OctToNum(const std::string& str)
{
    T num = T(0);

    auto it = str.begin();

    if (str.size() > 2 && str[0] == '0' && str[1] == 'c')
        it += 2;

    for (; it != str.end(); ++it)
    {
        num <<= 3;
        num += ((*it) - '0') & 0x7;
    }

    return num;
}

template <typename T> T BinToNum(const std::string& str)
{
    T num = T(0);

    auto it = str.begin();

    if (str.size() > 2 && str[0] == '0' && str[1] == 'b')
        it += 2;

    for (; it != str.end(); ++it)
    {
        num <<= 1;
        if (*it != '0')
            ++num;
    }

    return num;
}

//! Removes all carriage return characters '\\r'.
template <typename T> void RemoveCR(std::basic_string<T>& str)
{
    for (auto it = str.begin(); it != str.end();)
    {
        if (*it == unsigned char('\r'))
            it = str.erase(it);
        else
            ++it;
    }
}

//! Removes all special characters, i.e. those < 32.
template <typename T> void RemoveSpecialChars(std::basic_string<T>& str)
{
    for (auto it = str.begin(); it != str.end();)
    {
        if (*it < 32)
            it = str.erase(it);
        else
            ++it;
    }
}

//! Replaces all tabulator characters '\\t' with a simple space ' ' character.
template <typename T> void RemoveTB(std::basic_string<T>& str)
{
    for (auto it = str.begin(); it != str.end();)
    {
        if (*it == unsigned char('\t'))
            *it = unsigned char(' ');
        ++it;
    }
}


} // /namespace Fork


#endif



// ========================