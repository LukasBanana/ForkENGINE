/*
 * Rich text model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/RichTextModel.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>
#include <sstream>


namespace Fork
{

namespace Utility
{


/* 
 * Internal functions
 */

/*
Converts the specified string into a list of strings,
one entry for each line (terminated by '\n').
*/
static std::vector<std::wstring> StringLines(const std::wstring& text)
{
    std::vector<std::wstring> lines;
    std::wstring line;

    /* Parse all string lines */
    std::wstringstream stream(text);
    while (std::getline(stream, line, L'\n'))
        lines.emplace_back(std::move(line));

    return lines;
}


/*
 * Task class
 */

RichTextModel::Task::~Task()
{
}

size_t RichTextModel::Task::TextSize() const
{
    return 0;
}


/*
 * SetFontTask class
 */

RichTextModel::Task::Types RichTextModel::SetFontTask::Type() const
{
    return Types::SetFont;
}


/*
 * SetColorTask class
 */

RichTextModel::Task::Types RichTextModel::SetColorTask::Type() const
{
    return Types::SetColor;
}


/*
 * PutTextTask class
 */

RichTextModel::Task::Types RichTextModel::PutTextTask::Type() const
{
    return Types::PutText;
}

size_t RichTextModel::PutTextTask::TextSize() const
{
    return text.size();
}


/*
 * Line class
 */

RichTextModel::Line::Line() :
    taskIt_{ tasks_.end() }
{
}

void RichTextModel::Line::IterateTasks()
{
    /* Initialize task iterator */
    taskIt_ = tasks_.begin();
}

const RichTextModel::Task* RichTextModel::Line::NextTask()
{
    /*
    Return current task if the end has not reached yet
    and iterate to the next one (post increment)
    */
    return (taskIt_ != tasks_.end()) ? (taskIt_++)->get() : nullptr;
}

void RichTextModel::Line::Replace(const std::wstring& subText, size_t start, size_t end)
{
    //...
}

void RichTextModel::Line::Insert(const std::wstring& text, size_t pos)
{
    /* Insert text into 'put-text' task and increase line width */
    auto task = FindTextTask(pos);
    task->text.insert(pos, text);
    width_ += text.size();
}


/*
 * ======= Private: =======
 */

RichTextModel::PutTextTask* RichTextModel::Line::FindTextTask(size_t& pos)
{
    /* Clamp position to [0 .. width] */
    pos = std::min(pos, GetWidth());

    size_t prevLen = 0;
    size_t len = 0;

    for (auto& task : tasks_)
    {
        /* Search 'put-text' task where the position is located */
        prevLen = len;
        len += task->TextSize();

        if (task->Type() != Task::Types::PutText)
            continue;

        auto textTask = static_cast<PutTextTask*>(task.get());

        if (len >= pos)
        {
            /* Found task, now trim the position to the tasks text beginning */
            pos -= prevLen;
            return textTask;
        }
    }

    /* Create new 'put-text' task */
    auto task = std::make_unique<PutTextTask>();
    auto taskRef = task.get();
    tasks_.push_back(std::move(task));

    return taskRef;
}


/*
 * DefaultBehavior class
 */

std::wstring RichTextModel::DefaultBehavior::SeparatorCharList() const
{
    return L" /\\.,:;()[]{}\"'";
}

bool RichTextModel::DefaultBehavior::OnExceededLineBegin(RichTextModel& richText, size_t& exceeding, bool dragCursor)
{
    if (richText.GetCursorEndPosition().y > 0)
    {
        /* Move cursor to the end of the upper line */
        richText.MoveCursorVert(-1);
        richText.MoveCursorEnd(dragCursor);

        /* Decrement exceeding, this is consumed by the <new line> jump */
        --exceeding;
        return true;
    }

    /* No further line exceeding possible */
    return false;
}

bool RichTextModel::DefaultBehavior::OnExceededLineEnd(RichTextModel& richText, size_t& exceeding, bool dragCursor)
{
    if (richText.GetCursorEndPosition().y + 1 < richText.NumLines())
    {
        /* Move cursor to the beginning of the lower line */
        richText.MoveCursorVert(1);
        richText.MoveCursorBegin(dragCursor);

        /* Decrement exceeding, this is consumed by the <new line> jump */
        --exceeding;
        return true;
    }

    /* No further line exceeding possible */
    return false;
}


/*
 * RichTextModel class
 */

RichTextModel::RichTextModel() :
    lineIt_{ lines_.end() }
{
    /* Initialize behavior and task iterator */
    SetupBehavior(nullptr);
}

void RichTextModel::IterateLines()
{
    /* Initialize line iterator */
    lineIt_ = lines_.begin();
}

const RichTextModel::Line* RichTextModel::NextLine()
{
    /*
    Return current line if the end has not reached yet
    and iterate to the next one (post increment)
    */
    return (lineIt_ != lines_.end()) ? (lineIt_++)->get() : nullptr;
}

void RichTextModel::Insert(const std::wstring& text)
{
    if (GetCurrentLine())
    {
        if (IsAreaSelected())
            ReplaceSelectedText(text);
        else
            InsertText(text);
    }
}

void RichTextModel::Remove(bool backwards, bool removeSequence)
{
    const auto sepCharList = behavior_->SeparatorCharList();

    //...
}

void RichTextModel::LocateCursor(CursorPosition position)
{
    /* Set new end position (first vertical, then horizontal!) and update start position */
    LocateCursorVert(position.y);
    LocateCursorHorz(position.x);
    SyncCursorPos();
}

void RichTextModel::DragCursor(CursorPosition position)
{
    /* Set new end position (first vertical, then horizontal!) */
    LocateCursorVert(position.y);
    LocateCursorHorz(position.x);
}

// Because of event handling, this function must never call "MoveCursorVert"!
void RichTextModel::MoveCursorHorz(int direction, bool dragCursor)
{
    /* Move end position */
    bool brokeExceeding = false;

    if (direction < 0)
    {
        /* Move left */
        auto movement = static_cast<size_t>(-direction);

        while (movement > CursorPos().x)
        {
            /* Move cursor to the line beginning */
            movement -= CursorPos().x;
            CursorPos().x = 0;

            /* Post 'exceeded line begin' event */
            if (!behavior_->OnExceededLineBegin(*this, movement, dragCursor))
            {
                brokeExceeding = true;
                break;
            }
        }

        if (!brokeExceeding)
        {
            /* Move remaining left direction */
            CursorPos().x -= movement;
        }
    }
    else if (direction > 0)
    {
        /* Move right */
        auto movement = static_cast<size_t>(direction);

        while (CursorPos().x + movement > LineDesc().width)
        {
            /* Move cursor to the line beginning */
            movement -= (LineDesc().width - CursorPos().x);
            CursorPos().x = LineDesc().width;

            /* Post 'exceeded line end' event */
            if (!behavior_->OnExceededLineEnd(*this, movement, dragCursor))
            {
                brokeExceeding = true;
                break;
            }
        }

        if (!brokeExceeding)
        {
            /* Move remaining left direction */
            CursorPos().x += movement;
        }
    }

    /* Move start position */
    SyncCursorPos(dragCursor);
}

//!TODO! -> must handling different font sizes!!!
// Because of event handling, this function must never call "MoveCursorHorz"!
void RichTextModel::MoveCursorVert(int direction, bool dragCursor)
{
    /* Move end position */
    if (direction < 0)
    {
        /* Move up */
        auto movement = static_cast<size_t>(-direction);

        if (movement > CursorPos().y)
            LocateCursorVert(0);
        else
            LocateCursorVert(CursorPos().y - movement);
    }
    else if (direction > 0)
    {
        /* Move down */
        auto movement = static_cast<size_t>(direction);

        if (CursorPos().y + movement >= NumLines())
        {
            if (lines_.empty())
                LocateCursorVert(0);
            else
                LocateCursorVert(NumLines() - 1);
        }
        else
            LocateCursorVert(CursorPos().y + movement);

    }

    /* Clamp cursor X position */
    LocateCursorHorz(CursorPos().x);

    /* Move start position */
    SyncCursorPos(dragCursor);
}

void RichTextModel::MoveCursorBegin(bool dragCursor)
{
    LocateCursorHorz(0);
    SyncCursorPos(dragCursor);
}

void RichTextModel::MoveCursorEnd(bool dragCursor)
{
    LocateCursorHorz(LineDesc().width);
    SyncCursorPos(dragCursor);
}

void RichTextModel::MoveCursorTop(bool dragCursor)
{
    LocateCursorVert(0);
    SyncCursorPos(dragCursor);
}

void RichTextModel::MoveCursorBottom(bool dragCursor)
{
    LocateCursorVert(lines_.empty() ? 0 : NumLines() - 1);
    SyncCursorPos(dragCursor);
}

RichTextModel::Line::Description RichTextModel::LineDesc() const
{
    Line::Description desc;
    {
        desc.index = CursorPos().y;
        desc.width = (GetCurrentLine() != nullptr ? GetCurrentLine()->GetWidth() : 0);
    }
    return desc;
}

void RichTextModel::JumpLeft()
{
    const auto sepCharList = behavior_->SeparatorCharList();

    //...
}

void RichTextModel::JumpRight()
{
    const auto sepCharList = behavior_->SeparatorCharList();

    //...
}

void RichTextModel::ChangeFont(const Video::Font::Description& fontDesc)
{
    //...
}

void RichTextModel::ChangeColor(const Video::ColorRGBAf& color)
{
    //...
}

void RichTextModel::SetupBehavior(const BehaviorPtr& behavior)
{
    if (behavior)
        behavior_ = behavior;
    else
        behavior_ = std::make_shared<DefaultBehavior>();
}


/*
 * ======= Private: =======
 */

/*
The Y position must be clamped in the range [0 .. lines-count).
The X position must be clamped in the range [0 .. line-width].
*/
void RichTextModel::ClampPosition(CursorPosition& position) const
{
    if (!lines_.empty())
    {
        position.y = std::min(position.y, NumLines() - 1);
        position.x = std::min(position.x, lines_[position.y]->GetWidth());
    }
    else
        position = { 0, 0 };
}

void RichTextModel::LocateCursorHorz(size_t positionX)
{
    /* Set new X position */
    CursorPos().x = std::min(positionX, LineDesc().width);
}

void RichTextModel::LocateCursorVert(size_t positionY)
{
    if (!lines_.empty())
    {
        /* Set new Y position and update current line */
        CursorPos().y = std::min(positionY, NumLines() - 1);
        currentLine_ = lines_[CursorPos().y].get();
    }
    else
    {
        /* Clear cursor Y position and current line */
        CursorPos().y = 0;
        currentLine_ = nullptr;
    }
}

void RichTextModel::SyncCursorPos(bool dragCursor)
{
    if (dragCursor)
        cursorStartPosition_ = cursorEndPosition_;
}

void RichTextModel::ReplaceSelectedText(const std::wstring& text)
{
    //...
}

void RichTextModel::InsertText(const std::wstring& text)
{
    auto lines = StringLines(text);

    if (lines.size() > 1)
    {
        //...
    }
    else if (!lines.empty())
        GetCurrentLine()->Insert(lines.front(), CursorPos().x);
}


} // /namespace Utility

} // /namespace Fork



// ========================