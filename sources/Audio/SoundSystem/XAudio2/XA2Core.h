/*
 * XAudio2 core function header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_CORE_H__
#define __FORK_XA2_CORE_H__


namespace Fork
{

namespace Audio
{


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


} // /namespace Audio

} // /namespace Fork


#endif



// ========================