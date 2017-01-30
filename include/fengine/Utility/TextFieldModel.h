/*
 * Text field model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_TEXT_FIELD_MODEL_H__
#define __FORK_UTILITY_TEXT_FIELD_MODEL_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Utility
{


/**
TextFieldModel base class for UTF-8 single line text fields.
This can be used as the model of a text field in a user interface.
*/
class FORK_EXPORT TextFieldModel
{

    public:
        
        virtual ~TextFieldModel();

        //! Moves the cursor into the specified direction.
        void MoveCursor(int direction);
        //! Moves the cursor to the beginning.
        void MoveCursorBegin();
        //! Moves the cursor to the end.
        void MoveCursorEnd();

        //! Jumps to the next left sided space.
        void JumpLeft();
        //! Jumps to the next right sided space.
        void JumpRight();

        //! Returns true if the cursor is at the beginning.
        bool IsCursorBegin() const;
        //! Returns true if the cursor is at the end.
        bool IsCursorEnd() const;

        /**
        Returns the current character which stands immediately before the cursor position.
        If the cursor is at the very beginning of the text field, the return value is '\0'.
        */
        char CharLeft() const;
        /**
        Returns the current character which stands immediately after the cursor position.
        If the cursor is at the very end of the text field, the return value is '\0'.
        */
        char CharRight() const;

        /**
        Inserts the specified character to the text field.
        If the cursor is not at the text field's end, it will be inserted.
        Special characters are:
        - '\b' which will remove the character before the cursor.
        - char(127) which will remove all characters before the cursor until a space appears.
        */
        virtual void Insert(char chr);
        //! Inserts the specified text.
        virtual void Insert(const std::string& text);

        /**
        Removes the character after the cursor position.
        \param[in] removeSequence Specifies whether all characters after the cursor until a space appears are to be removed.
        This is the respective functionality of removing characters like 'Append(char(127))'.
        */
        void Remove(bool removeSequence = false);

        //! Clears the text field content.
        void ClearText();

        //! Changes the content of the text field and moves the cursor at the end.
        void ChangeText(const std::string& cmd);

        //! Returns the content of the text field.
        inline const std::string& GetText() const
        {
            return text_;
        }

        /**
        Returns the current (horizontal) cursor position. This is always in the range [0 .. GetText().size()].
        \note The maximal value is equal to "GetText().size()",
        i.e. the range is 1 unit larger, than the number of characters in the text string.
        */
        inline size_t GetCursorPosition() const
        {
            return cursorPos_;
        }

        /* === Members === */

        bool insertionEnabled = false;  //!< Specifies whether the insertion modd is enabled or not. By default false.

    protected:

        /**
        Returns the char array of separator characters.
        \see IsSeparatorChar
        */
        virtual const char* SeparatorCharList() const;

        /**
        Returns true if a 'special character' was passed as argument.
        This will be used, when the user jumps over the text using ctrl+left or ctrl+right,
        or when the user removes sequences of characters using ctrl+delete or ctrl+backspace.
        */
        virtual bool IsSeparatorChar(char chr) const;

    private:
        
        std::string text_;          //!< Text field content
        size_t      cursorPos_ = 0; //!< Horizontal cursor position.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================