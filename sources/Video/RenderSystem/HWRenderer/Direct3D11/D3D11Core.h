/*
 * Direct3D 11 core function header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_CORE_H__
#define __FORK_D3D11_CORE_H__


#include <string>
#include <d3d11.h>


namespace Fork
{

namespace Video
{


#define CAST_TO_D3D11_OBJECT(t, o) CastToConcreteObject<D3D11##t>(o, __FUNCTION__, #t, "D3D11" #t)

//! Converts the DX error code into a string.
std::string DXErrorToStr(HRESULT errorCode);
//! Converts the DX error code into a string with a prefixed error message.
std::string DXErrorToStr(HRESULT errorCode, const std::string& errorMessage);

/**
Asserts that 'result' is S_OK. Otherwise the specified error message is printed to the log output.
\param[in] result Specifies the result of a Direct3D operation.
\param[in] errorMessage Specifies the erorr message which is printed on failure.
\param[in] throwResult Specifies whether an exception is thrown in the case of a failure or not. By default true.
\throws HRESULT This is the same as 'result' is 'throwResult' is true.
\return True if 'result' is S_OK and 'throwResult' is false.
*/
bool DXAssert(HRESULT result, const std::string& errorMessage, bool throwResult);
//! This is equivalent to 'DXAssert(result, errorMessage, true)'.
void DXAssert(HRESULT result, const std::string& errorMessage);

//! Releases the specified DirectX object.
template <class T> void DXSafeRelease(T*& object)
{
    if (object)
    {
        object->Release();
        object = nullptr;
    }
}

//! Releases the specified list of DirectX objects.
template <class T> void DXSafeReleaseList(T& list)
{
    for (auto& object : list)
        DXSafeRelease(object);
    list.clear();
}

//! Converts the specified boolean into a D3D-boolean value.
inline BOOL DXBool(bool boolean)
{
    return boolean ? TRUE : FALSE;
}


} // /namespace Video

} // /namespace Fork


#endif



// ========================