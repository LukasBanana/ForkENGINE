/*
 * WIN32: Win32 frame class header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_WIN32FRAME_CLASS_H__
#define __FORK_PLATFORM_WIN32FRAME_CLASS_H__


namespace Fork
{

namespace Platform
{


class Win32FrameClass
{
    
    public:
        
        ~Win32FrameClass();

        static Win32FrameClass* Instance();

        const wchar_t* GetName() const;

    private:

        Win32FrameClass();
        Win32FrameClass(const Win32FrameClass&);
        //Win32FrameClass(const Win32FrameClass&) = delete;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================