/*
 * Direct3D 11 feature support header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_FEATURE_SUPPORT_H__
#define __FORK_D3D11_FEATURE_SUPPORT_H__


#include "Video/Monitor/RendererSupport.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


/*
Here are the full Direct3D renderer support lists
for the Direct3D feature support levels.
*/

extern const RendererSupport dxFeatureSupport_9_1;
extern const RendererSupport dxFeatureSupport_9_2;
extern const RendererSupport dxFeatureSupport_9_3;
extern const RendererSupport dxFeatureSupport_10_0;
extern const RendererSupport dxFeatureSupport_10_1;
extern const RendererSupport dxFeatureSupport_11_0;
extern const RendererSupport dxFeatureSupport_11_1;


/**
Returns the renderer support for the specified D3D feature level.
\throws InvalidArgumentException If 'featureLevel' is invalid.
*/
const RendererSupport& DXFeatureSupport(D3D_FEATURE_LEVEL featureLevel);

//! Returns the shader model name by the specified feature level.
const char* DXFeatureLevelToShaderModel(D3D_FEATURE_LEVEL featureLevel);


} // /namespace Video

} // /namespace Fork


#endif



// ========================