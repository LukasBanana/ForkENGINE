/*
 * Log header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_LOG_H__
#define __FORK_IO_LOG_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "IO/Core/Error.h"
#include "Platform/Core/ConsoleManip.h"
#include "Lang/SyntaxAnalyzer/ScriptError.h"

#include <functional>
#include <memory>
#include <map>


namespace Fork
{

namespace IO
{

/**
The log is a pure event handling system, i.e. no messages will be printed to the console,
file or whatever until an event handler has been added to the log system.
The default console log can be easily added by calling "IO::Log::AddDefaultEventHandler()".
All event handlers will get a callback whenever a message is printed with the log.
The log also manages indentation for better output visibility.
\see AddDefaultEventHandler
\see ScopedIndent
*/
namespace Log
{


/**
Debug message infinite (to ignore the limit).
\see Debug
*/
static const int debugMessageInfinite = -1;

/**
Default debug message limit (3).
\see Debug
*/
static const int debugMessageDefaultLimit = 3;

typedef Platform::ConsoleManip::Colors::DataType ColorFlags;


//! Log entry type enumeration.
enum class EntryTypes
{
    Meta,       //!< No information log entry, just unspecified meta text.
    Info,       //!< Information log entry.
    Warning,    //!< Warning log entry.
    Error,      //!< Error log entry.
    Debug       //!< Debug log entry.
};


DECL_SHR_PTR(EventHandler);

/**
Log event handler interface.
The default event handler is 'Console::LogEventHandler'.
\see AddEventHandler
\see Console::LogEventHandler
*/
class FORK_EXPORT EventHandler
{
    
    public:
        
        //! Type alias for the color flags.
        typedef IO::Log::ColorFlags ColorFlags;

        virtual ~EventHandler()
        {
        }

        //! Prints a new message to the current log line.
        virtual void OnPrint(const std::string& message, const EntryTypes type)
        {
        }
        /**
        Prints a new line with the specified indentation 'indent' and message.
        By default 'OnStartLn(indent)', 'OnPrint(message)' and 'OnEndLn()' is called here.
        \see OnStartLn
        \see OnPrint
        \see OnEndLn
        */
        virtual void OnPrintLn(const std::string& indent, const std::string& message, const EntryTypes type)
        {
            OnStartLn(indent);
            OnPrint(message, type);
            OnEndLn();
        }

        //! Starts a new log line with the specified indentation 'indent'.
        virtual void OnStartLn(const std::string& indent)
        {
        }
        //! Ends a log line. Commonly the new-line character is printed here.
        virtual void OnEndLn()
        {
        }

        //! Pushes a new front color flag.
        virtual void OnPushColor(const ColorFlags& frontColorFlags)
        {
        }
        //! Pushes a new front- and back color flag.
        virtual void OnPushColor(const ColorFlags& frontColorFlags, const ColorFlags& backColorFlags)
        {
        }
        //! Pops the previous color flag.
        virtual void OnPopColor()
        {
        }

        /**
        Prints a blank line. By default 'OnStartLn("")' and 'OnEndLn()' is called here.
        \see OnStartLn
        \see OnEndLn
        */
        virtual void OnBlank()
        {
            OnStartLn("");
            OnEndLn();
        }

    protected:
        
        /**
        Comfort function to convert the specified color flags into a real RGB color.
        This is just a shortcut for the 'Platform::ConsoleManip::ToColor' function.
        \see Platform::ConsoleManip::ToColor
        */
        inline Video::ColorRGBf ToColor(const ColorFlags& colorFlags) const
        {
            return Platform::ConsoleManip::ToColor(colorFlags);
        }

};


/* --- Event handling --- */

/**
Adds an event handler.
\see EventHandler
*/
FORK_EXPORT void AddEventHandler(const EventHandlerPtr& eventHandler);
//! Removes the specified event handler.
FORK_EXPORT void RemoveEventHandler(const EventHandlerPtr& eventHandler);
//! Removes all event handlers.
FORK_EXPORT void ClearEventHandlers();

/**
Adds the default console event handler.
\return Shared pointer to the new added event handler.
\see Console::LogEventHandler
*/
FORK_EXPORT EventHandlerPtr AddDefaultEventHandler();

/* --- Indentation --- */

//! Sets the indentation string. By default "  " (two spaces).
FORK_EXPORT void SetIndent(const std::string& indent);
//! Returns the indentation string.
FORK_EXPORT const std::string& GetIndent();

/**
Returns the current full indentation. This can be increased or decreased
by the functions 'IncIndent' and 'DecIndent' or the 'ScopedIndent' structure.
The indentation string can be set with 'SetIndent'.
\see SetIndent
\see IncIndent
\see DecIndent
\see ScopedIndent
*/
FORK_EXPORT const std::string& GetFullIndent();

//! Increases indentation.
FORK_EXPORT void IncIndent();
//! Decreases indentation.
FORK_EXPORT void DecIndent();

/* --- Messages --- */

/**
Prints a message. This will call all event handlers. The callbacks 'OnPrintLn' will be called.
\see EventHandler::OnPrintLn
*/
FORK_EXPORT void Message(
    const std::string& message, const EntryTypes type = EntryTypes::Info
);
/**
Prints a message with the specified color flags.
\see EventHandler::OnPushColor
\see ColorFlags
*/
FORK_EXPORT void Message(
    const std::string& message, const ColorFlags colorFlags, const EntryTypes type = EntryTypes::Info
);
/**
Prints a message with the specified color flags.
\see EventHandler::OnPushColor
\see ColorFlags
*/
FORK_EXPORT void Message(
    const std::string& message, const ColorFlags colorFlagsFront, const ColorFlags colorFlagsBack, const EntryTypes type = EntryTypes::Info
);

/**
Prints the specified message with individual colors. The colors can be specified inside the message:
\code
// Example:
MessageColor("Hello, <0101>World</>!");
MessageColor("<0000|0010><1001>5</> &lt; <1101>7</> &amp; 3 &gt; 2</>");
\endcode
The attributes inside the angle brackets ('<' and '>') are four binary values.
They describe which flag is to be set: Red, Green, Blue, Intens
\see Platform::ConsoleManip::Colors
*/
FORK_EXPORT void MessageColored(const std::string& message, const EntryTypes type = EntryTypes::Info);

//! Prints the message (EntryTypes::Info) in green.
FORK_EXPORT void Success(const std::string& message);

//! Prints "Warning: " + message + "!" in yellow (EntryTypes::Warning).
FORK_EXPORT void Warning(const std::string& message);

//! Prints a single blank line.
FORK_EXPORT void Blank();

/* --- Debugging --- */

/**
Prints the message (EntryTypes::Debug) in pink.
\param[in] message Specifies the debug message.
\param[in] messageLimit Specifies the message limit. This should be used,
when a debug message may occure every frame (e.g. when a drawing operation failed).
To ignore the limit, set this to 'debugMessageInfinite' - which is the default value.
*/
FORK_EXPORT void Debug(
    const std::string& message, int messageLimit = debugMessageInfinite
);
//! \see Debug(const std::string&, int)
FORK_EXPORT void Debug(
    const std::string& operationDesc, const std::string& message,
    int messageLimit = debugMessageInfinite
);
//! \see Debug(const std::string&, int)
FORK_EXPORT void Debug(
    const std::string& errorPrefix, const std::string& operationDesc,
    const std::string& message, int messageLimit = debugMessageInfinite
);

/* --- Errors --- */

//! Prints "Error: " + message + "!" in red (EntryTypes::Error).
FORK_EXPORT void Error(const std::string& message);

//! Prints the specified error message (EntryTypes::Error).
FORK_EXPORT void Error(const IO::Error& err);

//! Prints the specified script error message (EntryTypes::Error).
FORK_EXPORT void Error(const Lang::SyntaxAnalyzer::ScriptError& err);

//! Prints the specified exception error message (exception identifier is colored in pink).
FORK_EXPORT void Error(const DefaultException& err);

/* --- Misc --- */

/**
Prints program options.
\param[in] options Specifies the options map. The map-key is the program
option (or rather parameter) and the map-value is the description.
\param[in] separator Specifies the option/description separator. By default '.'.
\param[in] minNumSeparators Specifies the minimal number of separators. By default 3.
\param[in] separatorBorder Specifies the number of blanks (char(' ')) before and after the spearator string. By default 1.
\remarks Here is a small output example:
\code
// Code:
ProgramOptions(
    { { "help", "Prints the help" }, { "version", "Prints the version" } },
    '.', 3, 1
);
// Output:
// "help ...... Prints the help"
// "version ... Prints the version"
\endcode
*/
FORK_EXPORT void ProgramOptions(
    const std::map<std::string, std::string>& options,
    char separator = '.', size_t minNumSeparators = 3, size_t separatorBorder = 1
);


/**
Scoped indentation structure.
\code
// Print a tree hierarhy
IO::Log::Message("Tree Root");
{
    // Only the constructor and destructor of the scoped indentation structure will be used
    IO::Log::ScopedIndent indent;
    // Print some children nodes
    IO::Log::Message("Child A");
    IO::Log::Message("Child B");
}
// Print another tree hierarchy in the previous indentation.
IO::Log::Message("Another Tree Root");
\endcode
*/
struct ScopedIndent
{
    ScopedIndent(const ScopedIndent&) = delete;
    ScopedIndent& operator = (const ScopedIndent&) = delete;

    inline ScopedIndent()
    {
        IncIndent();
    }
    inline ~ScopedIndent()
    {
        DecIndent();
    }
};


} // /namespace Log

} // /namespace IO

} // /namespace Fork


#endif



// ========================