/*
 * Post processor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PostProcessor/PostProcessor.h"
#include "Core/Exception/InvalidStateException.h"


namespace Fork
{

namespace Video
{


PostProcessor::PostProcessor(const Math::Size2i& resolution) :
    resolution_{ resolution }
{
}
PostProcessor::~PostProcessor()
{
}

void PostProcessor::ResizeResolution(const Math::Size2i& resolution)
{
    if (resolution_ != resolution)
    {
        resolution_ = resolution;
        OnUpdateResolution();
    }
}


/*
 * ======= Protected: =======
 */

void PostProcessor::OnUpdateResolution()
{
    // dummy
}

void PostProcessor::NotInitializedException(const std::string& errorDesc)
{
    std::string msg = "Post processing effect \"" + Name() + "\" is not initialized";

    if (!errorDesc.empty())
        msg += " (" + errorDesc + ")";

    throw InvalidStateException(__FUNCTION__, msg);
}


} // /namespace Video

} // /namespace Fork



// ========================