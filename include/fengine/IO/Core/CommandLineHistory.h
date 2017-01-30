/*
 * Command line history header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_COMMAND_LINE_HISTORY_H__
#define __FORK_IO_COMMAND_LINE_HISTORY_H__


#include "Core/Export.h"

#include <string>
#include <vector>


namespace Fork
{

namespace IO
{


/**
General purpose command line history class. This allows the user to perform a
command line several times, without that the user must type it again.
*/
class FORK_EXPORT CommandLineHistory
{
    
    public:
        
        /**
        Adds the specified command line to the histroy.
        \remarks A command will only be added if it does not equals the previous command line in the list.
        \remarks This will also set the list iterator to the end.
        \remarks Empty command lines or command lines which consist only of white spaces will not be added.
        \see GetCommandLineList
        */
        void AddCommandLine(const std::string& commandLine);

        /**
        Returns the current command line where the iterator points to.
        This will be an empty string if 'Prev' was not called since the last command line was added.
        \see AddCommandLine
        */
        std::string CurrentCommandLine();

        /*
        Decrements the list iterator of the command line list.
        Call this to get the previous command line in the history.
        \return True if a new command line has been accessed from the history.
        Otherwise the beginning of the list has been reached.
        */
        bool Prev();
        /*
        Increments the list iterator of the command line list.
        Call this to get the next command line in the history.
        \return True if a new command line has been accessed from the history.
        Otherwise the end of the list has been reached.
        */
        bool Next();

        //! Clears the command line list.
        void Clear();

        //! Restes the list iterator to the end of the list.
        void ResetIterator();
        
        //! Returns the list of all command lines in the history.
        inline const std::vector<std::string>& GetCommandLineList() const
        {
            return commandLineList_;
        }

        //! Returns true if the command line list is empty.
        inline bool IsEmpty() const
        {
            return commandLineList_.empty();
        }

        //! Specifies the history limit. If this is 0, then there is no limit. By default 100.
        size_t historyLimit = 100;

    private:
        
        std::vector<std::string> commandLineList_;
        std::vector<std::string>::iterator commandLineIt_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================