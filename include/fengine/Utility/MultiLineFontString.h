/*
 * Multi-line font string header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_MULTI_LINE_FONT_STRING_H__
#define __FORK_UTILITY_MULTI_LINE_FONT_STRING_H__


#include "Core/Export.h"
#include "Video/Font/Font.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Utility
{


//! Helper class to easily draw a multi-line string for a specific font.
class FORK_EXPORT MultiLineFontString
{

    public:
        
        MultiLineFontString() = default;
        MultiLineFontString(const Video::Font* font, const std::string& text, int maxWidth);

        //! Changes the font used to determine a line break.
        void ChangeFont(const Video::Font* font);
        //! Changes the text.
        void ChangeText(const std::string& text);
        //! Changes the maximal width for each line. The minimal number of characters in each line will be 1.
        void ChangeMaxWidth(int maxWidth);

        //! Changes all multi-line string settings at once.
        void Change(const Video::Font* font, const std::string& text, int maxWidth);

        //! Returns the height of the font or 0 if the font is null.
        int FontHeight() const;

        //! Returns the list of all string lines.
        inline const std::vector<std::string> GetLines() const
        {
            return lines_;
        }

    private:
        
        void UpdateLines();

        const Video::Font*          font_       = nullptr;  //!< Reference to the font.
        std::string                 text_;                  //!< Source text.
        int                         maxWidth_   = 0;        //!< Maximal string width.

        std::vector<std::string>    lines_;                 //!< List of all line strings.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================