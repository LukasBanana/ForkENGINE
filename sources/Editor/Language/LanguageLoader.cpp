/*
 * Editor language loader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "LanguageLoader.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"
#include "Platform/Core/OS.h"


namespace Fork
{

namespace Editor
{

namespace Language
{


/* --- Internal members --- */

DEFINE_PROC_INTERFACE(const wchar_t*, PFNLANGUAGESTRINGPROC, (unsigned int));
static PFNLANGUAGESTRINGPROC LanguageString = nullptr;

DEFINE_PROC_INTERFACE(const wchar_t*, PFNLANGUAGENAMEPROC, (void));
static PFNLANGUAGENAMEPROC LanguageName = nullptr;


/* --- Global functions --- */

Platform::DynamicLibraryPtr LoadLanguageModule(const std::wstring& libraryName)
{
    try
    {
        /* Load language module, name- and string functions */
        auto library = Platform::DynamicLibrary::Open(libraryName);

        LanguageName = reinterpret_cast<PFNLANGUAGENAMEPROC>(library->LoadProc("LanguageName"));

        if (!LanguageName)
        {
            IO::Log::Error("Loading 'LanguageName' function from language module \"" + UTF16toUTF8(libraryName) + "\" failed");
            return nullptr;
        }

        LanguageString = reinterpret_cast<PFNLANGUAGESTRINGPROC>(library->LoadProc("LanguageString"));

        if (!LanguageString)
        {
            IO::Log::Error("Loading 'LanguageString' function from language module \"" + UTF16toUTF8(libraryName) + "\" failed");
            return nullptr;
        }

        return library;
    }
    catch (const DynamicLibraryOpenException&)
    {
        IO::Log::Error("Loading language module \"" + UTF16toUTF8(libraryName) + "\" failed");
    }

    return nullptr;
}

wxString Name()
{
    return LanguageName != nullptr ? wxString(LanguageName()) : "ERR";
}

wxString String(unsigned int index)
{
    if (LanguageString && index < LangStrings::__Num__)
    {
        auto str = LanguageString(index);
        if (str)
            return wxString(str);
    }
    return "ERR";
}


} // /namespace Language

} // /namespace Editor

} // /namespace Fork



// ========================