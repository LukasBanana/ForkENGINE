/*
 * Editor error reporter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ERROR_REPORTER_H__
#define __FORK_EDITOR_ERROR_REPORTER_H__


#include <string>
#include <vector>


namespace Fork
{

namespace Editor
{


//! The error reporter keeps track of all errors printed to the terminal and others.
class ErrorReporter
{
    
    public:
        
        //! Registers a new error.
        void RegisterError(const std::string& message);

        /**
        Refreshs the main frame's status bar's field for the number of errors.
        This will be called by "RegisterError" automatically.
        \see RegisterError
        */
        void RefreshMainFrameStatusBar();

        //! Clears all error reports.
        void Clear();

        inline const std::vector<std::string>& GetErrorMessages() const
        {
            return errorMessages_;
        }

    private:
        
        std::vector<std::string> errorMessages_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================