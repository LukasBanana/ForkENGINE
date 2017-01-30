/*
 * Terminal model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/TerminalModel.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/Texture/Texture1D.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/Texture3D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Utility
{


/*
 * TextEntry class
 */

TerminalModel::TextEntry::TextEntry(
    const Video::TextureFont* font, const std::wstring& text, const Video::ColorRGBf& color, bool indicateNewLine) :
        Entry   { indicateNewLine },
        font_   { font            },
        text_   { text            },
        color_  { color           }
{
    ASSERT_POINTER(font);
    RemoveTB(text_);
    size_ = font_->TextSize(text_);
}

void TerminalModel::TextEntry::ChangeText(const std::wstring& text)
{
    text_ = text;
    RemoveTB(text_);
    size_ = font_->TextSize(text_);
}

TerminalModel::Entry::Types TerminalModel::TextEntry::Type() const
{
    return TerminalModel::Entry::Types::Text;
}


/*
 * SourceCodeEntry class
 */

TerminalModel::SourceCodeEntry::SourceCodeEntry(const Video::TextureFont* font, const std::string& codeLine) :
    codeLine_{ codeLine }
{
    ASSERT_POINTER(font);
    size_ = font->TextSize(codeLine_);
}

TerminalModel::Entry::Types TerminalModel::SourceCodeEntry::Type() const
{
    return TerminalModel::Entry::Types::SourceCode;
}


/*
 * TextureEntry class
 */

TerminalModel::TextureEntry::TextureEntry(const Video::TexturePtr& texture) :
    texture_{ texture }
{
    ASSERT_POINTER(texture);

    switch (texture_->Type())
    {
        case Video::Texture::Types::Tex1D:
        {
            auto tex = dynamic_cast<Video::Texture1D*>(texture_.get());
            size_ = { tex->GetSize(), 1 };
            numSlices_ = tex->GetArraySize();
        }
        break;

        case Video::Texture::Types::Tex2D:
        {
            auto tex = dynamic_cast<Video::Texture2D*>(texture_.get());
            size_ = tex->GetSize();
            numSlices_ = tex->GetArraySize();
        }
        break;

        case Video::Texture::Types::Tex3D:
        {
            auto tex = dynamic_cast<Video::Texture3D*>(texture_.get());
            size_ = tex->GetSize().Sz2();
            numSlices_ = tex->GetSize().depth;
        }
        break;

        case Video::Texture::Types::TexCube:
        {
            auto tex = dynamic_cast<Video::TextureCube*>(texture_.get());
            size_ = tex->GetSize();
            numSlices_ = tex->GetArraySize();
        }
        break;
    }
}

TerminalModel::Entry::Types TerminalModel::TextureEntry::Type() const
{
    return TerminalModel::Entry::Types::Texture;
}


/*
 * TerminalModel class
 */

TerminalModel::TerminalModel()
{
    scrollBar.SetupMinBarSize(10);
}
TerminalModel::~TerminalModel()
{
}

void TerminalModel::Insert(char chr)
{
    if (mode == Modes::Script && chr == '\t')
    {
        TextFieldModel::Insert(' ');
        TextFieldModel::Insert(' ');
    }
    else
        TextFieldModel::Insert(chr);
}

void TerminalModel::Insert(const std::string& text)
{
    TextFieldModel::Insert(text);
}

void TerminalModel::AddEntry(const EntryPtr& entry)
{
    if (entry)
    {
        /* Add entry and keep track of maximal scroll position */
        entries_.push_back(entry);

        //!INCOMPLETE!
        if (entry->IndicateNewLine())
        {
            scrollBar.SetupAreaSize(
                scrollBar.GetAreaSize() + entry->Size().height
            );
        }
    }
}

void TerminalModel::ClearEntries()
{
    /* Clear entry list */
    entries_.clear();

    /* Reset scroll position */
    scrollBar.SetupAreaSize(0.0f);
    scrollBar.ScrollBegin();
}

std::string TerminalModel::LastScriptCode() const
{
    std::string sourceCode;

    bool startedCollecting = false;

    for (auto it = entries_.rbegin(); it != entries_.rend(); ++it)
    {
        if ((*it)->Type() == TerminalModel::Entry::Types::SourceCode)
        {
            auto codeEntry = dynamic_cast<SourceCodeEntry*>(it->get());

            startedCollecting = true;
            sourceCode = codeEntry->GetCodeLine() + "\n" + sourceCode;
        }
        else if (startedCollecting)
            break;
    }

    return sourceCode;
}


} // /namespace Utility

} // /namespace Fork



// ========================