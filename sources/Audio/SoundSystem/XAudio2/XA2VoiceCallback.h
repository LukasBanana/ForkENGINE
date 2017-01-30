/*
 * XAudio2 voice callback header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_VOICE_CALLBACK_H__
#define __FORK_XA2_VOICE_CALLBACK_H__


#include "Core/StdCall.h"
#include "Audio/Core/WaveBuffer.h"

#include <xaudio2.h>


namespace Fork
{

namespace Audio
{


//! XAudio2 voice callback implementation.
class XA2VoiceCallback : public IXAudio2VoiceCallback
{
    
    public:
        
        XA2VoiceCallback(PCMBufferQueue& bufferQueue);

        void FORK_STDCALL OnStreamEnd();

        void FORK_STDCALL OnVoiceProcessingPassEnd();
        void FORK_STDCALL OnVoiceProcessingPassStart(UINT32 samplesRequired);

        void FORK_STDCALL OnBufferEnd(void* bufferContext);
        void FORK_STDCALL OnBufferStart(void* bufferContext);

        void FORK_STDCALL OnLoopEnd(void* bufferContext);
        void FORK_STDCALL OnVoiceError(void* bufferContext, HRESULT error);

    private:

        PCMBufferQueue* bufferQueueRef_ = nullptr;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================