/*
 * Direct3D 11 core function file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11Core.h"
#include "Core/StringModifier.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Video
{


std::string DXErrorToStr(HRESULT errorCode)
{
    if (errorCode != S_OK)
    {
        switch (errorCode)
        {
            case D3D11_ERROR_FILE_NOT_FOUND:
                return "File Not Found";
            case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
                return "Too Many Unique State Objects";
            case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
                return "Too Many Unique View Objects";
            case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
                return "Deferred Context Map Without Initial Discard";
            //case D3DERR_INVALIDCALL:
            //case D3DERR_WASSTILLDRAWING:
            case E_FAIL:
                return "Fail";
            case E_INVALIDARG:
                return "Invalid Argument";
            case E_OUTOFMEMORY:
                return "Out Of Memory";
            case S_FALSE:
                return "False";
            default:
                return "Unknown (Code = " + ToStr(errorCode) + ")";
        }
    }
    return "";
}

std::string DXErrorToStr(HRESULT errorCode, const std::string& errorMessage)
{
    return errorMessage + " (Error Code: " + DXErrorToStr(errorCode) + ")";
}

static void DXError(HRESULT result, const std::string& errorMessage)
{
    IO::Log::Error(DXErrorToStr(result, errorMessage));
}

bool DXAssert(HRESULT result, const std::string& errorMessage, bool throwResult)
{
    if (result != S_OK)
    {
        DXError(result, errorMessage);
        if (throwResult)
            throw result;
        return false;
    }
    return true;
}

void DXAssert(HRESULT result, const std::string& errorMessage)
{
    if (result != S_OK)
    {
        DXError(result, errorMessage);
        throw result;
    }
}


} // /namespace Video

} // /namespace Fork



// ========================