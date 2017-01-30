/*
 * Clipboard header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_CLIPBOARD_H__
#define __FORK_PLATFORM_CLIPBOARD_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Platform
{

namespace Clipboard
{


//! Returns the current clipboard text.
FORK_EXPORT std::wstring GetClipboardText();

//! Sets the new clipboard text.
FORK_EXPORT void SetClipboardText(const std::wstring& wstr);


} // /namespace Clipboard

} // /namespace Platform

} // /namespace Fork


#endif



// ========================