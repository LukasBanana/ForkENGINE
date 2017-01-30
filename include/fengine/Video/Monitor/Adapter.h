/*
 * Video adapter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIDEO_ADAPTER_H__
#define __FORK_VIDEO_ADAPTER_H__


#include <string>
#include <iostream>


namespace Fork
{

namespace Video
{


//! Hardware adpater (or video card) information structure.
struct Adapter
{
    /**
    Prints this renderer information into the specified output stream.
    \tparam Stream Specifies the stream type. This could be std::cout or IO::Log::Stream.
    \code
    // Example for the standard- and log output:
    myRendererInfo.Print(std::cout);
    myRendererInfo.Print(IO::Log::Stream());
    \endcode
    */
    template <class Stream> void Print(Stream& outputStream) const
    {
        auto PrintLine = [&](const char* head, const std::string& body)
        {
            if (!body.empty())
                outputStream << head << body << std::endl;
        };

        PrintLine("Graphics Device: ", graphicsDeviceDesc);
        PrintLine("Hardware Vendor: ", hardwareVendor);
        PrintLine("Renderer: ", rendererVersion);
        PrintLine("Shader Model: ", shaderModel);
    }

    std::string graphicsDeviceDesc; //!< Graphics hardware device description (e.g. "NVIDIA GeForce GTX 670").
    std::string hardwareVendor;     //!< Graphics hardware vendor name (e.g. "NVIDIA Corporation" or "AMD Inc.").
    std::string rendererVersion;    //!< Renderer API version (e.g. "OpenGL 4.0" or "Direct3D 11.0").
    std::string shaderModel;        //!< Shader programming model (e.g. "4.00 NVIDIA via Cg compiler" or "HLSL Shader Model 5").
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================