/*
 * Selector view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Selector/SelectorView.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/Shader/StandardShader/UtilCommonModelConstantBuffers.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Utility
{


using namespace Video::StandardShader;

/*
 * SelectorView class
 */

SelectorView::SelectorView(
    Scene::SceneManager* sceneManager, const SelectorModel* model, const Video::TextureFont* labelFont) :
        labelFont_  { labelFont },
        model_      { model     }
{
    ASSERT_POINTER( sceneManager );
    ASSERT_POINTER( model        );
    ASSERT_POINTER( labelFont    );

    CreateModels(sceneManager);
    CreateShader();
}
SelectorView::~SelectorView()
{
}

void SelectorView::Draw(
    Video::PrimitiveRenderer* renderer,
    const Math::Transform3Df& transform,
    const Math::Vector3<SelectorModel::AxisState>& axesState,
    const DrawFlags::DataType flags)
{
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    ASSERT_POINTER(renderer);
    
    /* Store previous render state */
    const auto prevLineSize = renderContext->GetRenderState().lineSize;

    /* Setup base transformation */
    const auto baseMatrix = transform.GetMatrix();
    Math::Matrix4f matrix;

    /* Setup model visibility by flags */
    Math::Vector3b axisVisible { axesState.x.isEnabled, axesState.y.isEnabled, axesState.z.isEnabled };

    if ((flags & DrawFlags::DrawSelectedAxesOnly) != 0 && GetModel()->IsActive())
    {
        axisVisible.x = (axisVisible.x && axesState.x.isActive);
        axisVisible.y = (axisVisible.y && axesState.y.isActive);
        axisVisible.z = (axisVisible.z && axesState.z.isActive);
    }

    bool planeZYVisible = (axisVisible.z && axisVisible.y);
    bool planeYXVisible = (axisVisible.y && axisVisible.x);
    bool planeXZVisible = (axisVisible.x && axisVisible.z);

    /* Draw 3D lines */
    renderer->BeginDrawing3D(false);
    {
        /* Draw Z axis */
        matrix = baseMatrix;
        if (axisVisible.z)
            DrawAxis(renderer, matrix, axesState.z, config.axes.z);

        /* Draw Y axis */
        matrix.RotateX(-Math::pi*0.5f);
        if (axisVisible.y)
            DrawAxis(renderer, matrix, axesState.y, config.axes.y);

        /* Draw X axis */
        matrix.RotateY(Math::pi*0.5f);
        if (axisVisible.x)
            DrawAxis(renderer, matrix, axesState.x, config.axes.x);

        if (GetModel()->AreAxesPlanesEnabled())
        {
            /* Draw ZY plane */
            matrix = baseMatrix;
            if (planeZYVisible)
            {
                DrawAxesPlane(
                    renderer, matrix,
                    axesState.z, config.axes.z,
                    axesState.y, config.axes.y
                );
            }

            /* Draw YX plane */
            matrix.RotateY(Math::pi*0.5f);
            if (planeYXVisible)
            {
                DrawAxesPlane(
                    renderer, matrix,
                    axesState.x, config.axes.x,
                    axesState.y, config.axes.y
                );
            }

            /* Draw XZ plane */
            matrix.RotateZ(Math::pi*0.5f);
            if (planeXZVisible)
            {
                DrawAxesPlane(
                    renderer, matrix,
                    axesState.x, config.axes.x,
                    axesState.z, config.axes.z
                );
            }
        }
    }
    renderer->EndDrawing3D();

    /* Setup arrow shader constant buffer */
    unsigned int bufferIndex = 0;

    UtilCommonModelShader::ConstBuffer constBuffer;
    {
        constBuffer.projectionMatrix    = renderContext->GetRenderState().projectionMatrix;
        constBuffer.viewMatrix          = renderContext->GetRenderState().viewMatrix;

        /*
        Setup mesh transform matrix
        -> We need to adjust the basic geometry generated coordinates.
        */
        Math::Matrix4f meshTransform;
        meshTransform.RotateX(Math::pi*0.5f);

        /* Setup world matrices */
        Math::Matrix4f matrices[3];

        matrices[2] = baseMatrix;

        matrices[1] = baseMatrix;
        matrices[1].RotateX(-Math::pi*0.5f);

        matrices[0] = baseMatrix;
        matrices[0].RotateY(Math::pi*0.5f);

        for (size_t axisIndex = 0; axisIndex < 3; ++axisIndex)
        {
            if (axisVisible[axisIndex] && axesState[axisIndex].visibility > Math::epsilon)
            {
                auto& matrix = constBuffer.worldMatrices[bufferIndex];

                matrix = matrices[axisIndex];

                matrix.Translate({ 0, 0, 1 });
                matrix.Scale(Math::Vector3f(config.arrowSize));

                matrix *= meshTransform;

                constBuffer.colors[bufferIndex] = AxisColor(axesState[axisIndex], config.axes[axisIndex]);

                ++bufferIndex;
            }
        }

        constBuffer.viewPosition = Math::Point4f(renderContext->GetRenderState().cameraMatrix.GetPosition());
    }
    renderSystem->UpdateBuffer(constBuffer_.get(), constBuffer);

    /* Draw arrow models */
    auto arrowModel = CurrentArrowModel();

    renderContext->BindShader(arrowShader_.get());
    
    renderContext->BindVertexBuffer(arrowModel->GetVertexBuffer());
    renderContext->BindIndexBuffer(arrowModel->GetIndexBuffer());
    {
        renderContext->SetupDrawMode(Video::GeometryPrimitives::Triangles);
        renderContext->DrawInstancedIndexed(arrowModel->indices.size(), bufferIndex);
    }
    renderContext->UnbindIndexBuffer();
    renderContext->UnbindVertexBuffer();

    /* Restore previous render state */
    renderContext->SetupLineSize(prevLineSize);

    if (GetModel()->IsPlaneSelection())
    {
        /* Draw plane selection */
        renderer->BeginDrawing3D(false);
        {
            /* Draw ZY plane */
            matrix = baseMatrix;
            if (planeZYVisible)
            {
                DrawAxesPlaneSelection(
                    renderer, matrix,
                    axesState.z, axesState.y
                );
            }

            /* Draw YX plane */
            matrix.RotateY(Math::pi*0.5f);
            if (planeYXVisible)
            {
                DrawAxesPlaneSelection(
                    renderer, matrix,
                    axesState.x, axesState.y
                );
            }

            /* Draw XZ plane */
            matrix.RotateZ(Math::pi*0.5f);
            if (planeXZVisible)
            {
                DrawAxesPlaneSelection(
                    renderer, matrix,
                    axesState.x, axesState.z
                );
            }
        }
        renderer->EndDrawing3D();
    }

    if ((flags & DrawFlags::DrawAxesLabels) != 0)
    {
        /* Draw 2D text */
        renderer->BeginDrawing2D();
        {
            /* Setup 2D text positions */
            static const float labelOffset = 1.2f;

            Math::Matrix4f wvMatrix;
            GetModel()->RetrieveWorldViewMatrix(wvMatrix);

            Math::Point4f points[3];

            points[0] = wvMatrix * Math::Point4f(labelOffset, 0, 0);
            points[1] = wvMatrix * Math::Point4f(0, labelOffset, 0);
            points[2] = wvMatrix * Math::Point4f(0, 0, labelOffset);

            /* Draw X, Y, Z labels */
            for (size_t i = 0; i < 3; ++i)
            {
                const char* label = ("X\0Y\0Z" + i*2);

                if (axisVisible[i] && GetModel()->GetProjection().ProjectPoint(points[i]))
                {
                    /* Draw axis label ("X", "Y", "Z") */
                    renderer->DrawText2D(
                        GetFont(), Math::Floor(points[i].Vec2()),
                        label, {}, Video::PrimitiveRenderer::FontDrawFlags::Center
                    );
                }
            }
        }
        renderer->EndDrawing2D();
    }
}

void SelectorView::Draw(
    Video::PrimitiveRenderer* renderer, const DrawFlags::DataType flags)
{
    Draw(renderer, GetModel()->GetTransform(), GetModel()->GetAxesState(), flags);
}


/*
 * ======= Protected: =======
 */

Video::ColorRGBAf SelectorView::AxisColor(
    bool isSelected, const Configuration::Axis& axisConfig, float alpha) const
{
    Video::ColorRGBAf color;

    if (isSelected)
        color = axisConfig.selectColor;      // <-- Axis is selected => selection color.
    else if (GetModel()->IsActive())
        color = config.inactiveAxisColor;    // <-- Selector is transforming, but this axis is not selected => inactive color.
    else
        color = axisConfig.normalColor;          // <-- No transformation and no selection => normal color.

    color.a = alpha;

    return color;
}

Video::ColorRGBAf SelectorView::AxisColor(
    const SelectorModel::AxisState& axisState, const Configuration::Axis& axisConfig) const
{
    /* Get axis color and set alpha channel to axis visibility factor */
    return AxisColor(axisState.isSelected, axisConfig, axisState.visibility);
}

void SelectorView::DrawAxis(
    Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
    const SelectorModel::AxisState& axisState, const Configuration::Axis& axisConfig)
{
    /* Setup render state */
    RenderCtx()->SetupLineSize(axisState.isSelected ? config.selectAxisWidth : config.normalAxisWidth);
    RenderCtx()->SetupWorldMatrix(matrix);

    /* Draw axis line */
    renderer->DrawLine({ { 0, 0, 0, 1 }, { 0, 0, 1, 1 } }, AxisColor(axisState, axisConfig));
}

void SelectorView::DrawAxesPlane(
    Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
    const SelectorModel::AxisState& axisStateA, const Configuration::Axis& axisConfigA,
    const SelectorModel::AxisState& axisStateB, const Configuration::Axis& axisConfigB)
{
    const bool isPlaneSelected = (axisStateA.isSelected && axisStateB.isSelected);

    /* Setup colors */
    const auto colorA = AxisColor(isPlaneSelected, axisConfigA, axisStateA.visibility);
    const auto colorB = AxisColor(isPlaneSelected, axisConfigB, axisStateB.visibility);
    
    auto colorAB = (!isPlaneSelected && GetModel()->IsActive() ? colorA : colorA + colorB);
    colorAB.a = colorA.a * colorB.a;

    /* Setup render state */
    RenderCtx()->SetupLineSize(isPlaneSelected ? config.selectAxisWidth : config.normalAxisWidth);
    RenderCtx()->SetupWorldMatrix(matrix);

    /* Draw axes lines */
    const auto& len = GetModel()->GetAxesPlaneSize();

    renderer->DrawLine({ { 0, len, 0 }, { 0, len, len } }, colorB, colorAB);
    renderer->DrawLine({ { 0, 0, len }, { 0, len, len } }, colorA, colorAB);
}

void SelectorView::DrawAxesPlaneSelection(
    Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
    const SelectorModel::AxisState& axisStateA, const SelectorModel::AxisState& axisStateB)
{
    const bool isPlaneSelected = (axisStateA.isSelected && axisStateB.isSelected);

    if (isPlaneSelected)
    {
        const auto& len = GetModel()->GetAxesPlaneSize();
        const auto& planeColor = config.selectPlaneColor;

        /* Setup render state */
        RenderCtx()->SetupWorldMatrix(matrix);

        /* Draw plane quad */
        renderer->Draw(
            Video::GeometryPrimitives::TriangleStrip,
            { { 0, 0, 0 }, { 0, len, 0 }, { 0, 0, len }, { 0, len, len }, { 0, 0, 0 }, { 0, len, 0 } },
            planeColor
        );
    }
}

const Scene::Simple3DMeshGeometry* SelectorView::CurrentArrowModel() const
{
    switch (GetModel()->transformMode)
    {
        case SelectorModel::TransformModes::Translate:
            return arrowModels_[0].get();
        case SelectorModel::TransformModes::Rotate:
            return arrowModels_[1].get();
        case SelectorModel::TransformModes::Scale:
            return arrowModels_[2].get();
    }
    return nullptr;
}


/*
 * ======= Private: =======
 */

void SelectorView::CreateModels(Scene::SceneManager* sceneManager)
{
    static const float arrowMdlSize = 0.6f;

    arrowModels_[0] = sceneManager->GenerateCone({ arrowMdlSize*0.5f, arrowMdlSize*2.0f, 15 });
    arrowModels_[1] = sceneManager->GenerateIcoSphere({ arrowMdlSize*0.5f, 2 });
    arrowModels_[2] = sceneManager->GenerateCube(arrowMdlSize*0.5f);
}

void SelectorView::CreateShader()
{
    arrowShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        Video::ShaderProvider::StandardShaders::UtilCommonModel,
        Video::Simple3DVertex::Format()
    );
    constBuffer_ = arrowShader_->CreateAndAttachConstantBuffer("vertexParam", "UtilCommonModel");
}


/*
 * Configuration structure
 */

static const float defInactiveAxisClr = 0.75f;

SelectorView::Configuration::Configuration() :
    axes                { {{ 1, 0, 0 }}, {{ 0, 1, 0 }}, {{ 0, 0, 1 }} },
    inactiveAxisColor   { Video::ColorRGBf{ defInactiveAxisClr}, 1    },
    selectPlaneColor    { 1, 1, 0, 0.5f                               }
{
}


/*
 * Axis structure
 */

SelectorView::Configuration::Axis::Axis(const Video::ColorRGBAf& normalColor) :
    normalColor{ normalColor },
    selectColor{ 1, 1, 0     }
{
}
SelectorView::Configuration::Axis::Axis(int)
{
    /* Dummy */
}


} // /namespace Utility

} // /namespace Fork



// ========================