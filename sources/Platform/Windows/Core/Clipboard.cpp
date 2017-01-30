/*
 * WIN32: Clipboard file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/Clipboard.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{

namespace Clipboard
{


FORK_EXPORT std::wstring GetClipboardText()
{
    if (!OpenClipboard(0))
        return L"";

    /* Get clipboard text data */
    HANDLE clip = GetClipboardData(CF_UNICODETEXT);

    /* Copy raw-string into wstring object */
    wchar_t* rawString = reinterpret_cast<wchar_t*>(GlobalLock(clip));
    std::wstring wstr(rawString);

    /* Release clipboard object */
    GlobalUnlock(clip);
    CloseClipboard();

    return wstr;
}

FORK_EXPORT void SetClipboardText(const std::wstring& wstr)
{
    if (!OpenClipboard(0))
        return;

    /* Allocate clipboard object */
    const size_t len = wstr.size();
    HANDLE clip = GlobalAlloc(GMEM_MOVEABLE, (len + 1)*sizeof(wchar_t));

    /* Copy wstring object content into raw-string */
    wchar_t* rawString = reinterpret_cast<wchar_t*>(GlobalLock(clip));

    memcpy(rawString, wstr.c_str(), len*sizeof(wchar_t));
    rawString[len] = wchar_t(0);

    /* Set clipboard text data */
    SetClipboardData(CF_UNICODETEXT, clip);

    /* Release clipboard object */
    GlobalUnlock(clip);
    CloseClipboard();
}


} // /namespace Clipboard

} // /namespace Platform

} // /namespace Fork



// ========================