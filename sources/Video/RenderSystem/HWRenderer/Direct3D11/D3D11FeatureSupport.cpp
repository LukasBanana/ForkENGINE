/*
 * Direct3D 11 feature support file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11FeatureSupport.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Video
{


/*
The information for this pre-defined feature support has been accessed from:
http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876%28v=vs.85%29.aspx
*/

//                                              RTs?  Geom?  Tess?  Comp?  Array? CbAry? Inst?  VPA?  NumAr RT  CBuff      Num Work Groups       Thread Group Size   1D Tex 2D Tex 3D Tex  Cube  Ani
const RendererSupport dxFeatureSupport_9_1  = { true, false, false, false, false, false, false, false,    1, 1,     0, {     0,     0,     0 }, {    0,    0,    0 },  2048,  2048,  256,   512,  2 };
const RendererSupport dxFeatureSupport_9_2  = { true, false, false, false, false, false, false, false,    1, 1,     0, {     0,     0,     0 }, {    0,    0,    0 },  2048,  2048,  256,   512, 16 };
const RendererSupport dxFeatureSupport_9_3  = { true, false, false, false, false, false,  true, false,    1, 4,     0, {     0,     0,     0 }, {    0,    0,    0 },  4096,  4096,  256,  4096, 16 };
const RendererSupport dxFeatureSupport_10_0 = { true,  true, false, false,  true, false,  true,  true, 2048, 8, 65536, { 65536, 65536,     1 }, {  768,  768,    1 },  8192,  8192,  256,  8192, 16 };
const RendererSupport dxFeatureSupport_10_1 = { true,  true, false, false,  true,  true,  true,  true, 2048, 8, 65536, { 65536, 65536,     1 }, {  768,  768,    1 },  8192,  8192, 2048,  8192, 16 };
const RendererSupport dxFeatureSupport_11_0 = { true,  true,  true,  true,  true,  true,  true,  true, 2048, 8, 65536, { 65536, 65536, 65536 }, { 1024, 1024, 1024 }, 16384, 16384, 2048, 16384, 16 };
const RendererSupport dxFeatureSupport_11_1 = { true,  true,  true,  true,  true,  true,  true,  true, 2048, 8, 65536, { 65536, 65536, 65536 }, { 1024, 1024, 1024 }, 16384, 16384, 2048, 16384, 16 };


const RendererSupport& DXFeatureSupport(D3D_FEATURE_LEVEL featureLevel)
{
    switch (featureLevel)
    {
        case D3D_FEATURE_LEVEL_9_1:
            return dxFeatureSupport_9_1;
        case D3D_FEATURE_LEVEL_9_2:
            return dxFeatureSupport_9_2;
        case D3D_FEATURE_LEVEL_9_3:
            return dxFeatureSupport_9_3;
        case D3D_FEATURE_LEVEL_10_0:
            return dxFeatureSupport_10_0;
        case D3D_FEATURE_LEVEL_10_1:
            return dxFeatureSupport_10_1;
        case D3D_FEATURE_LEVEL_11_0:
            return dxFeatureSupport_11_0;
        case D3D_FEATURE_LEVEL_11_1:
            return dxFeatureSupport_11_1;
    }
    throw InvalidArgumentException(__FUNCTION__, "featureLevel", "Invalid feature level selected");
}

const char* DXFeatureLevelToShaderModel(D3D_FEATURE_LEVEL featureLevel)
{
    switch (featureLevel)
    {
        case D3D_FEATURE_LEVEL_9_1:
            return "Shader Model 2.0";
        case D3D_FEATURE_LEVEL_9_2:
            return "Shader Model 2.0";
        case D3D_FEATURE_LEVEL_9_3:
            return "Shader Model 3.0";
        case D3D_FEATURE_LEVEL_10_0:
            return "Shader Model 4.0";
        case D3D_FEATURE_LEVEL_10_1:
            return "Shader Model 4.1";
        case D3D_FEATURE_LEVEL_11_0:
            return "Shader Model 5.0";
        case D3D_FEATURE_LEVEL_11_1:
            return "Shader Model 5.1";
    }
    return "";
}


} // /namespace Video

} // /namespace Fork



// ========================