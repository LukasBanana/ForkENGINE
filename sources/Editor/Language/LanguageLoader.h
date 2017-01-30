/*
 * Editor language loader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_LANGUAGE_LOADER_H__
#define __FORK_EDITOR_LANGUAGE_LOADER_H__


#include "Core/Export.h"
#include "Platform/Core/DynamicLibrary.h"
#include "LanguageStrings.h"

#include <wx/string.h>
#include <string>


namespace Fork
{

namespace Editor
{

namespace Language
{


/**
Returns the specified language string.
\see Language::String
*/
#define LANG_STRING(n) Language::String(LangStrings::n)

/**
Loads the specified editor language module (e.g. L"ForkEdLanguageEN").
If the specified module could not be loaded, the return value is null.
*/
Platform::DynamicLibraryPtr LoadLanguageModule(const std::wstring& libraryName);

//! Returns the current language name.
wxString Name();

/**
Returns the specified language string.
\param[in] index Specifies the string index. This should be a value from "LangStrings".
\return Language specific string. If the string could not be found, the string "ERR" (as "Error") will be returned.
\see LangStrings
*/
wxString String(unsigned int index);


} // /namespace Language

} // /namespace Editor

} // /namespace Fork


#endif



// ========================