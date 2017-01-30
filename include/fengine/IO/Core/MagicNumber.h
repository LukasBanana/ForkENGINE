/*
 * Magic number header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_MAGIC_NUMBER_H__
#define __FORK_IO_MAGIC_NUMBER_H__


namespace Fork
{

namespace IO
{


/**
32-bit magic number union. This can be used to determine a file's format type by its magic number:
\code
// Read magic number from file
IO::MagicNumber magicNumber;
std::ifstream file(filename);
file.read(magicNumber.charSet, 4);

// Check for file format type
static const unsigned short magicNumberBMP = 0x4d42;
static const unsigned long  magicNumberPNG = 0x474e5089;

if (magicNumber.number16 == magicNumberBMP)
{
    //...
}
else if (magicNumber.number32 == magicNumberPNG)
{
    //...
}
\endcode
*/
union MagicNumber
{
    char            charSet[4]; //!< Charater set of four elements.
    unsigned char   number8;    //!< 8-bit unsigned integer.
    unsigned short  number16;   //!< 16-bit unsigned integer.
    unsigned long   number32;   //!< 32-bit unsigned integer.
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================