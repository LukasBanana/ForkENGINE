/*
 * Rasterizer state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RASTERIZER_STATE_H__
#define __FORK_RASTERIZER_STATE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(RasterizerState);

/**
Rasterizer state interface.
This is used to configure the rasterizer of the hardware renderer.
*/
class FORK_EXPORT RasterizerState
{
    
    public:
        
        //! Polygon fill modes.
        enum class FillModes
        {
            Solid,      //!< Polygons are filled solid. This is the default setting.
            Wireframe,  //!< Lines connect the vertices of a polygon.
            Points,     //!< Vertices are represented as points. This is only supported with OpenGL!
        };

        //! Triangle culling modes.
        enum class CullModes
        {
            None,   //!< No culling is performed. Always draw triangles.
            Front,  //!< Front face culling.
            Back,   //!< Back face culling. This is the default setting.
        };

        //! Rasterizer state description structure.
        struct FORK_EXPORT Description
        {
            //! Specifies the triangle fill mode. By default FillModes::Solid.
            FillModes   fillMode                = FillModes::Solid;
            //! Specifies the triangle culling mode. By default CullModes::back (back-face culling).
            CullModes   cullMode                = CullModes::Back;

            int         depthBias               = 0;
            float       depthBiasClamp          = 0.0f;
            float       slopeScaledDepthBias    = 0.0f;

            //! Specifies if a triangle is front facing, when its vertices are counter-clockwise. By default false.
            bool        isFrontCCW              = false;
            /**
            Specifies if depth clipping is enabled. If this is false,
            view frustum culling against the near plane is not correct! By default true.
            */
            bool        isDepthClipEnabled      = true;
            /**
            Enables or disables scissoring.
            \see RenderContext::SetupScissor
            */
            bool        isScissorTestEnabled    = false;
            bool        isMultiSampleEnable     = false;
            bool        isAntiAliasedLineEnable = false;
        };

        virtual ~RasterizerState()
        {
        }

        RasterizerState(const RasterizerState&) = delete;
        RasterizerState& operator = (const RasterizerState&) = delete;

        //! Returns the rasterizer state description.
        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        RasterizerState(const Description& desc) :
            desc_{ desc }
        {
        }

        Description desc_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================