/*
 * Log file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/Log.h"
#include "IO/Core/Console.h"
#include "Core/STLHelper.h"

#include <fstream>
#include <map>
#include <algorithm>
#include <regex>
#include <stack>


namespace Fork
{

namespace IO
{

namespace Log
{


using namespace Platform::ConsoleManip;

/* === Internal members === */

/*
Internal log state.
Stores information about the output device and configuration.
*/
struct InternalState
{
    std::vector<EventHandlerPtr>    eventHandlers;
    std::map<std::string, int>      debugMessageCounter;

    std::string                     indentFull;
    std::string                     indent { "  " };

    std::stack<size_t>              indentSizeStack;
};

static InternalState internalState;

static const ColorFlags debugColor = Colors::Pink | Colors::Intens;


/* === Internal functions === */

static void PushFrontColor(const ColorFlags& frontColorFlags)
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnPushColor(frontColorFlags);
        }
    );
};

static void PushFrontAndBackColor(const ColorFlags& frontColorFlags, const ColorFlags& backColorFlags)
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnPushColor(frontColorFlags, backColorFlags);
        }
    );
};

static void PopColor()
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnPopColor();
        }
    );
};

static void StartLn()
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnStartLn(internalState.indentFull);
        }
    );
}

static void EndLn()
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnEndLn();
        }
    );
}

static int MessageLimit(const std::string& message, int messageLimit)
{
    /* Check if message should be skiped */
    if (messageLimit > 0)
    {
        auto& counter = internalState.debugMessageCounter[message];

        /* Increase message counter */
        if (counter <= messageLimit)
            ++counter;

        return counter;
    }
    return -1;
}


/* === Function implementations === */

/* --- Event handling --- */

FORK_EXPORT void AddEventHandler(const EventHandlerPtr& eventHandler)
{
    if (eventHandler)
        internalState.eventHandlers.push_back(eventHandler);
}

FORK_EXPORT void RemoveEventHandler(const EventHandlerPtr& eventHandler)
{
    RemoveFromList(internalState.eventHandlers, eventHandler);
}

FORK_EXPORT void ClearEventHandlers()
{
    internalState.eventHandlers.clear();
}

FORK_EXPORT EventHandlerPtr AddDefaultEventHandler()
{
    auto eventHandler = std::make_shared<Console::LogEventHandler>();
    AddEventHandler(eventHandler);
    return eventHandler;
}

/* --- Indentation --- */

FORK_EXPORT void SetIndent(const std::string& indent)
{
    internalState.indent = indent;
}

FORK_EXPORT const std::string& GetIndent()
{
    return internalState.indent;
}

FORK_EXPORT const std::string& GetFullIndent()
{
    return internalState.indentFull;
}

FORK_EXPORT void IncIndent()
{
    internalState.indentFull += internalState.indent;
    internalState.indentSizeStack.push(internalState.indent.size());
}

FORK_EXPORT void DecIndent()
{
    if (!internalState.indentSizeStack.empty())
    {
        /* Remove previous indent size from the full indentation */
        auto size = internalState.indentSizeStack.top();
        internalState.indentFull.resize(internalState.indentFull.size() - size);
        internalState.indentSizeStack.pop();
    }
    else
        internalState.indentFull.clear();
}

/* --- Messages --- */

FORK_EXPORT void Message(const std::string& message, const EntryTypes type)
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnPrintLn(internalState.indentFull, message, type);
        }
    );
}

FORK_EXPORT void Message(const std::string& message, const ColorFlags colorFlags, const EntryTypes type)
{
    PushFrontColor(colorFlags);
    Message(message, type);
    PopColor();
}

FORK_EXPORT void Message(const std::string& message, const ColorFlags colorFlagsFront, const ColorFlags colorFlagsBack, const EntryTypes type)
{
    PushFrontAndBackColor(colorFlagsFront, colorFlagsBack);
    Message(message, type);
    PopColor();
}

FORK_EXPORT void MessageColored(const std::string& message, const EntryTypes type)
{
    auto msg = message;

    /* Define color flags extraction lambda function */
    auto ExtractColorFlags = [](const std::string& str) -> Colors::DataType
    {
        Colors::DataType flags = 0;

        if (str.size() == 4)
        {
            if (str[0] == '1') flags |= Colors::Red;
            if (str[1] == '1') flags |= Colors::Green;
            if (str[2] == '1') flags |= Colors::Blue;
            if (str[3] == '1') flags |= Colors::Intens;
        }

        return flags;
    };

    auto PrintSub = [&type](const std::string& msg)
    {
        ForEach(
            internalState.eventHandlers,
            [&](EventHandlerPtr& evtHandler)
            {
                evtHandler->OnPrint(msg, type);
            }
        );
    };

    /* Setup regular expressions */
    const std::regex regExMain("</>|<[01]{4}>|<[01]{4}\\|[01]{4}>|&amp;|&lt;|&gt;");
    std::smatch match;

    int colorPushCounter = 0;

    /* Start message print */
    StartLn();

    while (std::regex_search(msg, match, regExMain))
    {
        /* Get tag attribute */
        auto attrStr = match.begin()->str();
        auto prefixStr = match.prefix().str();

        /* Print prefix message part */
        PrintSub(prefixStr);

        if (attrStr == "&amp;")
            PrintSub("&");
        else if (attrStr == "&lt;")
            PrintSub("<");
        else if (attrStr == "&gt;")
            PrintSub(">");
        else
        {
            attrStr = attrStr.substr(1, attrStr.size() - 2);

            switch (attrStr.size())
            {
                case 1:
                    if (colorPushCounter > 0)
                    {
                        PopColor();
                        --colorPushCounter;
                    }
                    break;
                case 4:
                    PushFrontColor(ExtractColorFlags(attrStr));
                    ++colorPushCounter;
                    break;
                case 9:
                    PushFrontAndBackColor(
                        ExtractColorFlags(attrStr.substr(0, 4)),
                        ExtractColorFlags(attrStr.substr(5, 4))
                    );
                    ++colorPushCounter;
                    break;
            }
        }

        /* Trim message string */
        msg = match.suffix().str();
    }

    /* Print rest of the message */
    PrintSub(msg);

    /* End message print */
    EndLn();

    /* Pop remaining color flags */
    for (; colorPushCounter > 0; --colorPushCounter)
        PopColor();
}

FORK_EXPORT void Success(const std::string& message)
{
    Message(message, Colors::Green | Colors::Intens);
}

FORK_EXPORT void Warning(const std::string& message)
{
    Message("Warning: " + message + "!", Colors::Yellow | Colors::Intens, EntryTypes::Warning);
}

FORK_EXPORT void Blank()
{
    ForEach(
        internalState.eventHandlers,
        [&](EventHandlerPtr& evtHandler)
        {
            evtHandler->OnBlank();
        }
    );
}

/* --- Debugging --- */

FORK_EXPORT void Debug(const std::string& message, int messageLimit)
{
    auto limit = MessageLimit(message, messageLimit);
    
    if (limit >= 0 && limit == messageLimit)
        Message("Debug: " + message + " --> Skip Message Trail", debugColor, EntryTypes::Debug);
    else if (limit < messageLimit || messageLimit == debugMessageInfinite)
        Message("Debug: " + message, debugColor, EntryTypes::Debug);
}

FORK_EXPORT void Debug(const std::string& operationDesc, const std::string& message, int messageLimit)
{
    auto limit = MessageLimit(message, messageLimit);
    
    if (limit >= 0 && limit == messageLimit)
    {
        MessageColored(
            "<1011>Debug: <1001>[" + operationDesc + "]</> -- " +
            message + " --> Skip Message Trail</>",
            EntryTypes::Debug
        );
    }
    else if (limit < messageLimit || messageLimit == debugMessageInfinite)
    {
        MessageColored(
            "<1011>Debug: <1001>[" + operationDesc + "]</> -- " + message + "</>",
            EntryTypes::Debug
        );
    }
}

FORK_EXPORT void Debug(
    const std::string& errorPrefix, const std::string& operationDesc,
    const std::string& message, int messageLimit)
{
    auto limit = MessageLimit(message, messageLimit);
    
    if (limit >= 0 && limit == messageLimit)
    {
        MessageColored(
            "<1011>Debug: " + errorPrefix + " <1001>[" + operationDesc +
            "]</> -- " + message + " --> Skip Message Trail</>",
            EntryTypes::Debug
        );
    }
    else if (limit < messageLimit || messageLimit == debugMessageInfinite)
    {
        MessageColored(
            "<1011>Debug: " + errorPrefix + " <1001>[" + operationDesc +
            "]</> -- " + message + "</>",
            EntryTypes::Debug
        );
    }
}

/* --- Errors --- */

FORK_EXPORT void Error(const std::string& message)
{
    Message("Error: " + message + "!", Colors::Red | Colors::Intens, EntryTypes::Error);
}

FORK_EXPORT void Error(const IO::Error& err)
{
    Message(err.FullMessage(), Colors::Red | Colors::Intens, EntryTypes::Error);
}

FORK_EXPORT void Error(const Lang::SyntaxAnalyzer::ScriptError& err)
{
    if (err.HasLineWithMarker())
    {
        Message(err.FullMessage(), Colors::Black, Colors::Red | Colors::Intens, EntryTypes::Error);
        Message(err.Line(), Colors::Red | Colors::Intens, EntryTypes::Error);
        Message(err.Marker(), Colors::Red | Colors::Intens, EntryTypes::Error);
    }
    else
        Message(err.FullMessage(), Colors::Red | Colors::Intens, EntryTypes::Error);
}

FORK_EXPORT void Error(const DefaultException& err)
{
    if (!err.GetName().empty())
        MessageColored("<1001>Error: <1011>[" + err.GetName() + "]</> " + err.GetMessage() + "</>", EntryTypes::Error);
    else
        Message(err.GetMessage(), EntryTypes::Error);
}

/* --- Misc --- */

FORK_EXPORT void ProgramOptions(
    const std::map<std::string, std::string>& options,
    char separator, size_t minNumSeparators, size_t separatorBorder)
{
    /* Determine longest option string */
    size_t maxLen = 0;

    for (const auto& opt : options)
    {
        auto len = opt.first.size();
        if (maxLen < len)
            maxLen = len;
    }

    maxLen += minNumSeparators;

    /* Pre-construct separator border string */
    const std::string border(separatorBorder, ' ');

    /* Print each option and help */
    for (const auto& opt : options)
    {
        IO::Log::Message(
            opt.first + border +
            std::string(maxLen - opt.first.size(), separator) +
            border + opt.second
        );
    }
}


} // /namespace Log

} // /namespace IO

} // /namespace Fork



// ========================