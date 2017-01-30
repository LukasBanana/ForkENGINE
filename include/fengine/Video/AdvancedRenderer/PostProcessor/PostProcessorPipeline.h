/*
 * Post processor pipeline header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POST_PROCESSOR_PIPELINE_H__
#define __FORK_POST_PROCESSOR_PIPELINE_H__


#include "Video/AdvancedRenderer/PostProcessor/PostProcessor.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(PostProcessorPipeline);

//! Post processor pipeline. This class manages a set of post-processor effects in a pipeline.
class FORK_EXPORT PostProcessorPipeline : public PostProcessor
{
    
    public:
        
        PostProcessorPipeline(const Math::Size2i& initResolution);

        //! Returns "Post Processor Pipeline".
        std::string Name() const override;

        void Draw(RenderTarget* renderTarget) override;

        /**
        Adds the specified post-processor to the pipeline.
        \note A post-processor should never be added to several pipelines!
        Otherwise the a resolution mismatch may occur.
        */
        void AddProcessor(const PostProcessorPtr& processor);
        //! Removes the specified post-processor from the pipeline.
        void RemoveProcessor(const PostProcessor* processor);

    private:
        
        void OnUpdateResolution() override;

        std::vector<PostProcessorPtr> processors_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================