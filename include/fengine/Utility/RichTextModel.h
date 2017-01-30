/*
 * Rich text model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_RICH_TEXT_MODEL_H__
#define __FORK_UTILITY_RICH_TEXT_MODEL_H__


#include "Core/Export.h"
#include "Math/Core/Vector2.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Font/TextureFont.h"

#include <string>
#include <vector>
#include <memory>


namespace Fork
{

namespace Utility
{


/**
Base class for rich text area elements in a user interface.
\todo Incomplete (This will be used for the Terminal).
*/
class FORK_EXPORT RichTextModel
{

    public:

        //! Type used for the cursor position in this richt-text model.
        typedef Math::Point2st CursorPosition;

        /* === Classes === */

        /**
        Base structure for the rich-text tasks.
        \remarks A 'task' here can set the new font, the new color, put text or makes a new line.
        */
        struct FORK_EXPORT Task
        {
            enum class Types
            {
                SetFont,    //!< \see SetFontTask
                SetColor,   //!< \see SetColorTask
                PutText,    //!< \see PutTextTask
            };
            virtual ~Task();
            //! Returns the type of this task.
            virtual Types Type() const = 0;
            //! Returns the number of characters or zero if this task has no text.
            virtual size_t TextSize() const;
        };

        //! This task sets a new font for further text drawing.
        struct FORK_EXPORT SetFontTask : public Task
        {
            Types Type() const override;
            Video::TextureFontPtr font;
        };

        //! This task sets a new color for further text drawing.
        struct FORK_EXPORT SetColorTask : public Task
        {
            Types Type() const override;
            Video::ColorRGBAf color;
        };

        //! This task puts more text.
        struct FORK_EXPORT PutTextTask : public Task
        {
            Types Type() const override;
            size_t TextSize() const override;
            std::wstring text;
        };

        /* === Classes === */

        //! Rich-text line.
        class FORK_EXPORT Line
        {
            
            public:
                
                //! Line meta data structure.
                struct Description
                {
                    size_t index = 0; //!< Index of this line.
                    size_t width = 0; //!< Number of characters in this line.
                };

                Line();

                /* <--- Copy constructor must be deleted because of 'vector<unique_ptr<Task>>' list ---> */
                Line(const Line&) = delete;
                Line& operator = (const Line&) = delete;

                void IterateTasks();
                const Task* NextTask();

                //! Replaces the content of this line with the sub-text at the specified position.
                void Replace(const std::wstring& subText, size_t start, size_t end);
                //! Inserts the text at the specified position.
                void Insert(const std::wstring& text, size_t pos);

                //! Returns the line width. This is the number of characters in this line.
                inline size_t GetWidth() const
                {
                    return width_;
                }

            private:

                typedef std::vector<std::unique_ptr<Task>> TaskList;

                /**
                Returns the "PutTextTask" object where the specified position is located.
                \param[in,out] pos Specifies the position where the task is to be searched.
                This will be clamped to [0 .. width].
                \remarks If the current task list is empty, the first "PutTextTask" will be created,
                thus this function never returns null!
                \see GetWidth
                */
                PutTextTask* FindTextTask(size_t& pos);

                TaskList            tasks_;     //!< List of all tasks in this line.
                TaskList::iterator  taskIt_;

                size_t              width_ = 0; //!< Number of characters in this line.
                
        };

        //! Rich-text behavior interface.
        class FORK_EXPORT Behavior
        {
            
            public:
                
                virtual ~Behavior()
                {
                }

                /**
                Returns the char array of separator characters. This is used for fast cursor movements.
                \see RichtTextModel::JumpLeft
                \see RichtTextModel::JumpRight
                */
                virtual std::wstring SeparatorCharList() const = 0;

                /**
                This is called when the cursor movement exceeded the beginning of the current line.
                \param[in,out] richText Specifies the rich text from which the event was posted.
                \param[in,out] exceeding Specifies the cursor exceeding. This is always greater than zero!
                \param[in] dragCursor Specifies whether the cursor is dragged or not.
                \return False if cursor movement is to be canceled.
                \see RichTextModel::MoveCursor
                */
                virtual bool OnExceededLineBegin(RichTextModel& richText, size_t& exceeding, bool dragCursor) = 0;
                /**
                This is called when the cursor movement exceeded the end of the current line.
                \param[in,out] richText Specifies the rich text from which the event was posted.
                \param[in,out] exceeding Specifies the cursor exceeding. This is always greater than zero!
                \param[in] dragCursor Specifies whether the cursor is dragged or not.
                \return False if cursor movement is to be canceled.
                \see RichTextModel::MoveCursor
                */
                virtual bool OnExceededLineEnd(RichTextModel& richText, size_t& exceeding, bool dragCursor) = 0;

        };

        //! Rich-text default behavior.
        class FORK_EXPORT DefaultBehavior : public Behavior
        {
            
            public:
                
                //! Returns L" /\\.,:;()[]{}\"'".
                std::wstring SeparatorCharList() const override;

                //! This will move the cursor to the end of the next upper line.
                bool OnExceededLineBegin(RichTextModel& richText, size_t& exceeding, bool dragCursor) override;
                //! This will move the cursor to the beginning of the next lower line.
                bool OnExceededLineEnd(RichTextModel& richText, size_t& exceeding, bool dragCursor) override;

        };

        typedef std::shared_ptr<Behavior> BehaviorPtr;

        /* === Functions === */

        RichTextModel();

        /* <--- Copy constructor must be deleted because of 'vector<unique_ptr<Task>>' list ---> */
        RichTextModel(const RichTextModel&) = delete;
        RichTextModel& operator = (const RichTextModel&) = delete;

        /**
        Starts iterating over all tasks. This must be called before "NextTask" can be used.
        \see NextTask
        */
        void IterateLines();
        /**
        Returns the next task and iterates to the next one.
        "IterateTasks" must be called before iteration can be started.
        \see IterateTasks
        */
        const Line* NextLine();

        /**
        Inserts the specified text at the current cursor position.
        If the cursor is currently selecting a range of characters,
        all of them will be replaced by this character.
        \see IsAreaSelected
        */
        void Insert(const std::wstring& text);

        /**
        Removes a single character at the current cursor position.
        If the cursor is currently selecting a range of characters,
        all of them will be removed.
        \param[in] backwards Specifies whether the character behind (true)
        or before (false) the current cursor position will be removed. By default false.
        \see IsAreaSelected
        */
        void Remove(bool backwards = false, bool removeSequence = false);

        /**
        Moves the cursor start- and end positions.
        \see GetCursorStartPosition
        \see GetCursorEndPosition
        */
        void LocateCursor(CursorPosition position);
        /**
        Moves the cursor end position.
        \see GetCursorEndPosition
        */
        void DragCursor(CursorPosition position);

        //! Moves the cursor into the specified horizontal direction.
        void MoveCursorHorz(int direction, bool dragCursor = false);
        //! Moves the cursor into the specified vertical direction.
        void MoveCursorVert(int direction, bool dragCursor = false);

        //! Moves the cursor to the beginning of the current line.
        void MoveCursorBegin(bool dragCursor = false);
        //! Moves the cursor to the end of the current line.
        void MoveCursorEnd(bool dragCursor = false);

        //! Moves the cursor to the top.
        void MoveCursorTop(bool dragCursor = false);
        //! Moves the cursor to the bottom.
        void MoveCursorBottom(bool dragCursor = false);

        /**
        Returns the meta data of the current line.
        \see GetCurrentLine
        */
        Line::Description LineDesc() const;

        /**
        Jumps to the next left sided space. The 'separator character list' specifies which characters can skipped.
        \see Behavior::SeparatorCharList
        */
        void JumpLeft();
        /**
        Jumps to the next right sided space. The 'separator character list' specifies which characters can skipped.
        \see Behavior::SeparatorCharList
        */
        void JumpRight();

        /**
        Changes the font for further text or only for the selected text area, if something is selected.
        \see IsAreaSelected
        */
        void ChangeFont(const Video::Font::Description& fontDesc);
        /**
        Changes the color for further text or only for the selected text area, if something is selected.
        \see IsAreaSelected
        */
        void ChangeColor(const Video::ColorRGBAf& color);

        /**
        Sets the new bahavior.
        \param[in] behavior Specifies the new behavior object.
        If this is null, the default behavior will be set.
        \see Behavior
        \see DefaultBehavior
        */
        void SetupBehavior(const BehaviorPtr& behavior);

        /**
        Returns the cursor start position.
        \see LocateCursor
        */
        inline const CursorPosition& GetCursorStartPosition() const
        {
            return cursorStartPosition_;
        }
        /**
        Returns the cursor end position.
        \see LocateCursor
        \see DragCursor
        */
        inline const CursorPosition& GetCursorEndPosition() const
        {
            return cursorEndPosition_;
        }

        /**
        Returns true if the cursor is currently in a selection.
        This is the case, when the cursor's start- and end positions differ.
        */
        inline bool IsAreaSelected() const
        {
            return cursorStartPosition_ != cursorEndPosition_;
        }

        /**
        Returns the current line or null if this rich-text model has no line.
        \remarks This is the line where the cursor end position is located.
        \see GetCursorEndPosition
        \see NumLines
        */
        inline Line* GetCurrentLine() const
        {
            return currentLine_;
        }

        //! Returns the number of lines.
        inline size_t NumLines() const
        {
            return lines_.size();
        }

    private:
        
        typedef std::vector<std::unique_ptr<Line>> LineList;

        /* === Functions === */

        void ClampPosition(CursorPosition& position) const;

        void LocateCursorHorz(size_t positionX);
        void LocateCursorVert(size_t positionY);

        void SyncCursorPos(bool dragCursor = true);

        void ReplaceSelectedText(const std::wstring& text);
        void InsertText(const std::wstring& text);

        //! Returns the main cursor position (this is the cursor end position).
        inline CursorPosition& CursorPos()
        {
            return cursorEndPosition_;
        }
        inline const CursorPosition& CursorPos() const
        {
            return cursorEndPosition_;
        }

        /* === Members === */

        LineList            lines_;
        LineList::iterator  lineIt_;

        CursorPosition      cursorStartPosition_;
        CursorPosition      cursorEndPosition_;
        Line*               currentLine_ = nullptr;

        BehaviorPtr         behavior_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================