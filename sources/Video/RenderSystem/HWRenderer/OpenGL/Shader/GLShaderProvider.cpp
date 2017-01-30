/*
 * OpenGL shader provider file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLShaderProvider.h"


namespace Fork
{

namespace Video
{


/*
 * Internal members
 */

struct StdShaderSources
{
    const char* vertexShaderSources;
    const char* pixelShaderSources;
};

/* --- GLSL shaders --- */

static const char* vertexShaderSourcePostProcessingGLSL
{
    #include "../shaders/Resources/PostProcessingVertexShader.glsl.h"
};

static const char* vertexShaderSourceCustomMIPMapsGLSL
{
    #include "../shaders/Resources/CustomMIPMapsVertexShader.glsl.h"
};

static const StdShaderSources shaderSourcesGLSL[] =
{
    /* --- Common drawing shaders --- */

    /* FontDrawing */
    {
        #include "../shaders/Resources/FontDrawingVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/FontDrawingPixelShader.glsl.h"
    },

    /* PrimitiveDrawing */
    {
        #include "../shaders/Resources/PrimitiveDrawingVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/PrimitiveDrawingPixelShader.glsl.h"
    },

    /* BezierDrawing */
    {
        #include "../shaders/Resources/BezierDrawingVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/BezierDrawingPixelShader.glsl.h"
    },
    
    /* ImageArrayDrawing */
    {
        #include "../shaders/Resources/ImageArrayDrawingVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/ImageArrayDrawingPixelShader.glsl.h"
    },

    /* UtilCommonModel */
    {
        #include "../shaders/Resources/UtilCommonModelVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/UtilCommonModelPixelShader.glsl.h"
    },

    /* BoundingBox */
    {
        #include "../shaders/Resources/BoundingBoxVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/BoundingBoxPixelShader.glsl.h"
    },

    /* SimpleColored */
    {
        #include "../shaders/Resources/SimpleColoredVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/SimpleColoredPixelShader.glsl.h"
    },

    /* SolidColored */
    {
        #include "../shaders/Resources/SolidColoredVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/SolidColoredPixelShader.glsl.h"
    },

    /* Simple3DMesh */
    {
        #include "../shaders/Resources/Simple3DMeshVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/Simple3DMeshPixelShader.glsl.h"
    },

    /* TangentSpaceMesh */
    {
        #include "../shaders/Resources/TangentSpaceMeshVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/TangentSpaceMeshPixelShader.glsl.h"
    },

    /* --- Post processing shaders --- */

    /* GaussianBlur */
    {
        vertexShaderSourcePostProcessingGLSL
        ,
        #include "../shaders/Resources/GaussianBlurPixelShader.glsl.h"
    },

    /* SSCTHorzBlur */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/SSCTHorzBlurPixelShader.glsl.h"
    },

    /* SSCTVertBlur */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/SSCTVertBlurPixelShader.glsl.h"
    },

    /* SSCTHiZMap */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/SSCTHiZMapPixelShader.glsl.h"
    },

    /* SSCTVisibilityMap */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/SSCTVisibilityMapPixelShader.glsl.h"
    },

    /* SSCTReflection */
    {
        #include "../shaders/Resources/SSCTReflectionVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/SSCTReflectionPixelShader.glsl.h"
    },

    /* SSLRRayTrace */
    {
        #include "../shaders/Resources/SSLRRayTraceVertexShader.glsl.h"
        ,
        #include "../shaders/Resources/SSLRRayTracePixelShader.glsl.h"
    },

    /* --- Advanced rendering shaders --- */

    /* Custom MIP maps */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/CustomMIPMapsPixelShader.glsl.h"
    },

    /* Hi-Z MIP maps */
    {
        vertexShaderSourceCustomMIPMapsGLSL
        ,
        #include "../shaders/Resources/HiZMIPMapsPixelShader.glsl.h"
    },
};


/* 
 * GLShaderProvider class
 */

const char* GLShaderProvider::QueryVertexShaderFromResource(const StandardShaders shaderName) const
{
    return shaderSourcesGLSL[ShaderNameToIndex(shaderName)].vertexShaderSources;
}

const char* GLShaderProvider::QueryPixelShaderFromResource(const StandardShaders shaderName) const
{
    return shaderSourcesGLSL[ShaderNameToIndex(shaderName)].pixelShaderSources;
}


} // /namespace Video

} // /namespace Fork



// ========================