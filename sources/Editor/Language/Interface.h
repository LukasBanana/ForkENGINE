/*
 * Editor language library interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_LANGUAGE_LIB_INTERFACE_H__
#define __FORK_EDITOR_LANGUAGE_LIB_INTERFACE_H__


#include "Core/Export.h"

#include <wchar.h>


#ifdef __cplusplus
extern "C"
{
#endif

//! Returns the language name (e.g. L"English" or L"Deutsch").
FORK_EXPORT const wchar_t* LanguageName();

/**
Returns the language specific string for the specified index.
\param[in] index Specifies the index of the language string. This must be a value of the 'LangStrings' enumeration.
\see LangStrings
*/
FORK_EXPORT const wchar_t* LanguageString(unsigned int index);

#ifdef __cplusplus
}
#endif


#endif



// ========================