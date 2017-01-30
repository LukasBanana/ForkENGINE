/*
 * Key code record header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_KEY_CODE_RECORD_H__
#define __FORK_IO_KEY_CODE_RECORD_H__


#include <array>


namespace Fork
{

namespace IO
{


/**
Common key code record structure (can be used for KeyCodes, MouseKeyCodes etc.).
\see IO::KeyCodes
\see IO::MouseKeyCodes
\see IO::GamePadKeyCodes
*/
struct KeyCodeRecord
{
    static const size_t numEntries = 8;

    inline void Add(size_t index)
    {
        if (num < numEntries)
            records[num++] = index;
    }

    inline void Clear()
    {
        num = 0;
    }

    std::array<size_t, numEntries> records;
    size_t num;                             //!< Pointer (or rather index) to the current record in the array.
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================