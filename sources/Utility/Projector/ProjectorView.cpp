/*
 * Projector view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Projector/ProjectorView.h"
#include "Video/RenderSystem/Shader/StandardShader/UtilCommonModelConstantBuffers.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Core/Exception/NullPointerException.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Utility
{


/*
 * ProjectorView class
 */

ProjectorView::ProjectorView(const ProjectorModel* model, const Video::TextureFont* labelFont) :
    model_      { model     },
    labelFont_  { labelFont }
{
    ASSERT_POINTER(model);
    //ASSERT_POINTER(labelFont);

    CreateModels();
    CreateShader();

    InitWorldMatrices();
}
ProjectorView::~ProjectorView()
{
}

//extern Fork::Math::Vector4f p;

void ProjectorView::Draw(Video::PrimitiveRenderer* renderer, const DrawFlags::DataType flags)
{
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    ASSERT_POINTER(renderer);

    /* Setup projection and view matrices */
    renderContext->SetupViewport(GetModel()->GetViewport());

    renderContext->SetupViewMatrix(GetModel()->ViewMatrix());
    renderContext->SetupProjectionMatrix(GetModel()->GetVisualProjection());

    /* Setup constant buffer base matrices */
    constBufferData_.projectionMatrix = renderContext->GetRenderState().projectionMatrix;
    constBufferData_.viewMatrix = renderContext->GetRenderState().viewMatrix;

    constBufferData_.viewPosition = Math::Point4f(renderContext->GetRenderState().cameraMatrix.GetPosition());

    /* Setup constant buffer colors */
    constBufferData_.colors[0] = config.mainTrigger.GetColorByState(
        GetModel()->GetMainTriggerState().isSelected
    );

    constBufferData_.instanceIndexOffset = 0;

    for (size_t i = 0; i < 6; ++i)
    {
        const auto& axis = config.triggers[i];
        const auto& state = GetModel()->GetTriggerStates()[i];

        auto& color = constBufferData_.colors[i + 1];

        color = axis.GetColorByState(state.isSelected);
        color.a = state.visibility;
    }

    renderSystem->UpdateBuffer(constBuffer_.get(), constBufferData_);

    /* Draw geometry */
    renderContext->BindShader(modelShader_.get());

    renderContext->SetupDrawMode(Video::GeometryPrimitives::Triangles);

    /* Draw cube */
    renderContext->BindVertexBuffer(cubeModel_.GetVertexBuffer());
    renderContext->BindIndexBuffer(cubeModel_.GetIndexBuffer());
    {
        renderContext->DrawIndexed(cubeModel_.indices.size());
    }
    //renderContext->UnbindIndexBuffer();
    //renderContext->UnbindVertexBuffer();

    /* Draw six cones */
    renderContext->BindVertexBuffer(coneModel_.GetVertexBuffer());
    renderContext->BindIndexBuffer(coneModel_.GetIndexBuffer());
    {
        constBufferData_.instanceIndexOffset = 1;
        renderSystem->UpdateBuffer(constBuffer_.get(), constBufferData_);

        renderContext->DrawInstancedIndexed(coneModel_.indices.size(), 6);
    }
    renderContext->UnbindIndexBuffer();
    renderContext->UnbindVertexBuffer();

    #if 0
    // Draw 3D
    renderer->BeginDrawing3D(false);
    {
        renderer->Setup3DProjection(GetModel()->ViewMatrix());

        renderContext->SetupPointSize(5);
        renderer->DrawPoint(p, { 1, 0, 1 });
        renderContext->SetupPointSize(1);
    }
    renderer->EndDrawing3D();
    #endif

    /* Reset viewport */
    renderContext->SetupViewport(GetModel()->GetProjection().GetViewport());
}


/* 
 * ======= Private: =======
 */

void ProjectorView::CreateModels()
{
    Scene::GeometryGenerator::GenerateCube(cubeModel_);
    cubeModel_.SetupHardwareBuffer();

    Scene::GeometryGenerator::GenerateCone(coneModel_, { 1.0f, 1.0f });
    coneModel_.SetupHardwareBuffer();
}

void ProjectorView::CreateShader()
{
    modelShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        Video::ShaderProvider::StandardShaders::UtilCommonModel,
        Video::Simple3DVertex::Format()
    );
    constBuffer_ = modelShader_->CreateAndAttachConstantBuffer("vertexParam", "UtilCommonModel");
}

void ProjectorView::InitWorldMatrices()
{
    /* Matrix settings */
    static const Math::Vector3f cubeScale { 0.3f };
    static const Math::Vector3f conePos { 0, -0.55f, 0 };
    static const Math::Vector3f coneScale { 0.2f, 0.5f, 0.2f };

    /* Rotate the world matrices for the six cone models */
    auto& matrices = constBufferData_.worldMatrices;
    const auto deg90 = Math::pi*0.5f;
    const auto deg180 = Math::pi;

    matrices[1].RotateZ(-deg90 ); // <-- Left
    matrices[2].RotateZ( deg180); // <-- Top
    matrices[3].RotateX( deg90 ); // <-- Front
    matrices[4].RotateZ( deg90 ); // <-- Right
    /* Do not change matrix[5] */ // <-- Bottom
    matrices[6].RotateX(-deg90 ); // <-- Back

    for (size_t i = 1; i <= 6; ++i)
    {
        auto& matrix = matrices[i];
        
        matrix.Translate( conePos   );
        matrix.Scale    ( coneScale );
    }

    /* Setup cube matrix */
    matrices[0].Scale(cubeScale);
}


/*
 * Configuration structure
 */

ProjectorView::Configuration::Configuration() :
    triggers
    {
        {{ 1, 0, 0 }}, {{ 0, 1, 0 }}, {{ 0, 0, 1 }},
        {{ 1, 0, 0 }}, {{ 0, 1, 0 }}, {{ 0, 0, 1 }}
    },
    mainTrigger({ 1, 1, 1 })
{
}


/*
 * Trigger structrure
 */

ProjectorView::Configuration::Trigger::Trigger(const Video::ColorRGBAf& normalColor) :
    normalColor { normalColor },
    selectColor { 1, 1, 0, 1  }
{
}
ProjectorView::Configuration::Trigger::Trigger(int)
{
    /* Dummy */
}

const Video::ColorRGBAf& ProjectorView::Configuration::Trigger::GetColorByState(bool isSelected) const
{
    return isSelected ? selectColor : normalColor;
}


} // /namespace Utility

} // /namespace Fork



// ========================