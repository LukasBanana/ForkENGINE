/*
 * Error types header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_ERROR_TYPES_H__
#define __FORK_IO_ERROR_TYPES_H__


namespace Fork
{

namespace IO
{


//! Error message types.
enum class ErrorTypes
{
    /* --- Common error types --- */
    Default,        //!< Default error message.
    Fatal,          //!< Fata error. Forces the program to quit.
    FileNotFound,   //!< File could not found error.

    /* --- Script errors --- */
    Syntax,         //!< Syntax error in a script file (e.g. for "ForkSCript").
    Context,        //!< Context error in a script file (e.g. for "ForkSCript").
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================