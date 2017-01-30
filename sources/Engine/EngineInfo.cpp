/*
 * Engine info file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/EngineInfo.h"
#include "IO/Core/Log.h"
#include "Core/Version.h"
#include "Platform/Core/SystemInfo.h"


namespace Fork
{

namespace Engine
{


FORK_EXPORT void PrintEngineInfo(const Video::RenderContext* context)
{
    auto PrintChartInfo = [&](const char* desc, const std::string& info)
    {
        if (!info.empty())
            IO::Log::MessageColored(ToStr(desc) + "<0101>" + info + ToStr("</>"));
    };

    /* Print engine header- and copyright information */
    std::string headerInfo;

    headerInfo += "<0111>" + ToStr(GetEngineIDName()) + "</> Version " + EngineVersion();

    #ifndef FORK_COMMERCIAL_USE
    headerInfo += " -- <1101>NON COMMERCIAL VERSION</>";
    #endif

    IO::Log::MessageColored(headerInfo);
    IO::Log::Message("Copyright (c) 2014 by Lukas Hermanns");
    IO::Log::Blank();

    /* Operating system information */
    const auto sysInfo = Platform::QuerySystemInfo();

    std::string platform;

    platform += "<0111>" + sysInfo.osName + "</>";
    platform += " " + sysInfo.osProductType;
    platform += " " + sysInfo.osExtensions;
    platform += " <0110>(" + sysInfo.osBuild + ")</>";
    
    IO::Log::MessageColored(platform);

    /* Compiler information */
    const auto compilerInfo = Platform::QueryCompilerInfo();

    std::string compiler;

    compiler += compilerInfo.compilerVersion;
    if (compilerInfo.hasDebugInfo)
        compiler += " (<1011>DEBUG</>)";
    compiler += " on " + compilerInfo.latestBuild;

    IO::Log::MessageColored(compiler);
    IO::Log::Blank();

    /* Further system information */
    PrintChartInfo("Processors:         ", ToStr(sysInfo.numProcessors));
    PrintChartInfo("Processor Speed:    ", ToStr(sysInfo.processorSpeed) + " MHz");
    PrintChartInfo("CPU Type:           ", Platform::SystemInfo::ToStr(sysInfo.cpuType));
    PrintChartInfo("CPU Architecture:   ", Platform::SystemInfo::ToStr(sysInfo.cpuArch));
    IO::Log::Blank();

    /* Render system information */
    if (context)
    {
        const auto adapter = context->QueryAdapter();

        PrintChartInfo( "Graphics Device:    ", adapter.graphicsDeviceDesc );
        PrintChartInfo( "Hardware Vendor:    ", adapter.hardwareVendor     );
        PrintChartInfo( "Renderer:           ", adapter.rendererVersion    );
        PrintChartInfo( "Shader Model:       ", adapter.shaderModel        );
        IO::Log::Blank();
    }
}


} // /namespace Engine

} // /namespace Fork



// ========================