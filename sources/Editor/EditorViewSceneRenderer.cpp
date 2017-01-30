/*
 * Editor view scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorViewSceneRenderer.h"
#include "Core/Devices.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/Shader/StandardShader/SimpleColoredConstantBuffers.h"


namespace Fork
{

namespace Editor
{


using namespace Video::StandardShader;


/*
 * SimpleColoredVertex internal structure
 */

//! Vertex structure for the "SimpleColored" shader.
struct SimpleColoredVertex
{
    static Video::VertexFormat Format()
    {
        Video::VertexFormat format;

        format.SetupAttributes(
            {
                { "coord", Video::RendererDataTypes::Float, 4 },
                { "color", Video::RendererDataTypes::Float, 4 },
            }
        );

        return format;
    }

    Math::Vector4f      coord;
    Video::ColorRGBAf   color;
};


/*
 * SimpleColoredGeometry class
 */

class EditorViewSceneRenderer::SimpleColoredGeometry :
    public Scene::BaseMeshGeometry<SimpleColoredVertex, unsigned char>
{
    
    public:
        
        Types Type() const
        {
            return Types::CustomA;
        }

        void AddVertex(const Math::Vector4f& coord, const Video::ColorRGBAf& color = {})
        {
            vertices.push_back({ coord, color });
        }

        void AddLine(const Math::Line4f& line, const Video::ColorRGBAf& color = {})
        {
            vertices.push_back({ line.start, color });
            vertices.push_back({ line.end  , color });
        }
        
};


/* 
 * EditorViewSceneRenderer class
 */

EditorViewSceneRenderer::EditorViewSceneRenderer()
{
    CreateExtViewModels();
    CreateExtModelShader();
}
EditorViewSceneRenderer::~EditorViewSceneRenderer()
{
}

void EditorViewSceneRenderer::RenderSceneFromEditorView(
    const Scene::CameraNode& camera, const EditorView::Configuration& config)
{
    /* Setup camera view  */
    SetupView(camera);

    /* Draw additional editor models */
    DrawExtModels(config);
}


/*
 * ======= Private: =======
 */

void EditorViewSceneRenderer::CreateExtViewModels()
{
    gridModel_ = std::make_unique<SimpleColoredGeometry>();
    GenerateGridModel(*gridModel_);

    originModel_ = std::make_unique<SimpleColoredGeometry>();
    GenerateOriginModel(*originModel_);
}

void EditorViewSceneRenderer::CreateExtModelShader()
{
    extModelShader_ = Video::RenderSystem::Active()->GetShaderProvider()->CreateShaderCompositionFromResource(
        Video::ShaderProvider::StandardShaders::SimpleColored,
        SimpleColoredVertex::Format()
    );
    constBuffer_ = extModelShader_->CreateAndAttachConstantBuffer("vertexParam", "SimpleColored");
}

void EditorViewSceneRenderer::GenerateGridModel(SimpleColoredGeometry& geometry)
{
    const float gridSize    = 10.0f;
    const float step        = 1.0f;

    const Video::ColorRGBAf color { 0.6f };

    for (float pos = -gridSize; pos <= gridSize; pos += step)
    {
        geometry.AddLine({ { -gridSize, 0, pos }, { gridSize, 0, pos } }, color); // <-- X axis aligned
        geometry.AddLine({ { pos, 0, -gridSize }, { pos, 0, gridSize } }, color); // <-- Z axis aligned
    }

    geometry.SetupHardwareBuffer();
    geometry.primitiveType = Video::GeometryPrimitives::Lines;
}

void EditorViewSceneRenderer::GenerateOriginModel(SimpleColoredGeometry& geometry)
{
    const float originSize = 2.0f;

    geometry.AddLine({ { 0, 0, 0 }, { originSize, 0, 0 } }, { 1, 0, 0 });
    geometry.AddLine({ { 0, 0, 0 }, { 0, originSize, 0 } }, { 0, 1, 0 });
    geometry.AddLine({ { 0, 0, 0 }, { 0, 0, originSize } }, { 0, 0, 1 });

    geometry.SetupHardwareBuffer();
    geometry.primitiveType = Video::GeometryPrimitives::Lines;
}

void EditorViewSceneRenderer::DrawExtModels(const EditorView::Configuration& config)
{
    if (!config.showGrid && !config.showOrigin)
        return;
    
    auto renderContext = Video::RenderContext::Active();

    renderContext->BindShader(extModelShader_.get());
    
    /*
    All extended models don't have a world matrix
    -> Reset world matrix
    */
    renderContext->SetupWorldMatrix({});

    /* Update constant buffer (WVP matrix and base color) */
    SimpleColoredShader::ConstBuffer constBufferData
    {
        renderContext->WVPMatrix(), {}
    };
    Devices::renderSystem->UpdateBuffer(constBuffer_.get(), constBufferData);

    /* Draw geometries */
    if (config.showGrid)
        DrawSimpleColoredGeometry(*gridModel_);
    if (config.showOrigin)
        DrawSimpleColoredGeometry(*originModel_);
}

void EditorViewSceneRenderer::DrawSimpleColoredGeometry(const SimpleColoredGeometry& geometry)
{
    auto renderContext = Video::RenderContext::Active();

    renderContext->BindVertexBuffer(geometry.GetVertexBuffer());
    renderContext->SetupDrawMode(geometry.primitiveType);

    renderContext->Draw(geometry.vertices.size());
}


} // /namespace Editor

} // /namespace Fork



// ========================