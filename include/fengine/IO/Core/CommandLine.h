/*
 * Command line header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_COMMAND_LINE_H__
#define __FORK_IO_COMMAND_LINE_H__


#include "Core/Export.h"

#include <string>
#include <vector>


namespace Fork
{

namespace IO
{


/**
General purpose command line class.
This class parses and splits a command line into its arguments.
*/
class FORK_EXPORT CommandLine
{
    
    public:
        
        //! Command argument list type (array list of strings).
        typedef std::vector<std::string> ArgumentListType;

        /**
        The command constructor parses the command line and stores all arguments.
        \param[in] commandLine Specifies the command line which will be parsed to access all arguments via 'GetArguments'.
        \remarks The following command constructions will result in the respective argument lists:
        \code
        CommandLine("test") -> { "test" }
        CommandLine("test -foo bar") -> { "test", "-foo", "bar" }
        CommandLine("command > foo_bar") -> { "command", ">", "foo_bar" }
        CommandLine("foo \"and bar\"") -> { "foo", "and bar" }
        CommandLine("    ") -> { }
        \endcode
        \note String literals are not treated as in ANSI C, i.e. there are no escape characters!
        \see GetArguments
        */
        CommandLine(const std::string& commandLine);
        CommandLine(const char* commandLine);
        CommandLine(const CommandLine&& other);

        //! Returns the command name or an empty string if the argument list is empty.
        std::string Name() const;

        //! Cast operator to std::string. This will concatenate all arguments and separate them with a space character.
        operator std::string () const;

        /**
        Returns an argument list, converted from the specified C arguments.
        \param[in] argc Specifies the number of arguments ('ARGument Count').
        \param[in] argv Specifies the argument strings ('ARGument Values').
        \remarks This can be used to easily iterate over the C main function's arguments:
        \code
        int main(int argc, char* argv[])
        {
            for (const auto& arg : IO::CommandLine::ToList(argc, argv))
            {
                //...
            }
            return 0;
        }
        \endcode
        */
        static ArgumentListType ToList(int argc, char* argv[]);

        //! Returns the argument list (inclusive command name).
        inline const ArgumentListType& GetArguments() const
        {
            return arguments_;
        }

        //! Returns true if the argument list is empty.
        inline bool IsEmpty() const
        {
            return arguments_.empty();
        }

    private:
        
        ArgumentListType arguments_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================