/*
 * Log file file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/LogFile.h"
#include "IO/FileSystem/FileOpenException.h"
#include "Core/StringModifier.h"

#include "Platform/Core/SystemInfo.h"//!!!


namespace Fork
{

namespace IO
{


/*
 * LogEventHandler class
 */

LogFile::LogEventHandler::LogEventHandler(const std::string& filename) :
    logFile_{ std::make_unique<LogFile>(filename) }
{
}

void LogFile::LogEventHandler::OnPrint(const std::string& message, const Log::EntryTypes type)
{
    messageStack += message;
}

void LogFile::LogEventHandler::OnStartLn(const std::string& indent)
{
    messageStack += indent;
}

void LogFile::LogEventHandler::OnEndLn()
{
    logFile_->WriteLine(messageStack);
    messageStack.clear();
}


/*
 * LogHTMLEventHandler class
 */

LogFile::LogHTMLEventHandler::LogHTMLEventHandler(
    const std::string& filename, const std::string& title, const DocumentType& documentType) :
        LogEventHandler { filename                    },
        insertImages_   { documentType.insertImages   },
        clampImageSize_ { documentType.clampImageSize }
{
    if (documentType.docType)
        GetLogFile()->WriteLine(documentType.docType);

    GetLogFile()->WriteLines(
        {
            "<html>",
            "\t<head>",
            "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">",
            "\t\t<title>" + title + "</title>",
            "\t\t<style type=\"text/css\">",
            "\t\t\tbody { background-color:#" + documentType.backColor + "; font-family:" + documentType.fontName + "; }",
            "\t\t</style>",
            "\t</head>",
            "\t<body>",
            "\t\t<p>",
            "\t\t<span style=\"font-size: " + ToStr(documentType.fontSize) + "em; color: #" + documentType.frontColor + "\">"
        }
    );
}
LogFile::LogHTMLEventHandler::~LogHTMLEventHandler()
{
    GetLogFile()->WriteLines(
        {
            "\t\t</span>",
            "\t\t</p>",
            "\t</body>",
            "</html>"
        }
    );
}

void LogFile::LogHTMLEventHandler::OnPrint(const std::string& message, const Log::EntryTypes type)
{
    auto msg = message;

    /* Replace special HTML characters in message string */
    ReplaceString(msg, "&", "&amp;");
    ReplaceString(msg, "<", "&lt;");
    ReplaceString(msg, ">", "&gt;");
    ReplaceString(msg, " ", "&nbsp;");

    messageStack += msg;
}

void LogFile::LogHTMLEventHandler::OnStartLn(const std::string& indent)
{
    OnPrint(indent, Log::EntryTypes::Meta);
    indent_ = messageStack;
}

void LogFile::LogHTMLEventHandler::OnEndLn()
{
    GetLogFile()->WriteLine("\t\t\t" + messageStack + "<br>");

    if (insertImages_)
        ExtractImageFilename();

    messageStack.clear();
}

void LogFile::LogHTMLEventHandler::OnPushColor(const Log::ColorFlags& frontColorFlags)
{
    messageStack += "<span style=\"color: #" + ToHexColor(frontColorFlags) + "\">";
}

void LogFile::LogHTMLEventHandler::OnPushColor(const Log::ColorFlags& frontColorFlags, const Log::ColorFlags& backColorFlags)
{
    OnPushColor(frontColorFlags);
}

void LogFile::LogHTMLEventHandler::OnPopColor()
{
    messageStack += "</span>";
}

void LogFile::LogHTMLEventHandler::ExtractImageFilename()
{
    /* Find start- and end '\"' characters */
    auto start = messageStack.find('\"');
    if (start == std::string::npos)
        return;
    
    auto end = messageStack.find('\"', start + 1);
    if (end == std::string::npos)
        return;
        
    /* Extract potential filename */
    auto filename = messageStack.substr(start + 1, end - start - 1);

    /* Check if file can be read */
    std::ifstream file(filename);
    if (!file.good())
        return;
    file.close();
    
    /* Extract filename extension and check if webbrowser supports it */
    auto fileExt = ToLower(ExtractFileExt(filename));
    if (fileExt != "bmp" && fileExt != "png" && fileExt != "jpg")
        return;
    
    /*
    !!!TODO!!!
    Remove absolute path from filename if included!
    */
    /* Check if filename must be extended by absolute path */
    if (filename.find(':') == std::string::npos)
    {
        auto workDir = UTF16toUTF8(Platform::QueryWorkingDirectory());
        ReplaceString(workDir, "\\", "/");
        filename = workDir + "/" + filename;
    }

    /* Build additional HTML line */
    std::string line = "\t\t\t" + indent_ + "<img src=\"file://" + filename + "\"";

    if (clampImageSize_ > 0)
    {
        auto sizeStr = ToStr(clampImageSize_);
        line += " width=\"" + sizeStr + "\" height=\"" + sizeStr + "\"";
    }

    line += "><br>";

    /* Write final HTML "img" tag line */
    GetLogFile()->WriteLine(line);
}


/*
 * LogFile class
 */

LogFile::LogFile(const std::string& filename) :
    filename_(filename)
{
    std::ofstream stream(filename_, std::ofstream::out);
    if (!stream.good())
        throw FileOpenException(filename_, "Creating log file failed");
}

bool LogFile::WriteLine(const std::string& line)
{
    std::ofstream stream(filename_, std::ios_base::out | std::ios_base::app);

    if (stream.good())
    {
        stream << line << std::endl;
        return true;
    }

    return false;
}

bool LogFile::WriteLines(const std::vector<std::string>& lines)
{
    std::ofstream stream(filename_, std::ios_base::out | std::ios_base::app);

    if (stream.good())
    {
        for (const auto& line : lines)
            stream << line << std::endl;
        return true;
    }

    return false;
}


} // /namespace IO

} // /namespace Fork



// ========================