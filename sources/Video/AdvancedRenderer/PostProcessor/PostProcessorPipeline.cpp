/*
 * Post processor pipeline file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PostProcessor/PostProcessorPipeline.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


PostProcessorPipeline::PostProcessorPipeline(const Math::Size2i& initResolution) :
    PostProcessor{ initResolution }
{
}

std::string PostProcessorPipeline::Name() const
{
    return "Post Processor Pipeline";
}

void PostProcessorPipeline::Draw(RenderTarget* renderTarget)
{
    //...
}

void PostProcessorPipeline::AddProcessor(const PostProcessorPtr& processor)
{
    /* Add this post-processor only once to the list */
    if (std::find(processors_.begin(), processors_.end(), processor) == processors_.end())
    {
        processors_.push_back(processor);
        processor->ResizeResolution(GetResolution());
    }
}

void PostProcessorPipeline::RemoveProcessor(const PostProcessor* processor)
{
    RemoveFromListIf(
        processors_,
        [&processor](const PostProcessorPtr& pp)
        {
            return pp.get() == processor;
        }
    );
}


/*
 * ======= Protected: =======
 */

void PostProcessorPipeline::OnUpdateResolution()
{
    for (auto& pp : processors_)
        pp->ResizeResolution(GetResolution());
}


} // /namespace Video

} // /namespace Fork



// ========================