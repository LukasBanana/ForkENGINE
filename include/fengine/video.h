/*
 * Video header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIDEO_H__
#define __FORK_VIDEO_H__


/* --- Static config header file --- */

#include "Core/StaticConfig.h"


/* --- Font header files --- */

#include "Video/Font/FontBuildException.h"


/* --- File handler header files --- */

#include "Video/FileHandler/ShaderReader.h"
#include "Video/FileHandler/ImageFileHandler.h"


/* --- Render system header files --- */

#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystemException.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/ScopedActiveRenderContext.h"

#include "Video/RenderSystem/Texture/TextureManager.h"

#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Shader/ShaderManager.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"


/* --- Advanced renderer header files --- */

#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/AdvancedRenderer/ImageArrayContainer.h"
#include "Video/AdvancedRenderer/RichTextRenderer.h"
#include "Video/AdvancedRenderer/CustomMIPMapsGenerator.h"
#include "Video/AdvancedRenderer/GBuffer.h"
#include "Video/AdvancedRenderer/CubeGBuffer.h"
#include "Video/AdvancedRenderer/PackedGBuffer.h"
#include "Video/AdvancedRenderer/PackedCubeGBuffer.h"


/* --- Post processing header files --- */

#include "Video/AdvancedRenderer/PostProcessor/GaussianBlur.h"
#include "Video/AdvancedRenderer/PostProcessor/SSCT.h"
#include "Video/AdvancedRenderer/PostProcessor/SSLR.h"


/* --- Standard shader constant buffers --- */

#include "Video/RenderSystem/Shader/StandardShader/BezierDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/BoundingBoxConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/FontDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/GaussianBlurConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/ImageArrayDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/PrimitiveDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/Simple3DMeshConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/SimpleColoredConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/SolidColoredConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/SSCTConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/SSLRConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/UtilCommonModelConstantBuffers.h"


/* --- Monitor header files --- */

#include "Video/Monitor/VendorIDs.h"


/* --- Buffer format header files --- */

#include "Video/BufferFormat/CommonVertex.h"
#include "Video/BufferFormat/Simple2DVertex.h"
#include "Video/BufferFormat/Simple3DVertex.h"
#include "Video/BufferFormat/TangentSpaceVertex.h"


/* --- Platform header files --- */

#include "Platform/Video/FontBuilder.h"


/* --- Shading model header files --- */

#include "Video/Material/BlinnPhongMaterial.h"
#include "Video/Material/CookTorranceMaterial.h"
#include "Video/Material/ReliefMaterial.h"


#endif



// ========================