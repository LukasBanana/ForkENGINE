/*
 * Primitive renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/Core/Projection.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"

#include "Video/Font/FontBuildException.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"

#include "IO/Core/Log.h"

#include "Platform/Video/FontBuilder.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


using namespace StandardShader;

PrimitiveRenderer::PrimitiveRenderer(RenderSystem* renderSystem) :
    renderSystem_{ renderSystem }
{
    if (!RenderCtx())
        throw RenderContextException("\"Video::PrimitiveRenderer\" requires an active render context");

    /* Create shaders */
    auto shaderProvider = renderSystem_->GetShaderProvider();

    basicDrawing_       .CreateShader(shaderProvider);
    fontDrawing_        .CreateShader(shaderProvider);
    bezierDrawing_      .CreateShader(shaderProvider);
    imageArrayDrawing_  .CreateShader(shaderProvider);

    /* Create font vertex buffer atlas */
    fontDrawing_.vertexBufferAtlas = renderSystem_->CreateVertexBuffer();

    /* Create depth-stencil state for drawing operations */
    DepthStencilState::Description depthStencilDesc;
    {
        depthStencilDesc.isDepthTestEnabled = false;
    }
    depthStencilState_ = renderSystem_->CreateDepthStencilState(depthStencilDesc);
}
PrimitiveRenderer::~PrimitiveRenderer()
{
}

void PrimitiveRenderer::BeginDrawing2D(bool setupDefaultProjection)
{
    ASSERT_3STATE_BEGIN_ID(0);

    auto renderContext = RenderCtx();

    /* Setup blend state for drawing operations */
    prevDepthStencilState_ = renderContext->GetRenderState().depthStencilState;
    renderContext->ChangeState(depthStencilState_.get());

    /* Bind font vertex buffer atlas */
    renderContext->BindVertexBuffer(fontDrawing_.vertexBufferAtlas.get());

    if (setupDefaultProjection)
        Setup2DProjection();
}

void PrimitiveRenderer::EndDrawing2D()
{
    ASSERT_3STATE_END_ID(0);

    auto renderContext = RenderCtx();

    /* Restore previous state */
    renderContext->UnbindVertexBuffer();

    if (prevDepthStencilState_)
        renderContext->ChangeState(prevDepthStencilState_);

    //!TODO! -> pop previous shader from stack !!!
    /* Unbind shader */
    renderContext->UnbindShader();
}

void PrimitiveRenderer::BeginDrawing3D(bool setupDefaultProjection)
{
    ASSERT_3STATE_BEGIN_ID(1);

    /* Just bind the dummy vertex buffer to enable geometry vertex instancing */
    RenderCtx()->BindVertexBuffer(renderSystem_->GetEmptyVertexBuffer());

    if (setupDefaultProjection)
        Setup3DProjection();
}

void PrimitiveRenderer::EndDrawing3D()
{
    ASSERT_3STATE_END_ID(1);

    /* Restore previous state */
    RenderCtx()->UnbindVertexBuffer();

    //!TODO! -> pop previous shader from stack !!!
    /* Unbind shader */
    RenderCtx()->UnbindShader();
}

void PrimitiveRenderer::Setup2DProjection(const Math::Transform2Df& viewTransform, const Math::Size2f& viewAreaSize)
{
    auto renderContext = RenderCtx();

    /* Setup 2D projection matrix */
    Math::Matrix4f projectionMatrix;
    Projectionf::Build2DProjectionLT(projectionMatrix, viewAreaSize);

    renderContext->SetupProjectionMatrix(projectionMatrix);

    /* Setup world matrix with the view transform */
    renderContext->SetupViewMatrix({});
    renderContext->SetupWorldMatrix(viewTransform.GetMatrix());
}

void PrimitiveRenderer::Setup2DProjection(const Math::Transform2Df& viewTransform)
{
    Setup2DProjection(
        viewTransform,
        RenderCtx()->GetVideoMode().resolution.Cast<float>()
    );
}

void PrimitiveRenderer::Setup3DProjection(const Math::Transform3Df& viewTransform)
{
    auto renderContext = RenderCtx();

    /* Setup 3D projection matrix */
    //...

    /* Setup world matrix with the view transform */
    renderContext->SetupViewMatrix({});
    renderContext->SetupWorldMatrix(viewTransform.GetMatrix());
}

void PrimitiveRenderer::DrawPoint(const Math::Point2f& position, const Video::ColorRGBAf& color)
{
    DrawPoint({ position.x, position.y, 0, 1 }, color);
}

void PrimitiveRenderer::DrawPoint(const Math::Point4f& position, const Video::ColorRGBAf& color)
{
    auto renderContext = RenderCtx();

    /* Setup vertex constant buffer */
    auto& constBuffer = basicDrawing_.vertexConstBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    constBuffer.coords[0] = position;
    constBuffer.colors[0] = color;

    renderSystem_->UpdateBuffer(basicDrawing_.vertexConstBuffer.get(), constBuffer);

    /* Setup pixel constant buffer */
    PrimitiveDrawingShader::PixelConstBuffer constBufferTex;
    constBufferTex.texEnabled = 0;

    renderSystem_->UpdateBuffer(basicDrawing_.pixelConstBuffer.get(), constBufferTex);

    /* Draw point */
    renderContext->BindShader(basicDrawing_.shader.get());
    renderContext->SetupDrawMode(GeometryPrimitives::Points);
    renderContext->Draw(1);
}

void PrimitiveRenderer::DrawLine(const Math::Line2f& line, const Video::ColorRGBAf& color)
{
    DrawLine({ { line.start.x, line.start.y, 0, 1 }, { line.end.x, line.end.y, 0, 1 } }, color, color);
}

void PrimitiveRenderer::DrawLine(const Math::Line2f& line, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB)
{
    DrawLine({ { line.start.x, line.start.y, 0, 1 }, { line.end.x, line.end.y, 0, 1 } }, colorA, colorB);
}

void PrimitiveRenderer::DrawLine(const Math::Line4f& line, const Video::ColorRGBAf& color)
{
    DrawLine(line, color, color);
}

void PrimitiveRenderer::DrawLine(const Math::Line4f& line, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB)
{
    auto renderContext = RenderCtx();

    /* Setup vertex constant buffer */
    auto& constBuffer = basicDrawing_.vertexConstBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    constBuffer.coords[0] = line.start;
    constBuffer.colors[0] = colorA;

    constBuffer.coords[1] = line.end;
    constBuffer.colors[1] = colorB;

    renderSystem_->UpdateBuffer(basicDrawing_.vertexConstBuffer.get(), constBuffer);

    /* Setup pixel constant buffer */
    PrimitiveDrawingShader::PixelConstBuffer constBufferTex;
    constBufferTex.texEnabled = 0;

    renderSystem_->UpdateBuffer(basicDrawing_.pixelConstBuffer.get(), constBufferTex);

    /* Draw line */
    renderContext->BindShader(basicDrawing_.shader.get());
    renderContext->SetupDrawMode(GeometryPrimitives::Lines);
    renderContext->Draw(2);
}

void PrimitiveRenderer::DrawRect(const Math::Rectf& rect, const Video::ColorRGBAf& color, bool useTexture)
{
    DrawRect(rect, color, color, color, color, useTexture);
}

void PrimitiveRenderer::DrawRect(
    const Math::Rectf& rect,
    const Video::ColorRGBAf& colorLT, const Video::ColorRGBAf& colorRT,
    const Video::ColorRGBAf& colorRB, const Video::ColorRGBAf& colorLB,
    bool useTexture)
{
    auto renderContext = RenderCtx();

    /* Setup vertex constant buffer */
    auto& constBuffer = basicDrawing_.vertexConstBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    constBuffer.coords[0] = { rect.left, rect.top, 0, 1 };
    constBuffer.colors[0] = colorLT;

    constBuffer.coords[1] = { rect.right, rect.top, 0, 1 };
    constBuffer.colors[1] = colorRT;

    constBuffer.coords[2] = { rect.left, rect.bottom, 0, 1 };
    constBuffer.colors[2] = colorLB;

    constBuffer.coords[3] = { rect.right, rect.bottom, 0, 1 };
    constBuffer.colors[3] = colorRB;

    if (useTexture)
    {
        constBuffer.texCoords[0] = { 0, 0, 0, 1 };
        constBuffer.texCoords[1] = { 1, 0, 0, 1 };
        constBuffer.texCoords[2] = { 0, 1, 0, 1 };
        constBuffer.texCoords[3] = { 1, 1, 0, 1 };
    }

    renderSystem_->UpdateBuffer(basicDrawing_.vertexConstBuffer.get(), constBuffer);

    /* Setup pixel constant buffer */
    PrimitiveDrawingShader::PixelConstBuffer constBufferTex;
    constBufferTex.texEnabled = (useTexture ? 1 : 0);

    renderSystem_->UpdateBuffer(basicDrawing_.pixelConstBuffer.get(), constBufferTex);

    /* Draw rectangle */
    renderContext->BindShader(basicDrawing_.shader.get());
    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
    renderContext->Draw(4);
}

void PrimitiveRenderer::Draw(
    const GeometryPrimitives primitives, const std::vector<Math::Point4f>& coords,
    const std::vector<Math::Vector4f>& texCoords, const std::vector<Video::ColorRGBAf>& colors, bool useTexture)
{
    auto renderContext = RenderCtx();

    /* Setup pixel constant buffer */
    PrimitiveDrawingShader::PixelConstBuffer constBufferTex;
    constBufferTex.texEnabled = (useTexture ? 1 : 0);

    renderSystem_->UpdateBuffer(basicDrawing_.pixelConstBuffer.get(), constBufferTex);

    /* Bind shader for primitive drawing */
    renderContext->BindShader(basicDrawing_.shader.get());
    renderContext->SetupDrawMode(primitives);

    /* Setup world-view-projection matrix for vertex constant buffer */
    auto& constBuffer = basicDrawing_.vertexConstBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    /* Draw batches of primitives (maximal 32 vertices per batch) */
    auto numVertices = std::min({ coords.size(), colors.size(), texCoords.size() });
    
    size_t vertexIt = 0;
    size_t vertexOffset = 0;

    auto CopyVertex = [&](size_t dstIndex, size_t srcIndex)
    {
        constBuffer.coords   [dstIndex] = coords   [srcIndex];
        constBuffer.texCoords[dstIndex] = texCoords[srcIndex];
        constBuffer.colors   [dstIndex] = colors   [srcIndex];
    };

    while (numVertices > 0)
    {
        /* Copy vertices for current batch */
        auto numBatchVertices = std::min(numVertices, PrimitiveDrawingShader::VertexConstBuffer::maxNumVertices);

        for (auto i = vertexOffset; i < numBatchVertices; ++i, ++vertexIt)
            CopyVertex(i, vertexIt);

        renderSystem_->UpdateBuffer(basicDrawing_.vertexConstBuffer.get(), constBuffer);

        /* Draw primitives for current batch */
        renderContext->Draw(static_cast<unsigned int>(numBatchVertices));
        
        /* Decrement vertex counter */
        numVertices -= numBatchVertices;

        if (numVertices > 0)
        {
            /* Determine frist vertex of the next batch (required for 'fans' or 'strips') */
            switch (primitives)
            {
                case GeometryPrimitives::LineStrip:
                case GeometryPrimitives::LineStripAdjacency:
                    /* Set previous vertex as the new first vertex */
                    CopyVertex(0, vertexIt - 1);
                    vertexOffset = 1;
                    break;

                case GeometryPrimitives::TriangleStrip:
                case GeometryPrimitives::TriangleStripAdjacency:
                    /* Set previous two vertices as the new first two vertices */
                    CopyVertex(0, vertexIt - 2);
                    CopyVertex(1, vertexIt - 1);
                    vertexOffset = 2;
                    break;

                case GeometryPrimitives::TriangleFan:
                    /* Keep first vertex and set previous vertex as the new second vertex */
                    CopyVertex(1, vertexIt - 1);
                    vertexOffset = 2;
                    break;

                default:
                    vertexOffset = 0;
                    break;
            }

            numVertices += vertexOffset;
        }
    }
}

void PrimitiveRenderer::Draw(
    const GeometryPrimitives primitives, const std::vector<Math::Point4f>& coords, const Video::ColorRGBAf& color)
{
    auto renderContext = RenderCtx();

    /* Setup pixel constant buffer */
    PrimitiveDrawingShader::PixelConstBuffer constBufferTex;
    constBufferTex.texEnabled = 0;

    renderSystem_->UpdateBuffer(basicDrawing_.pixelConstBuffer.get(), constBufferTex);

    /* Bind shader for primitive drawing */
    renderContext->BindShader(basicDrawing_.shader.get());
    renderContext->SetupDrawMode(primitives);

    /* Setup world-view-projection matrix for vertex constant buffer */
    auto& constBuffer = basicDrawing_.vertexConstBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    /* Draw batches of primitives (maximal 32 vertices per batch) */
    auto numVertices = coords.size();
    
    size_t vertexIt = 0;
    size_t vertexOffset = 0;

    auto CopyVertex = [&](size_t dstIndex, size_t srcIndex)
    {
        constBuffer.coords[dstIndex] = coords[srcIndex];
        constBuffer.colors[dstIndex] = color;
    };

    while (numVertices > 0)
    {
        /* Copy vertices for current batch */
        auto numBatchVertices = std::min(numVertices, PrimitiveDrawingShader::VertexConstBuffer::maxNumVertices);

        for (auto i = vertexOffset; i < numBatchVertices; ++i, ++vertexIt)
            CopyVertex(i, vertexIt);

        renderSystem_->UpdateBuffer(basicDrawing_.vertexConstBuffer.get(), constBuffer);

        /* Draw primitives for current batch */
        renderContext->Draw(static_cast<unsigned int>(numBatchVertices));
        
        /* Decrement vertex counter */
        numVertices -= numBatchVertices;

        if (numVertices > 0)
        {
            /* Determine frist vertex of the next batch (required for 'fans' or 'strips') */
            switch (primitives)
            {
                case GeometryPrimitives::LineStrip:
                case GeometryPrimitives::LineStripAdjacency:
                    /* Set previous vertex as the new first vertex */
                    CopyVertex(0, vertexIt - 1);
                    vertexOffset = 1;
                    break;

                case GeometryPrimitives::TriangleStrip:
                case GeometryPrimitives::TriangleStripAdjacency:
                    /* Set previous two vertices as the new first two vertices */
                    CopyVertex(0, vertexIt - 2);
                    CopyVertex(1, vertexIt - 1);
                    vertexOffset = 2;
                    break;

                case GeometryPrimitives::TriangleFan:
                    /* Keep first vertex and set previous vertex as the new second vertex */
                    CopyVertex(1, vertexIt - 1);
                    vertexOffset = 2;
                    break;

                default:
                    vertexOffset = 0;
                    break;
            }

            numVertices += vertexOffset;
        }
    }
}

/* --- Curves --- */

void PrimitiveRenderer::DrawBezier(
    const BezierCurve2f& curve, const Video::ColorRGBAf& color, unsigned int segments)
{
    DrawBezier(curve.points, color, color, segments);
}

void PrimitiveRenderer::DrawBezier(
    const BezierCurve2f& curve, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB, unsigned int segments)
{
    std::array<Math::Vector4f, 4u> ctrlPoints
    {{
        { curve.points[0].x, curve.points[0].y, 0, 1 },
        { curve.points[1].x, curve.points[1].y, 0, 1 },
        { curve.points[2].x, curve.points[2].y, 0, 1 },
        { curve.points[3].x, curve.points[3].y, 0, 1 }
    }};
    DrawBezier(ctrlPoints, colorA, colorB, segments);
}

void PrimitiveRenderer::DrawBezier(
    const BezierCurve4f& curve, const Video::ColorRGBAf& color, unsigned int segments)
{
    DrawBezier(curve.points, color, color, segments);
}

void PrimitiveRenderer::DrawBezier(
    const BezierCurve4f& curve, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB, unsigned int segments)
{
    auto renderContext = RenderCtx();

    /* Setup constant buffer */
    auto& constBuffer = bezierDrawing_.constBufferData;

    constBuffer.wvpMatrix = renderContext->WVPMatrix();

    for (size_t i = 0; i < 4; ++i)
        constBuffer.ctrlPoints[i] = curve.points[i];

    constBuffer.colors[0] = colorA;
    constBuffer.colors[1] = colorB;

    constBuffer.segments = static_cast<int>(segments);

    renderSystem_->UpdateBuffer(bezierDrawing_.constBuffer.get(), constBuffer);

    /* Draw bezier curve */
    renderContext->BindShader(bezierDrawing_.shader.get());
    renderContext->SetupDrawMode(GeometryPrimitives::LineStrip);
    renderContext->Draw(segments + 1);
}

/* --- Images --- */

void PrimitiveRenderer::DrawImage(
    const Texture2D* texture, const Math::Point2f& position, const Math::Size2f& size, const Video::ColorRGBAf& color)
{
    if (texture)
    {
        auto renderContext = RenderCtx();
        renderContext->BindTexture(texture);
        DrawRect({ position, size }, color, true);
        renderContext->UnbindTexture(texture);
    }
    else
        DrawRect({ position, size }, color, false);
}

void PrimitiveRenderer::DrawImage(
    const Texture2D* texture, const Math::Point2f& position, const Video::ColorRGBAf& color)
{
    if (texture)
        DrawImage(texture, position, texture->GetSize().Cast<float>(), color);
}

void PrimitiveRenderer::SetupImageArray(const std::vector<ImageArrayEntry>& entries)
{
    imageArrayDrawing_.numEntries = std::min(entries.size(), ImageArrayDrawingShader::EntryConstBuffer::maxNumImages);

    /* Copy image array entries into constant buffer */
    ImageArrayDrawingShader::EntryConstBuffer constBuffer;
    auto& destEntries = constBuffer.entries;

    for (unsigned int i = 0; i < imageArrayDrawing_.numEntries; ++i)
    {
        destEntries[i].positionSizeLayer    = entries[i].positionSizeLayer;
        destEntries[i].color                = entries[i].color;
    }

    /* Update hardware constant buffer */
    renderSystem_->UpdateBuffer(imageArrayDrawing_.entriesConstBuffer.get(), constBuffer);
}

void PrimitiveRenderer::DrawImageArray(
    const Texture2D* texture, const Math::Point2f& offsetPosition,
    const Math::Size2f& baseSize, const Video::ColorRGBAf& baseColor)
{
    auto renderContext = RenderCtx();

    ASSERT_POINTER(texture);
    if (!texture->HasArray())
        throw InvalidArgumentException(__FUNCTION__, "texture", "Passed object is not a valid 2D array texture");
    
    if (imageArrayDrawing_.numEntries == 0)
        return;

    /* Setup constant buffer */
    auto& constBuffer = imageArrayDrawing_.mainConstBufferData;

    constBuffer.wvpMatrix           = renderContext->WVPMatrix();
    constBuffer.basePositionAndSize = { offsetPosition.x, offsetPosition.y, baseSize.width, baseSize.height };
    constBuffer.baseColor           = baseColor;

    renderSystem_->UpdateBuffer(imageArrayDrawing_.mainConstBuffer.get(), constBuffer);

    /* Draw bezier curve */
    renderContext->BindTexture(texture);
    {
        renderContext->BindShader(imageArrayDrawing_.shader.get());
        renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
        renderContext->DrawInstanced(4, 0, imageArrayDrawing_.numEntries);
    }
    renderContext->UnbindTexture(texture);
}

/* --- Font --- */

TextureFontPtr PrimitiveRenderer::CreateTextureFont(const Font::Description& fontDesc)
{
    IO::Log::Message("Create texture font { " + fontDesc.Desc() + " }");
    IO::Log::ScopedIndent indent;

    /* Build font */
    FontGlyphSet glyphSet;
    ImageUBytePtr fontAtlasImage;

    try
    {
        fontAtlasImage = Platform::FontBuilder::BuildFontAtlasImage(fontDesc, glyphSet);
    }
    catch (const FontBuildException& err)
    {
        IO::Log::Error(err);
        return nullptr;
    }

    /* Create font texture */
    auto texture = renderSystem_->CreateTexture2D();

    fontAtlasImage->AdjustFormatAlignment();
    renderSystem_->WriteTexture(texture.get(), *fontAtlasImage);

    const auto invImageSize = Math::Size2f(1.0f) / fontAtlasImage->GetSize().Sz2().Cast<float>();

    /* Create font object */
    auto font = std::make_shared<TextureFont>(fontDesc, glyphSet, texture);

    /* Create vertex buffer partition */
    std::vector<FontGlyphVertices> vertexDataPartition(glyphSet.glyphs.size());

    auto SetupGlyphVertex = [&](FontVertex& vertex, int x, int y, int tx, int ty)
    {
        vertex.coord = { static_cast<float>(x), static_cast<float>(y) };
        vertex.texCoord = { invImageSize.width * tx, invImageSize.height * ty };
    };

    size_t i = 0;
    for (const auto& glyph : glyphSet.glyphs)
    {
        auto& vertices = vertexDataPartition[i].vertices;

        SetupGlyphVertex( vertices[0], 0                                 , 0                                 , glyph.rect.left , glyph.rect.top    );
        SetupGlyphVertex( vertices[1], glyph.rect.right - glyph.rect.left, 0                                 , glyph.rect.right, glyph.rect.top    );
        SetupGlyphVertex( vertices[2], 0                                 , glyph.rect.bottom - glyph.rect.top, glyph.rect.left , glyph.rect.bottom );
        SetupGlyphVertex( vertices[3], glyph.rect.right - glyph.rect.left, glyph.rect.bottom - glyph.rect.top, glyph.rect.right, glyph.rect.bottom );

        ++i;
    }

    /* Add font to vertex-buffer atlas */
    font->partition_ = fontDrawing_.vertexDataAtlas.Add(
        font.get(),
        vertexDataPartition.data(),
        vertexDataPartition.size()*sizeof(FontGlyphVertices)
    );

    UpdateFontVertexBufferAtlas();

    return font;
}

void PrimitiveRenderer::ReleaseTextureFont(TextureFontPtr& font)
{
    /* Remove font from the vertex-buffer atlas */
    fontDrawing_.vertexDataAtlas.Remove(font.get());
    font = nullptr;

    //!TODO! -> update partitions for all TextureFont objects!!!

    /* Update vertex-buffer atlas */
    UpdateFontVertexBufferAtlas();
}

void PrimitiveRenderer::DrawText2D(
    const TextureFont* font, Math::Point2f position, const std::string& textUTF8,
    const Video::ColorRGBAf& color, const FontDrawFlags::DataType& flags)
{
    auto renderContext = RenderCtx();

    ASSERT_POINTER(font);
    
    /* Get font meta data */
    const auto& glyphSet = font->GetGlyphSet();

    const unsigned int vertexIndexOffset = font->GetPartition().GetOffset() / sizeof(FontVertex);

    /* Setup font drawing flags */
    if ((flags & FontDrawFlags::CenterX) != 0)
        position.x += std::floor(-0.5f * font->TextWidth(textUTF8));
    if ((flags & FontDrawFlags::CenterY) != 0)
        position.y += std::floor(-0.5f * font->GetDesc().size.height);

    /* Setup constant buffer for projection and color */
    FontDrawingShader::ConstBuffer constBuffer { renderContext->WVPMatrix(), color };

    constBuffer.wvpMatrix.Translate({ position.x, position.y, 0.0f });

    BeginDrawingText(font);
    {
        /* Draw each character for the text */
        float move = 0.0f;

        for (auto chr : textUTF8)
        {
            DrawFontGlyph(
                glyphSet, vertexIndexOffset, constBuffer, move,
                static_cast<wchar_t>(static_cast<unsigned char>(chr))
            );
        }
    }
    EndDrawingText(font);
}

void PrimitiveRenderer::DrawText2D(
    const TextureFont* font, Math::Point2f position, const std::wstring& textUTF16,
    const Video::ColorRGBAf& color, const FontDrawFlags::DataType& flags)
{
    auto renderContext = RenderCtx();

    ASSERT_POINTER(font);
    
    /* Get font meta data */
    const auto& glyphSet = font->GetGlyphSet();

    const unsigned int vertexIndexOffset = font->GetPartition().GetOffset() / sizeof(FontVertex);

    /* Setup font drawing flags */
    if ((flags & FontDrawFlags::CenterX) != 0)
        position.x += std::floor(-0.5f * font->TextWidth(textUTF16));
    if ((flags & FontDrawFlags::CenterY) != 0)
        position.y += std::floor(-0.5f * font->GetDesc().size.height);

    /* Setup constant buffer for projection and color */
    FontDrawingShader::ConstBuffer constBuffer { renderContext->WVPMatrix(), color };

    constBuffer.wvpMatrix.Translate({ position.x, position.y, 0.0f });

    BeginDrawingText(font);
    {
        /* Draw each character for the text */
        float move = 0.0f;

        for (auto chr : textUTF16)
            DrawFontGlyph(glyphSet, vertexIndexOffset, constBuffer, move, chr);
    }
    EndDrawingText(font);
}

VertexFormat PrimitiveRenderer::FontVertexFormat()
{
    VertexFormat fontVertexFormat;

    fontVertexFormat.AddAttribute({ "coord",    RendererDataTypes::Float, 2 });
    fontVertexFormat.AddAttribute({ "texCoord", RendererDataTypes::Float, 2 });
    
    return fontVertexFormat;
}


/*
 * ======= Private: =======
 */

void PrimitiveRenderer::UpdateFontVertexBufferAtlas()
{
    /* Update entire font vertex buffer atlas */
    renderSystem_->WriteBuffer(
        fontDrawing_.vertexBufferAtlas.get(),
        PrimitiveRenderer::FontVertexFormat(),
        fontDrawing_.vertexDataAtlas.GetRawBuffer(),
        fontDrawing_.vertexDataAtlas.Size()
    );
}

void PrimitiveRenderer::BeginDrawingText(const TextureFont* font)
{
    auto renderContext = RenderCtx();

    renderContext->BindShader(fontDrawing_.shader.get());
    renderContext->BindTexture(font->GetTexture());

    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
}

void PrimitiveRenderer::EndDrawingText(const TextureFont* font)
{
    auto renderContext = RenderCtx();

    renderContext->UnbindTexture(font->GetTexture());
}

void PrimitiveRenderer::DrawFontGlyph(
    const FontGlyphSet& glyphSet, const unsigned int vertexIndexOffset,
    FontDrawingShader::ConstBuffer& constBuffer, float& move, wchar_t chr)
{
    auto renderContext = RenderCtx();

    /* Get current glyph from text */
    const auto& glyph = glyphSet[chr];

    /* Offset movement */
    move += static_cast<float>(glyph.startOffset);

    constBuffer.wvpMatrix.Translate({ move, 0, 0 });
    renderSystem_->UpdateBuffer(fontDrawing_.constBuffer.get(), constBuffer);

    move = 0.0f;

    /* Draw current character */
    const unsigned int vertexIndex = (chr - glyphSet.firstGlyph)*4;
    renderContext->Draw(4, vertexIndexOffset + vertexIndex);

    /* Character with and white space movement */
    move += static_cast<float>(glyph.drawnWidth + glyph.whiteSpace);
}


/*
 * BasicDrawing structure
 */

void PrimitiveRenderer::BasicDrawing::CreateShader(Video::ShaderProvider* shaderProvider)
{
    shader = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::PrimitiveDrawing,
        { /* -- this shader has an empty vertex format -- */ }
    );
    vertexConstBuffer   = shader->CreateAndAttachConstantBuffer( "vertexParam", "PrimitiveDrawing" );
    pixelConstBuffer    = shader->CreateAndAttachConstantBuffer( "pixelParam",  "PrimitiveDrawing" );
}


/*
 * FontDrawing structure
 */

void PrimitiveRenderer::FontDrawing::CreateShader(Video::ShaderProvider* shaderProvider)
{
    shader = shaderProvider->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::FontDrawing,
        PrimitiveRenderer::FontVertexFormat()
    );
    constBuffer = shader->CreateAndAttachConstantBuffer("vertexParam", "FontDrawing");
}


/*
 * BezierDrawing structure
 */

void PrimitiveRenderer::BezierDrawing::CreateShader(Video::ShaderProvider* shaderProvider)
{
    shader = shaderProvider->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::BezierDrawing,
        { /* -- this shader has an empty vertex format -- */ }
    );
    constBuffer = shader->CreateAndAttachConstantBuffer("vertexParam", "BezierDrawing");
}


/*
 * ImageArrayDrawing structure
 */

void PrimitiveRenderer::ImageArrayDrawing::CreateShader(Video::ShaderProvider* shaderProvider)
{
    shader = shaderProvider->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::ImageArrayDrawing,
        { /* -- this shader has an empty vertex format -- */ }
    );
    mainConstBuffer     = shader->CreateAndAttachConstantBuffer( "vertexParam",       "ImageArrayDrawing" );
    entriesConstBuffer  = shader->CreateAndAttachConstantBuffer( "imageArrayEntries", "ImageArrayDrawing" );
}


} // /namespace Video

} // /namespace Fork



// ========================