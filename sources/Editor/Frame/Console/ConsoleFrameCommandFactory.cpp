/*
 * Editor console frame command factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConsoleFrameCommandFactory.h"
#include "ConsoleFrame.h"
#include "Utility/Terminal/StandardCommand.h"
#include "Utility/Terminal/Terminal.h"
#include "Core/Exception/NullPointerException.h"
#include "Engine/EngineInfo.h"
#include "IO/Core/Log.h"
#include "Platform/Core/SystemInfo.h"
#include "Video/RenderSystem/Texture/TextureMentor.h"
#include "Scene/Renderer/LogSceneRenderer.h"
#include "../../Project/ProjectFactory.h"
#include "../../Project/ProjectFolder.h"
#include "../Main/MainFrame.h"
#include "../Main/MainFrameComponentController.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/Devices.h"
#include "../../EditorView.h"
#include "../../Tools/AssetBrowser/AssetBrowser.h"
#include "../../Tools/AssetBrowser/Asset/TextureUIAsset.h"
#include "../../Core/Paths.h"


namespace Fork
{

namespace Editor
{


//!TODO! -> structure these commands into separate commands and files!!!

using namespace Utility;

ConsoleFrameCommandFactory::ConsoleFrameCommandFactory()
{
    /* Add further commands */
    AddCommand(
        std::make_shared<StandardCommand>(
            "system.info",
            "Prints the system information",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                Engine::PrintEngineInfo(Video::RenderContext::Active());
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "py.platform.info",
            "Prints the platform information (requires Python)",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                if (Devices::scriptInterpreter)
                {
                    Devices::scriptInterpreter->RunFromFile(Paths::dirScripts + "python/PlatformInfo.py");
                    Devices::scriptInterpreter->RunFromString("PrintPlatformInfo()");
                }
                else
                    IO::Log::Error("Python interpreter not available");
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "vsync",
            "Enables or disables vertical synchronisation",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                bool enableVSync = false;

                if (args.size() == 2)
                {
                    /* Set VSync configuration from 2nd argument */
                    const auto& arg = args.back();

                    if (arg == "-on")
                        enableVSync = true;
                    else if (arg == "-off")
                        enableVSync = false;
                    else
                    {
                        IO::Log::Error("Unknown flag \"" + arg + "\" for command \"vsync\"");
                        return false;
                    }
                }
                else
                {
                    /* Switch VSync configuration */
                    enableVSync = !Video::RenderContext::Active()->GetDesc().vsyncDesc.isEnabled;
                }

                IO::Log::Message(enableVSync ? "VSync ON" : "VSync OFF");

                /* Setup new VSync configuration for all render contexts */
                for (const auto& context : Devices::renderSystem->GetRenderContexts())
                    context->EnableVSync(enableVSync);

                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "open",
            "Opens the console frame",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                MainFrame::Instance()->GetConsoleFrame()->Show();
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "close",
            "Closes the console frame",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                MainFrame::Instance()->GetConsoleFrame()->Show(false);
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "profiler",
            "Shows or hides the renderer profiler",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                const auto& viewList = MainFrame::Instance()->GetEditorViews();

                if (!viewList.empty())
                {
                    bool showProfiler = !viewList.front()->config.showProfiler;
                    for (auto& view : viewList)
                        view->config.showProfiler = showProfiler;
                }

                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "assets.count",
            "Counts the number of assets",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                IO::Log::Message("Number of assets: " + ToStr(AssetBrowser::Instance()->GetModel()->GetAssets().size()));
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "assets.enum",
            "Enumerates all assets",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                for (const auto& asset : AssetBrowser::Instance()->GetModel()->GetAssets())
                {
                    switch (asset->Type())
                    {
                        case UIAsset::Types::Texture:
                        {
                            auto assetTex = dynamic_cast<TextureUIAsset*>(asset.get());

                            auto tex = assetTex->GetSharedTextureResource();
                            auto texDesc = Video::TextureMentor::QueryTextureDesc(tex.get());

                            std::string texInfo = ToStr(texDesc.size.width) + " x " + ToStr(texDesc.size.height);
                            
                            if (texDesc.size.depth > 1)
                                texInfo += " x " + ToStr(texDesc.size.depth);
                            else if (texDesc.arraySize > 1)
                                texInfo += " x " + ToStr(texDesc.arraySize);

                            IO::Log::Message(texInfo);
                            terminal->Print(tex);
                        }
                        break;
                    }
                }
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "editor.paths",
            "Prints all editor paths",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                IO::Log::Message("dirConfig: \"" + Paths::dirConfig + "\"");
                IO::Log::Message("dirImages: \"" + Paths::dirImages + "\"");
                IO::Log::Message("dirMisc:   \"" + Paths::dirMisc + "\"");
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "system.workdir",
            "Prints the current working directory (with args: [new_dir])",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                if (args.size() == 2)
                {
                    const auto& arg = args.back();
                    Platform::ChangeWorkingDirectory(UTF8toUTF16(arg));
                }
                IO::Log::Message("Working directory: \"" + UTF16toUTF8(Platform::QueryWorkingDirectory()) + "\"");
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "terminal.filter",
            "Prints all filtered terminal entries (with args: [search_filter])",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                if (args.size() != 2)
                    return false;

                auto searchFilter = UTF8toUTF16(args.back());

                auto terminalEntries = terminal->GetModel()->GetEntries();

                for (const auto& entry : terminalEntries)
                {
                    if (entry->Type() == Utility::TerminalModel::Entry::Types::Text)
                    {
                        auto textEntry = static_cast<Utility::TerminalModel::TextEntry*>(entry.get());
                        const auto& text = textEntry->GetText();

                        if (text.find(searchFilter) != std::string::npos)
                            terminal->GetModel()->AddEntry(entry);
                    }
                }

                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "debug.component.properties",
            "Prints all property names of the current components",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                MainFrame::Instance()->GetComponentCtrl()->GetComponentView()->DebugPropertyNames();
                return true;
            }
        )
    );

    AddCommand(
        std::make_shared<StandardCommand>(
            "debug.scene.hierarchy",
            "Prints the scene with hierarchy",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                Scene::LogSceneRenderer logSceneRenderer;
                auto project = ProjectFolder::Active();
                if (project)
                    logSceneRenderer.RenderScene(project->ActiveSceneGraph());
                return true;
            }
        )
    );

    #if 0

    AddCommand(
        std::make_shared<StandardCommand>(
            "debug.entity.list",
            "Prints the name and ID of all entities",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                for (const auto& entity : EntityFactory::Instance()->GetEntities())
                {
                    std::string text = "GameObject";

                    /*if (entity->GetSceneNode())
                        text += " \"" + entity->GetSceneNode()->metaData.name + "\"";
                    else
                        text += " <unnamed>";*/

                    text += " (ID: " + ToStr(entity->GetID()) + ")";

                    IO::Log::Message(text);
                }
                return true;
            }
        )
    );

    #endif
}


} // /namespace Editor

} // /namespace Fork



// ========================