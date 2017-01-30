/*
 * Image file formats file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/FileHandler/ImageFileFormats.h"
#include "Core/StringModifier.h"
#include "IO/Core/MagicNumber.h"

#include <fstream>


namespace Fork
{

namespace Video
{


/*
Magic numbers has been taken from:
http://en.wikipedia.org/wiki/List_of_file_signatures
*/
FORK_EXPORT ImageFileFormats DetectFileFormat(const std::string& filename)
{
    /* Open file for reading */
    std::ifstream file(filename);

    if (!file.good())
        return ImageFileFormats::__Unknown__;

    /* Read magic number */
    IO::MagicNumber magicNumber;
    file.read(magicNumber.charSet, 4);

    /* Compare magic number to detect image file format */
    static const unsigned short magicNumberBMP = 0x4d42;
    static const unsigned short magicNumberJPG = 0xd8ff;
    static const unsigned long  magicNumberPNG = 0x474e5089;
    static const unsigned char  magicNumberPCX = 0x0a;
    static const unsigned long  magicNumberDDS = 0x20534444;

    if (magicNumber.number16 == magicNumberBMP)
        return ImageFileFormats::BMP;
    if (magicNumber.number16 == magicNumberJPG)
        return ImageFileFormats::JPG;
    if (magicNumber.number32 == magicNumberPNG)
        return ImageFileFormats::PNG;
    if (magicNumber.number8 == magicNumberPCX)
        return ImageFileFormats::PCX;
    if (magicNumber.number32 == magicNumberDDS)
        return ImageFileFormats::DDS;

    /* Only compare the file extensions */
    const auto fileExt = ToLower(ExtractFileExt(filename));

    if (fileExt == "tga" || fileExt == "tpic")
        return ImageFileFormats::TGA;
    if (fileExt == "hdr")
        return ImageFileFormats::HDR;

    return ImageFileFormats::__Unknown__;
}


} // /namespace Video

} // /namespace Fork



// ========================