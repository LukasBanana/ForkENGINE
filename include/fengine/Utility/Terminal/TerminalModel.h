/*
 * Terminal model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_TERMINAL_MODEL_H__
#define __FORK_UTILITY_TERMINAL_MODEL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/TextFieldModel.h"
#include "Utility/ScrollBarModel.h"
#include "Video/Core/ColorRGBA.h"
#include "Math/Core/Size2.h"
#include "Video/Font/TextureFont.h"

#include <vector>
#include <memory>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(TerminalModel);

//! Terminal model class.
class FORK_EXPORT TerminalModel : public TextFieldModel
{

    public:
        
        /* === Enumerations === */

        //! Terminal use modes.
        enum class Modes
        {
            Default,    //!< Default mode. Enter commands.
            Script,     //!< Enter lines of script code.
        };

        /* === Classes === */

        //! Base entry interface.
        class FORK_EXPORT Entry
        {
            
            public:
                
                enum class Types
                {
                    Text,       //!< Standard text entry.
                    SourceCode, //!< Source code text entry.
                    Texture,    //!< Texture entry.
                };

                virtual ~Entry()
                {
                }

                /**
                Returns the entry's type.
                \see TerminalModel::Entry::Types
                */
                virtual Types Type() const = 0;

                //! Returns the entry's size.
                inline const Math::Size2i& Size() const
                {
                    return size_;
                }

                //! Returns true if this entry indicates a new-line.
                inline bool IndicateNewLine() const
                {
                    return indicateNewLine_;
                }

            protected:
                
                Entry(bool indicateNewLine = true) :
                    indicateNewLine_(indicateNewLine)
                {
                }

                bool indicateNewLine_ = true;
                Math::Size2i size_;

        };

        typedef std::shared_ptr<Entry> EntryPtr;

        //! Text entry.
        class FORK_EXPORT TextEntry : public Entry
        {
            
            public:
                
                //! \throws NullPointerException If the passed pointer is null.
                TextEntry(
                    const Video::TextureFont* font,
                    const std::wstring& text,
                    const Video::ColorRGBf& color,
                    bool indicateNewLine = true
                );
                
                Types Type() const;

                void ChangeText(const std::wstring& text);

                inline const Video::TextureFont* GetFont() const
                {
                    return font_;
                }
                inline const std::wstring& GetText() const
                {
                    return text_;
                }
                inline const Video::ColorRGBf& GetColor() const
                {
                    return color_;
                }

            private:

                const Video::TextureFont* font_ = nullptr;
                std::wstring text_;
                Video::ColorRGBf color_;
        };

        //! Source code entry.
        class FORK_EXPORT SourceCodeEntry : public Entry
        {
            
            public:
                
                SourceCodeEntry() = default;
                //! \throws NullPointerException If the passed pointer is null.
                SourceCodeEntry(const Video::TextureFont* font, const std::string& codeLine);

                Types Type() const;

                inline const std::string& GetCodeLine() const
                {
                    return codeLine_;
                }

            private:
                
                std::string codeLine_;

        };

        //! Texture entry.
        class FORK_EXPORT TextureEntry : public Entry
        {
            
            public:
                
                //! \throws NullPointerException If the passed pointer is null.
                TextureEntry(const Video::TexturePtr& texture);

                Types Type() const;

                inline unsigned int GetNumSlices() const
                {
                    return numSlices_;
                }

                inline const Video::Texture* GetTexture() const
                {
                    return texture_.get();
                }

            private:
                
                Video::TexturePtr texture_;
                unsigned int numSlices_ = 1;

        };

        /* === Functions === */

        TerminalModel();
        virtual ~TerminalModel();

        /**
        Inserts the specified character. Also converts some special characters:
        - '\t' into two ' ' characters.
        \see TextFieldModel::Insert(char)
        */
        void Insert(char chr) override;
        /**
        Equivalent to "TextFieldModel::Insert".
        \see TextFieldModel::Insert(const std::string&)
        */
        void Insert(const std::string& text) override;

        //! Adds a new entry to the terminal (if it's not a null pointer).
        void AddEntry(const EntryPtr& entry);
        //! Clears the terminal entries.
        void ClearEntries();

        //! Returns the source code (as string) of the last entered script.
        std::string LastScriptCode() const;

        //! Returns the entries list.
        inline const std::vector<EntryPtr>& GetEntries() const
        {
            return entries_;
        }

        //! Terminal model scroll bar.
        ScrollBarModel scrollBar;

        //! Current terminal use mode.
        Modes mode = Modes::Default;
        
    private:
        
        std::vector<EntryPtr> entries_; //!< Terminal entry list.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================