
// ForkENGINE: Video Test
// 02/03/2014

#include <fengine/import.h>
#include <fengine/using.h>

#include <thread>

using namespace Fork;

Utility::Terminal* termRef = nullptr;

Network::HTTPRecorder http;

std::string httpContent;

class CustomCommandFactory : public Util::CommandFactory
{
    
    public:
        
        CustomCommandFactory()
        {
            auto cmdFunc = [](const IO::CommandLine::ArgumentListType& args, Util::Terminal* terminal, void* userData) -> bool
            {
                if (args.size() != 3)
                {
                    terminal->ErrorNumArgs("http");
                    return false;
                }

                httpContent.clear();

                http.Connect(args[1]);
                http.SendGET(args[2]);

                return true;
            };
            AddCommand(
                std::make_shared<Util::StandardCommand>(
                    "http",
                    "Loads an HTTP resource (with args: [host resource])",
                    cmdFunc
                )
            );
        }

};

void PrintSomething(int threadNr, const std::string& message, int n)
{
    for (int i = 0; i < n; ++i)
    {
        termRef->Message(
            std::string(threadNr, ' ') + "Thread #" + ToStr(threadNr) + ", Message #" + ToStr(i) + " -- " + message
        );
    }
}

class ScriptEvtHandler : public Util::Terminal::ScriptEventHandler
{

    public:
        
        ScriptEvtHandler(Script::ScriptInterpreter* interpreter = nullptr) :
            interpreter_(interpreter)
        {
        }

        void OnRunCode(const std::string& code)
        {
            if (interpreter_)
                interpreter_->RunFromString(code);
            else
            {
                Lang::FSCInterpreter interpreter;
                interpreter.RunScript(code);
            }
        }

    private:
        
        Script::ScriptInterpreter* interpreter_ = nullptr;

};

class VideoTest : public Engine::App
{
    
    public:
        
        void OnMain();

        void OnExit()
        {
            http.Disconnect();
        }

};

void VideoTest::OnMain()
{
    auto renderSystem = engine.CreateRenderSystem(L"ForkRendererGL");

    Video::ContextDescription contextDesc(true, true, 8);
    contextDesc.rendererProfileDesc.useExtProfile = true;
    contextDesc.rendererProfileDesc.useGLCoreProfile = true;
    //contextDesc.rendererProfileDesc.versionGL = { 3, 2 };

    const Math::Size2ui screenSize(800, 600);

    auto renderContext = renderSystem->CreateRenderContext(
        Video::VideoMode(screenSize), contextDesc, L"Video Test"
    );

    renderContext->SetupClearColor(Video::ColorRGBub(20, 40, 100).Cast<float>());

    auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());

    // Create terminal
    Util::Terminal term(primRenderer, std::make_shared<CustomCommandFactory>());
    term.GetView()->groundColor.a = 0.8f;
    //term.Enable(true);

    termRef = &term;

    class LogEventHandler : public IO::Console::LogEventHandler
    {
        public:
            void OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type) override
            {
                auto front = Platform::ConsoleManip::QueryColor();
                termRef->Message(indent + message, Platform::ConsoleManip::ToColor(front | Platform::ConsoleManip::Colors::Intens));
            }
            void OnPrint(const std::string& message, const IO::Log::EntryTypes type) override
            {
                Platform::ConsoleManip::Colors::DataType front, back;
                Platform::ConsoleManip::QueryColor(front, back);
                termRef->Print(message, Platform::ConsoleManip::ToColor(front | Platform::ConsoleManip::Colors::Intens));
            }
            void OnStartLn(const std::string& indent) override
            {
                termRef->Print(indent);
            }
            void OnEndLn() override
            {
                termRef->Blank();
            }
            void OnBlank() override
            {
                termRef->Blank();
            }
    };

    IO::Log::AddEventHandler(std::make_shared<LogEventHandler>());

    Engine::PrintEngineInfo(renderContext.get());

    // Setup frame
    auto frame = renderContext->GetFrame();
    frame->ChangeDesc({ true, false, true });

    class FrameEventHandler : public Platform::Frame::EventHandler
    {
        void OnResize(const Math::Size2i& clientAreaSize)
        {
            auto renderContext = Video::RenderContext::Active();
            renderContext->ChangeVideoMode({ clientAreaSize.Cast<unsigned int>() });
            renderContext->SetupViewport({ {}, clientAreaSize });
        }
    };

    frame->SetEventHandler(std::make_shared<FrameEventHandler>());

    // Get input devices
    auto keyboard = IO::Keyboard::Instance();
    auto mouse = IO::Mouse::Instance();

    // Create texture
    const std::string texPath = "???";
    const std::string texFile = texPath + "StonesColorMap.jpg";//"HDR Test Image 1.hdr";

    Video::TextureManager texMngr;

    auto texA = texMngr.CreateTexture2DFromFile(texFile);//, true);

    //#define TEXARRAY_TEST
    #ifdef TEXARRAY_TEST
    auto texB = texMngr.CreateTexture2DArrayFromFiles(
        {
            texPath + "StonesColorMap.jpg",
            texPath + "StonesNormalMap.jpg",
            texPath + "StonesHeightMap.jpg",
            texPath + "Test_JPEG_Baseline.jpg",
            texPath + "Test_JPEG_Progressive.jpg"
        }
    );
    #endif

    //renderSystem->Bind(texA.get());

    // Create font
    auto fontA = primRenderer->CreateTextureFont({ "arial", 32 });
    auto fontB = primRenderer->CreateTextureFont({ "Times New Roman", 32 });
    auto fontC = primRenderer->CreateTextureFont({ "courier new", 18 });

    Video::Font::Description fontUnicodeDesc { "MingLiU", 15/*64*/ };
    {
        fontUnicodeDesc.firstGlyph = 0x5165;
        fontUnicodeDesc.lastGlyph = 0x5203;
    }
    auto fontUnicode = primRenderer->CreateTextureFont(fontUnicodeDesc);

    // Create standard depth-stencil state
    Video::DepthStencilState::Description depthStencilDesc;
    auto depthStencilState = renderSystem->CreateDepthStencilState(depthStencilDesc);
    renderContext->ChangeState(depthStencilState.get());

    term.Print("Test,");
    term.Print("LOL!", {}, fontB.get());
    term.Print(L"\u5165\u5166\u5167\u5168\u5169\u516a\n", {}, fontUnicode.get());

    Platform::NetworkInterface::StartUp();

    using Lang::AbstractSyntaxTrees::XMLTag;
    using Lang::AbstractSyntaxTrees::XMLAttribute;
    using Lang::AbstractSyntaxTrees::XMLContent;

    class XMLVisitor : public Lang::AbstractSyntaxTrees::Visitor
    {
        
        public:
            
            void VisitXMLTag(XMLTag* node, void* args)
            {
                IO::Log::Message(IO::Log::GetFullIndent() + "XML Tag \"" + node->ident + "\"");
                IO::Log::ScopedIndent indent;

                for (auto attrib : node->attributes)
                    attrib->Visit(this);
                for (auto node : node->nodes)
                    node->Visit(this);
            }

            void VisitXMLAttribute(XMLAttribute* node, void* args)
            {
                IO::Log::Message(IO::Log::GetFullIndent() + "XML Attribute \"" + node->ident + "\", value: \"" + node->value + "\"");
            }

            void VisitXMLContent(XMLContent* node, void* args)
            {
                IO::Log::Message(IO::Log::GetFullIndent() + "XML Content \"" + node->value + "\"");
            }

    };

    XMLVisitor xmlVisitor;

    #define TEXT_ILLUSTRATOR
    #ifdef TEXT_ILLUSTRATOR
    auto richTextRD = std::make_unique<Video::RichTextRenderer>(primRenderer);
    #endif

    //#define HTML_TEST
    #ifdef HTML_TEST

    Utility::HTMLDocument htmlDocument;
    Utility::HTMLRenderer htmlRenderer(primRenderer);

    Lang::XMLParser xmlParserHTML;
    htmlDocument.tags = xmlParserHTML.ParseXMLFile("../HTML/TestHTMLPage.html");

    #endif

    // THREAD TEST
    #if 0
        
    static const int numThreads = 10;
        
    std::unique_ptr<std::thread> threads[numThreads];

    for (int i = 0; i < numThreads; ++i)
    {
        threads[i] = std::make_unique<std::thread>(
            PrintSomething, i, "Hello, World!", 10
        );
    }
        
    for (int i = 0; i < numThreads; ++i)
    {
        threads[i]->join();
        threads[i] = nullptr;
    }

    #endif

    // SCRIPT TEST
    #define SCRIPT_TEST
    #ifdef SCRIPT_TEST

    Script::ScriptInterpreterPtr interpreter = nullptr;

    try
    {
        Script::ScriptInterpreter::EnvironmentDescription envDesc;
        {
            envDesc.scriptEnginePath = L"Python-3.4.2rc1";
            envDesc.scriptModulePath = "scripts/python";
        }
        interpreter = engine.CreateScriptInterpreter(L"ForkScriptPy", envDesc);

        term.SetScriptEventHandler(std::make_shared<ScriptEvtHandler>(interpreter.get()));

        //interpreter->RunFromFile("../Script/TestScript1.py");
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }

    #else

    term.SetScriptEventHandler(std::make_shared<ScriptEvtHandler>());

    #endif

    // Main loop
    while (frame->ReceiveEvents() && !keyboard->KeyDown(IO::KeyCodes::KeyEscape))
    {
        auto motion = mouse->GetMotion().Cast<float>() * 0.01f;

        float spin = 0.1f * mouse->GetWheelMotion();

        if (keyboard->KeyHit(IO::KeyCodes::KeyF3))
            term.Enable(!term.IsEnabled());

        #ifdef SCRIPT_TEST
        if (interpreter)
            interpreter->RunFromFile("../Script/TestScript2.py");
        #endif

        // Update HTTP requests
        if (http.IsConnected() && !http.IsContentComplete())
        {
            std::string content;
            if (http.PollContent(content))
            {
                httpContent += content;

                RemoveCR(content);
                term.Print(content);
            }
        }
        else if (!httpContent.empty())
        {
            #if 1

            // Parse HTTP GET response
            Network::HTTPResponseParser responseParser;
            responseParser.ParseGETResponse(httpContent);
            auto response = responseParser.GetResponseGET();

            // Parse XML body content
            Lang::XMLParser xmlParser;
            auto xmlTags = xmlParser.ParseXMLString(response.bodyContent);

            #ifdef HTML_TEST
            htmlDocument.tags = xmlTags;
            #endif

            //for (auto tag : xmlTags)
            //    tag->Visit(&xmlVisitor);

            #endif

            httpContent.clear();
        }

        // Draw scene
        renderContext->ClearBuffers();
        {
            renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());

            primRenderer->BeginDrawing2D();
            {
                primRenderer->Setup2DProjection();

                #ifndef TEXT_ILLUSTRATOR

                #ifdef HTML_TEST

                if (!htmlDocument.tags.empty())
                {
                    htmlRenderer.DrawDocument(
                        htmlDocument, { { 0, 0 }, renderContext->GetVideoMode().resolution.Cast<float>() }
                    );
                }

                #else

                // Draw text
                primRenderer->DrawText2D(fontA.get(), { 15, 15 }, L"Hello, World!", { 1, 1, 0 });
                primRenderer->DrawText2D(fontB.get(), { 15, 50 }, L"This is just a test");
                primRenderer->DrawText2D(fontC.get(), { 15, 85 }, L"ForkENGINE");

                primRenderer->DrawText2D(fontUnicode.get(), { 250, 15 }, L"\u5165\u5166\u5167\u5168\u5169\u516a");

                // Draw some primitives
                primRenderer->DrawLine(
                    { { 50, 50 }, { 50, 200 } }
                );

                primRenderer->DrawRect(
                    { { 500, 200 }, Math::Size2f{ 100, 100 } },
                    {}, {}, { 1, 0, 0, 1 }, { 1, 0, 0, 1 }
                );

                primRenderer->DrawImage(texA.get(), { 300, 200 }, { 150, 150 });

                // Draw circle
                static size_t numVerts = 10;
                if (mouse->GetWheelMotion() != 0)
                {
                    numVerts += mouse->GetWheelMotion();
                    numVerts = std::max(5u, numVerts);
                    IO::Log::Message("numVerts = " + ToStr(numVerts));
                }

                std::vector<Math::Point4f> coords(numVerts);
                std::vector<Math::Vector4f> texCoords(numVerts);
                std::vector<Video::ColorRGBAf> colors(numVerts);

                coords[0] = { 550.0f, 450.0f, 0, 1 };
                colors[0] = { 1, 0.1f, 0.2f, 1 };

                const float radius = 100.0f;

                for (size_t i = 1; i < numVerts; ++i)
                {
                    float a = static_cast<float>(i - 1)/(numVerts - 2);
                    float c = 1.0f - (Math::Sq((a - 0.5f)*2.0f));
                    coords[i] = coords[0] + Math::Point4f{ std::sin(-a*Math::pi*2.0f)*radius, std::cos(-a*Math::pi*2.0f)*radius, 0, 0 };
                    colors[i] = { c, 1.0f - c, c*c, 1.0 };
                }

                primRenderer->Draw(Video::GeometryPrimitives::TriangleFan, coords, texCoords, colors);

                #ifdef TEXARRAY_TEST
                // Draw image array
                std::vector<Video::PrimitiveRenderer::ImageArrayEntry> imageEntries
                {
                    { { 100, 100 }, 0.0f },
                    { { 250, 100 }, 1.0f },
                    { { 400, 100 }, 2.0f },
                };
                primRenderer->SetupImageArray(imageEntries);
                primRenderer->DrawImageArray(texB.get(), {}, { 100.0f, 100.0f });
                #endif

                // Draw bezier curve
                Video::BezierCurve2f curve;
                {
                    curve.points[0] = { 100, 500 };
                    curve.points[1] = { 250, 150 };
                    curve.points[2] = { 500, 200 };
                    curve.points[3] = { 450, 350 };

                    #if 1//!!
                    curve.points[2] = mouse->GetFrameMousePosition().position.Cast<float>();
                    #endif
                }
                primRenderer->DrawBezier(curve, { 1, 0, 0 }, { 1, 1, 0 }, 64);

                #endif // /HTML_TEST

                #else // TEXT_ILLUSTRATOR

                auto mousePos = mouse->GetFrameMousePosition().position;

                richTextRD->viewOffset = { 100, 200 };
                richTextRD->viewSize = { mousePos.x - 100.0f, mousePos.y - 100.0f };
                richTextRD->textBorder = { 0, 0 };
                richTextRD->textPosition = { 0, 0 };

                #   if 0

                richTextRD->Text(L"Hello, World. This is a simple text view area ;-)");
                richTextRD->PushFont(Video::Font::Description("courier new", 35));
                {
                    richTextRD->PushColor({ 1, 1, 0 });
                    {
                        richTextRD->Text(L"Lukas ");
                    }
                    richTextRD->PopColor();
                    richTextRD->PushFontItalic();
                    {
                        richTextRD->Text(L"Hermanns");
                    }
                    richTextRD->PopFont();
                }
                richTextRD->PopFont();
                richTextRD->Text(L"And this_is_a_very_long_identifier ...");

                #   else

                richTextRD->PushFont(Video::Font::Description("courier new", 20));

                richTextRD->PushFontBold();
                richTextRD->Text(L"def");
                richTextRD->PopFont();
                richTextRD->Text(L" fib(n):");
                richTextRD->NewLine();
                richTextRD->PushFontBold();
                richTextRD->Text(L"  if");
                richTextRD->PopFont();
                richTextRD->Text(L" n < 2:");
                richTextRD->NewLine();
                richTextRD->PushFontBold();
                richTextRD->Text(L"    return");
                richTextRD->PopFont();
                richTextRD->Text(L" 1");
                richTextRD->NewLine();
                richTextRD->PushFontBold();
                richTextRD->Text(L"  else");
                richTextRD->PopFont();
                richTextRD->Text(L":");
                richTextRD->NewLine();
                richTextRD->PushFontBold();
                richTextRD->Text(L"    return");
                richTextRD->PopFont();
                richTextRD->Text(L" fib(n-1)+fib(n-2)");
                richTextRD->NewLine();
                richTextRD->PushFontBold();
                richTextRD->Text(L"for");
                richTextRD->PopFont();
                richTextRD->Text(L" i in range(1, 20):");
                richTextRD->NewLine();
                richTextRD->Text(L"  print(\"fib(\" + str(i) + \") = \" + str(fib(i)))");
                richTextRD->NewLine();

                #   endif

                richTextRD->Flush();

                #endif // /TEXT_ILLUSTRATOR

                // Draw terminal
                if (term.IsEnabled())
                    term.Draw();
            }
            primRenderer->EndDrawing2D();
        }
        renderContext->Present();

        engine.UpdateFrameStates();
    }

    #ifdef TEXT_ILLUSTRATOR
    richTextRD = nullptr;
    #endif

    Platform::NetworkInterface::CleanUp();
}

FORK_IMPLEMENT_APP(VideoTest);


