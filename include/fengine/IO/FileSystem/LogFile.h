/*
 * Log file header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_LOG_FILE_H__
#define __FORK_IO_LOG_FILE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "IO/Core/Log.h"

#include <fstream>
#include <string>
#include <vector>


namespace Fork
{

namespace IO
{


DECL_SHR_PTR(LogFile);

/**
A log file can be used to write multiple lines of text in a crash safe manner.
The file is only locked (or rather opened) during the write process of one text line.
*/
class FORK_EXPORT LogFile
{
    
    public:
        
        //! Default log file event handler which uses an output log file.
        class FORK_EXPORT LogEventHandler : public Log::EventHandler
        {
    
            public:
                
                LogEventHandler(const std::string& filename);

                LogEventHandler(const LogEventHandler&) = delete;
                LogEventHandler& operator = (const LogEventHandler&) = delete;

                void OnPrint(const std::string& message, const Log::EntryTypes type) override;

                void OnStartLn(const std::string& indent) override;
                void OnEndLn() override;

            protected:
                
                //! Returns a raw pointer to the log file, this event handler uses to output its content.
                inline LogFile* GetLogFile() const
                {
                    return logFile_.get();
                }

                /**
                The message stack will be 'flushed' when "OnEndLn" is called,
                i.e. it is written to file and then cleared.
                */
                std::string messageStack;

            private:
                
                //! This log file is a pointer, because of forward declaration problems.
                std::unique_ptr<LogFile> logFile_;

        };

        //! Log HTML file event handler which uses an output log file with highlithing.
        class FORK_EXPORT LogHTMLEventHandler : public LogEventHandler
        {
    
            public:
                
                //! Configuration structure for the HTML document type.
                struct FORK_EXPORT DocumentType
                {
                    //! Specifies the font name. By default "courier new".
                    std::string fontName        = "courier new";

                    //! Specifies the font size. By default 0.8 (in 'em' of HTML 4.01 strictness).
                    float       fontSize        = 0.8f;

                    /**
                    Specifies the default front color.
                    By default "FFFFFF" (bright foreground on dark background).
                    */
                    std::string frontColor      = "FFFFFF";

                    /**
                    Specifies the default back color.
                    By default "000030" (like in the editor's terminal).
                    */
                    std::string backColor       = "000030";

                    /**
                    Optional raw pointer to the doctype string.
                    If this is null, no doc type is added (<!DOCTYPE ...>).
                    By default the 'www.w3.org' doctype "strict.dtd" is used.
                    */
                    const char* docType         = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">";

                    /**
                    Specifies whether image filenames (BMP, PNG, JPG only)
                    are to be extended to HTML "img" tags. By default false.
                    */
                    bool        insertImages    = false;

                    /**
                    Specifies the size all images of the "img" tags are clamped to.
                    Set this to 0, when no clamping should be done. This has no effect
                    if 'insertImages' is false. By default 0.
                    \see insertImages
                    */
                    int         clampImageSize  = 0;
                };

                /**
                Log HTML constructor. This will immediately print the HTML header.
                \param[in] filename Specifies the HTML filename.
                \param[in] title Specifies the HTML header title (<title>...</title>).
                \param[in] documentType Specifies the document type configuration.
                */
                LogHTMLEventHandler(
                    const std::string& filename,
                    const std::string& title = "Log File",
                    const DocumentType& documentType = DocumentType()
                );
                virtual ~LogHTMLEventHandler();

                LogHTMLEventHandler(const LogHTMLEventHandler&) = delete;
                LogHTMLEventHandler& operator = (const LogHTMLEventHandler&) = delete;

                void OnPrint(const std::string& message, const Log::EntryTypes type) override;

                void OnStartLn(const std::string& indent) override;
                void OnEndLn() override;

                void OnPushColor(const Log::ColorFlags& frontColorFlags) override;
                void OnPushColor(const Log::ColorFlags& frontColorFlags, const Log::ColorFlags& backColorFlags) override;
                void OnPopColor() override;

            protected:
                
                /**
                Comfort function to convert the specified color flags into a hex color string.
                This is just a shortcut for the 'Platform::ConsoleManip::ToHexColor' function.
                \see Platform::ConsoleManip::ToHexColor
                */
                inline std::string ToHexColor(const Log::ColorFlags& colorFlags) const
                {
                    return Platform::ConsoleManip::ToHexColor(colorFlags);
                }

            private:
                
                void ExtractImageFilename();

                std::string indent_;
                bool        insertImages_   = false;
                int         clampImageSize_ = 128;

        };

        /**
        Creates the file and closes it immediately. The file resource will
        be locked (or rather opened) only when a new line is written.
        \throws FileOpenException If the file specified by 'filename' could not be created.
        */
        LogFile(const std::string& filename);

        LogFile(const LogFile&) = delete;
        LogFile& operator = (const LogFile&) = delete;

        /**
        Writes a new line to the log file.
        \param[in] text Specifies the text which is to be written to the log file.
        \return True on success otherwise the file could not be opened again.
        */
        bool WriteLine(const std::string& line);
        /**
        Writes several lines at once.
        \see WriteLine
        */
        bool WriteLines(const std::vector<std::string>& lines);

    private:

        std::string filename_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================