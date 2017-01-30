/*
 * Text field model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/TextFieldModel.h"


namespace Fork
{

namespace Utility
{


TextFieldModel::~TextFieldModel()
{
}

void TextFieldModel::MoveCursor(int direction)
{
    if (direction < 0)
    {
        auto dir = static_cast<size_t>(-direction);
        if (cursorPos_ > dir)
            cursorPos_ -= dir;
        else
            cursorPos_ = 0;
    }
    else if (direction > 0)
    {
        auto dir = static_cast<size_t>(direction);
        cursorPos_ += dir;

        if (cursorPos_ > text_.size())
            cursorPos_ = text_.size();
    }
}

void TextFieldModel::MoveCursorBegin()
{
    cursorPos_ = 0;
}

void TextFieldModel::MoveCursorEnd()
{
    cursorPos_ = text_.size();
}

void TextFieldModel::JumpLeft()
{
    if (IsSeparatorChar(CharLeft()))
    {
        while (cursorPos_ > 0 && IsSeparatorChar(CharLeft()))
            --cursorPos_;
    }
    while (cursorPos_ > 0 && !IsSeparatorChar(CharLeft()))
        --cursorPos_;
}

void TextFieldModel::JumpRight()
{
    auto size = text_.size();
    if (IsSeparatorChar(CharRight()))
    {
        while (cursorPos_ < size && IsSeparatorChar(CharRight()))
            ++cursorPos_;
    }
    while (cursorPos_ < size && !IsSeparatorChar(CharRight()))
        ++cursorPos_;
}

bool TextFieldModel::IsCursorBegin() const
{
    return cursorPos_ == 0;
}

bool TextFieldModel::IsCursorEnd() const
{
    return cursorPos_ == text_.size();
}

char TextFieldModel::CharLeft() const
{
    return !IsCursorBegin() ? text_[cursorPos_ - 1] : '\0';
}

char TextFieldModel::CharRight() const
{
    return !IsCursorEnd() ? text_[cursorPos_] : '\0';
}

void TextFieldModel::Insert(char chr)
{
    if (cursorPos_ < text_.size())
    {
        if (chr == '\b')
        {
            /* Remove one character before the cursor position */
            if (cursorPos_ > 0)
            {
                --cursorPos_;
                text_.erase(text_.begin() + cursorPos_);
            }
        }
        else if (chr == 127)
        {
            /*
            Remove all characters before the cursor,
            until a space character appears.-
            */
            if (cursorPos_ > 0)
            {
                auto it = text_.begin() + cursorPos_ - 1;
                if (IsSeparatorChar(CharLeft()))
                {
                    while (cursorPos_ > 0 && IsSeparatorChar(CharLeft()))
                    {
                        it = text_.erase(it);
                        if (it != text_.begin())
                            --it;
                        --cursorPos_;
                    }
                }
                while (cursorPos_ > 0 && !IsSeparatorChar(CharLeft()))
                {
                    it = text_.erase(it);
                    if (it != text_.begin())
                        --it;
                    --cursorPos_;
                }
            }
        }
        else if (unsigned char(chr) >= 32 && unsigned char(chr) <= 255)
        {
            /* Insert the new character */
            if (insertionEnabled)
                text_[cursorPos_] = chr;
            else
                text_.insert(text_.begin() + cursorPos_, chr);
            ++cursorPos_;
        }
    }
    else
    {
        if (chr == '\b')
        {
            /* Remove one character before the cursor position */
            if (cursorPos_ > 0)
            {
                text_.pop_back();
                --cursorPos_;
            }
        }
        else if (chr == 127)
        {
            /*
            Remove all characters before the cursor,
            until a space character appears.
            */
            if (cursorPos_ > 0)
            {
                if (IsSeparatorChar(CharLeft()))
                {
                    while (cursorPos_ > 0 && IsSeparatorChar(CharLeft()))
                    {
                        text_.pop_back();
                        --cursorPos_;
                    }
                }
                while (cursorPos_ > 0 && !IsSeparatorChar(CharLeft()))
                {
                    text_.pop_back();
                    --cursorPos_;
                }
            }
        }
        else if (unsigned char(chr) >= 32 && unsigned char(chr) <= 255)
        {
            /* Push back the new character */
            text_ += chr;
            ++cursorPos_;
        }
    }
}

void TextFieldModel::Insert(const std::string& text)
{
    /* Find end of valid string (e.g. new-line character is not allowed) */
    size_t len = std::string::npos;

    for (size_t i = 0, n = text.size(); i < n; ++i)
    {
        if (text[i] < 32)
        {
            len = i;
            break;
        }
    }

    /* Insert text into command line */
    if (len == std::string::npos)
    {
        if (IsCursorEnd())
            text_ += text;
        else
            text_.insert(cursorPos_, text);
        cursorPos_ += text.size();
    }
    else
    {
        if (IsCursorEnd())
            text_ += text.substr(0, len);
        else
            text_.insert(cursorPos_, text, 0, len);
        cursorPos_ += len;
    }
}

void TextFieldModel::Remove(bool removeSequence)
{
    if (cursorPos_ < text_.size())
    {
        auto it = text_.begin() + cursorPos_;

        if (removeSequence)
        {
            /*
            Remove all characters after the cursor position,
            until a white space apprears.
            */
            if (IsSeparatorChar(CharRight()))
            {
                while (cursorPos_ < text_.size() && IsSeparatorChar(CharRight()))
                    it = text_.erase(it);
            }
            while (cursorPos_ < text_.size() && !IsSeparatorChar(CharRight()))
                it = text_.erase(it);
        }
        else
        {
            /* Remove one character after the cursor position */
            text_.erase(it);
        }
    }
}

void TextFieldModel::ClearText()
{
    text_.clear();
    cursorPos_ = 0;
}

void TextFieldModel::ChangeText(const std::string& cmd)
{
    text_ = cmd;
    cursorPos_ = text_.size();
}


/*
 * ======= Protected: =======
 */

const char* TextFieldModel::SeparatorCharList() const
{
    return " /\\.,:;()[]{}\"'";
}

bool TextFieldModel::IsSeparatorChar(char chr) const
{
    for (auto str = SeparatorCharList(); *str; ++str)
    {
        if (chr == *str)
            return true;
    }
    return false;
}


} // /namespace Utility

} // /namespace Fork



// ========================