/*
 * XAudio2 voice callback file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2VoiceCallback.h"


namespace Fork
{

namespace Audio
{


XA2VoiceCallback::XA2VoiceCallback(PCMBufferQueue& bufferQueue) :
    bufferQueueRef_{ &bufferQueue }
{
}

void XA2VoiceCallback::OnStreamEnd()
{
    // dummy
}

void XA2VoiceCallback::OnVoiceProcessingPassEnd()
{
    // dummy
}

void XA2VoiceCallback::OnVoiceProcessingPassStart(UINT32 samplesRequired)
{
    // dummy
}

void XA2VoiceCallback::OnBufferEnd(void* bufferContext)
{
    bufferQueueRef_->pop();
}

void XA2VoiceCallback::OnBufferStart(void* bufferContext)
{
    // dummy
}

void XA2VoiceCallback::OnLoopEnd(void* bufferContext)
{
    // dummy
}

void XA2VoiceCallback::OnVoiceError(void* bufferContext, HRESULT error)
{
    // dummy
}


} // /namespace Audio

} // /namespace Fork



// ========================