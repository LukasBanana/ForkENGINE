/*
 * Font build exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_BUILD_EXCEPTION_H__
#define __FORK_FONT_BUILD_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"
#include "Core/StringModifier.h"
#include "Video/Font/Font.h"


namespace Fork
{


/**
Font build exception.
\ingroup fork_exception
*/
class FORK_EXPORT FontBuildException : public DefaultException
{
    
    public:
        
        FontBuildException(const Video::Font::Description& fontDesc, const std::string& errorDesc) :
            DefaultException
            {
                "FontBuildException",
                "Font Name: \"" + UTF16toUTF8(fontDesc.name) + "\", Description: " + errorDesc
            },
            fontDesc_{ fontDesc }
        {
        }

        //! Returns the font description.
        inline const Video::Font::Description& GetFontDesc() const
        {
            return fontDesc_;
        }

    private:
        
        Video::Font::Description fontDesc_;

};


} // /namespace Fork


#endif



// ========================