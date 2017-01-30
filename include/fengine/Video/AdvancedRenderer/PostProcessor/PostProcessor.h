/*
 * Post processor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POST_PROCESSOR_H__
#define __FORK_POST_PROCESSOR_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Math/Core/Size2.h"

#include <vector>
#include <string>


namespace Fork
{

namespace Video
{


class RenderTarget;

DECL_SHR_PTR(PostProcessor);

/**
Post processor interface. Post processors can be used to manipulate an image supplementary.
There are different kind of post processing effects such as: Gaussian Blur, God Rays, FXAA, SSAO etc.
*/
class FORK_EXPORT PostProcessor
{
    
    public:
        
        PostProcessor(const Math::Size2i& resolution);
        virtual ~PostProcessor();

        //! Returns the name of the post processing effect.
        virtual std::string Name() const = 0;

        /**
        Resizes the effect resolution.
        \param[in] resolution Specifies the new effect resolution.
        \see GetResolution
        */
        void ResizeResolution(const Math::Size2i& resolution);

        /**
        Draws the post processing effect.
        \param[in] renderTarget Raw pointer to the render target in which the effect is to be rendered.
        \remarks Before the effect can be drawn, it must be initialized with the repsective "Setup" function.
        \note After the effect is drawn, the specified render target remains the active one.
        \throws InvalidStateException If the effect has not been initialized.
        */
        virtual void Draw(RenderTarget* renderTarget) = 0;

        /**
        Returns the current effect resolution. This can only be changed by 'ResizeResolution'.
        \see ResizeResolution
        */
        inline const Math::Size2i& GetResolution() const
        {
            return resolution_;
        }

    protected:
        
        /**
        Callback function when the resolution has changed.
        \see ResizeResolution
        \see GetResolution
        */
        virtual void OnUpdateResolution();

        /**
        Always throws an 'InvalidStateException' exception with the following error description:
        \code
        "Post processing effect \\"" + Name() + "\\" is not initialized (" + errorDesc + ")"
        \endcode
        */
        void NotInitializedException(const std::string& errorDesc = "");

    private:
        
        Math::Size2i resolution_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================