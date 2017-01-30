/*
 * Direct3D 11 shader provider file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ShaderProvider.h"


namespace Fork
{

namespace Video
{


/*
 * Internal members
 */

/* --- HLSL shaders --- */

static const char* shaderSourcesHLSL[] =
{
    /* --- Common drawing shaders --- */

    /* FontDrawing */
    #include "../shaders/Resources/FontDrawingShader.hlsl.h"
    ,

    /* PrimitiveDrawing */
    #include "../shaders/Resources/PrimitiveDrawingShader.hlsl.h"
    ,

    /* BezierDrawing */
    #include "../shaders/Resources/BezierDrawingShader.hlsl.h"
    ,
    
    /* ImageArrayDrawing */
    #include "../shaders/Resources/ImageArrayDrawingShader.hlsl.h"
    ,

    /* UtilCommonModel */
    #include "../shaders/Resources/UtilCommonModelShader.hlsl.h"
    ,

    /* BoundingBox */
    #include "../shaders/Resources/BoundingBoxShader.hlsl.h"
    ,

    /* SimpleColored */
    #include "../shaders/Resources/SimpleColoredShader.hlsl.h"
    ,

    /* SolidColored */
    #include "../shaders/Resources/SolidColoredShader.hlsl.h"
    ,

    /* Simple3DMesh */
    #include "../shaders/Resources/Simple3DMeshShader.hlsl.h"
    ,

    /* TangentSpaceMesh */
    nullptr
    ,

    /* --- Post processing shaders --- */

    /* GaussianBlur */
    #include "../shaders/Resources/GaussianBlurShader.hlsl.h"
    ,

    /* SSCTHorzBlur */
    nullptr
    ,

    /* SSCTVertBlur */
    nullptr
    ,

    /* SSCTHiZMap */
    nullptr
    ,

    /* SSCTVisibilityMap */
    nullptr
    ,

    /* SSCTReflection */
    nullptr
    ,

    /* SSLRRayTrace */
    nullptr
    ,

    /* --- Advanced rendering shaders --- */

    /* Custom MIP maps */
    #include "../shaders/Resources/CustomMIPMapsShader.hlsl.h"
    ,

    /* Hi-Z MIP maps */
    #include "../shaders/Resources/HiZMIPMapsShader.hlsl.h"
    ,
};


/* 
 * D3D11ShaderProvider class
 */

const char* D3D11ShaderProvider::QueryVertexShaderFromResource(const StandardShaders shaderName) const
{
    return shaderSourcesHLSL[ShaderNameToIndex(shaderName)];
}

const char* D3D11ShaderProvider::QueryPixelShaderFromResource(const StandardShaders shaderName) const
{
    return shaderSourcesHLSL[ShaderNameToIndex(shaderName)];
}


} // /namespace Video

} // /namespace Fork



// ========================