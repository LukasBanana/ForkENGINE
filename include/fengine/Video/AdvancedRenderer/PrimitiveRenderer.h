/*
 * Primitive renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PRIMITIVE_RENDERER_H__
#define __FORK_PRIMITIVE_RENDERER_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Core/ThreeStateAssertion.h"
#include "Core/Container/PartitionContainer.h"

#include "Math/Geometry/Line.h"
#include "Math/Core/Rect.h"
#include "Math/Core/Transform2D.h"
#include "Math/Core/Transform3D.h"
#include "Math/Common/StaticBezier.h"

#include "Video/Core/ColorRGBA.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/Font/TextureFont.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"

#include "Video/RenderSystem/Shader/StandardShader/FontDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/PrimitiveDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/BezierDrawingConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/ImageArrayDrawingConstantBuffers.h"

#include <memory>
#include <functional>


namespace Fork
{

namespace Video
{


//! Video type for bezier curves with 4 component vectors.
typedef Math::StaticBezier<Math::Vector4f, 4> BezierCurve4f;
//! Video type for bezier curves with 2 component vectors.
typedef Math::StaticBezier<Math::Vector2f, 4> BezierCurve2f;


class Texture2D;
class Font;
class RenderSystem;
class ShaderProvider;

DECL_SHR_PTR(PrimitiveRenderer);

/**
Primitive renderer class. This is the main renderer for primitive 2D and 3D
drawing (or rather rendering) operations such as points, lines, rectangles, text and curves.
\note A valid render system and render context must be created before your can allocate this class!
*/
class FORK_EXPORT PrimitiveRenderer
{
    
    public:
        
        /* === Structures === */

        /**
        Font drawing flags.
        \see DrawText2D
        */
        struct FORK_EXPORT FontDrawFlags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                CenterX = (1 << 0),             //!< Draw text with centered x position.
                CenterY = (1 << 1),             //!< Draw text with centered y position.
                Center  = (CenterX | CenterY),  //!< Draw text with centered x- and y position.
            };
        };

        /**
        Image array drawing entry.
        \see SetupImageArray
        */
        struct FORK_EXPORT ImageArrayEntry
        {
            ImageArrayEntry() = default;
            ImageArrayEntry(
                const Math::Point2f& position, float layer, float size = 1.0f, const Video::ColorRGBAf& color = {}) :
                    positionSizeLayer   { position.x, position.y, size, layer },
                    color               { color                               }
            {
            }

            //! Returns the position: { positionSizeLayer.x, positionSizeLayer.y }.
            inline Math::Point2f Position() const
            {
                return { positionSizeLayer.x, positionSizeLayer.y };
            }
            //! Returns the size: positionSizeLayer.z;
            inline float Size() const
            {
                return positionSizeLayer.z;
            }
            //! Returns the layer: positionSizeLayer.w.
            inline float Layer() const
            {
                return positionSizeLayer.w;
            }

            Math::Vector4f positionSizeLayer;   //!< 4 component vector for image- position (xy), size (z) and layer (w).
            Video::ColorRGBAf color;            //!< Image color (rgba).
        };

        /* === Functions === */

        /**
        Constructs the primitive renderer.
        \param[in] renderSystem Raw-pointer to the render system.
        \throws RenderContextException If there is no active render context.
        \throws InvalidArgumentException If an internal shader was modified and could not be loaded correctly.
        */
        PrimitiveRenderer(RenderSystem* renderSystem);
        virtual ~PrimitiveRenderer();

        /**
        Starts the mode for 2D drawing. This should be called before a 2D drawing operation is done.
        \param[in] setupDefaultProjection Specifies whether to setup the default 2D projection.
        If this is true "Setup2DProjection" with default parameters is called. By default false.
        \remarks Example usage:
        \code
        primitiveRenderer->BeginDrawing2D();
        {
            // Draw 2D primitives here ...
        }
        primitiveRenderer->EndDrawing2D();
        \endcode
        \throws InvalidStateException (Only in debug mode) If "BeginDrawing2D" was already called before "EndDrawing2D" was called.
        \see Setup2DProjection
        */
        virtual void BeginDrawing2D(bool setupDefaultProjection = true);
        /**
        Ends the mode for 2D drawing. This should be called after all 2D drawing operations are done.
        \throws InvalidStateException (Only in debug mode) If "EndDrawing2D" was already called before "BeginDrawing2D" was called.
        */
        virtual void EndDrawing2D();

        /**
        Begins the mode for 3D drawing. This should be called before a 3D drawing operation is done (not required for scene rendering!).
        \param[in] setupDefaultProjection Specifies whether to setup the default 3D projection.
        If this is true "Setup3DProjection" with default parameters is called. By default true.
        \remarks Example usage:
        \code
        primitiveRenderer->BeginDrawing3D();
        {
            // Draw 3D primitives here ...
        }
        primitiveRenderer->EndDrawing3D();
        \see Setup3DProjection
        \throws InvalidStateException (Only in debug mode) If "BeginDrawing3D" was already called before "EndDrawing3D" was called.
        \endcode
        */
        virtual void BeginDrawing3D(bool setupDefaultProjection = true);
        /**
        Ends the mode for 3D drawing. This should be called after all 3D drawing operations are done.
        \throws InvalidStateException (Only in debug mode) If "EndDrawing3D" was already called before "BeginDrawing3D" was called.
        */
        virtual void EndDrawing3D();

        /**
        Sets the projection for 2D drawing. This will build a left-top 2D projection,
        i.e. the coordinate system origin will be on the upper left corner of the screen.
        This also depends on the current render context resolution.
        \param[in] viewTransform Specifies an optional 2D base view transformation.
        This will be used for the world matrix.
        \param[in] viewAreaSize Specifies the view area size. This is typically the current render context resolution.
        \see Video::Projection::Build2DProjectionLT
        \see Video::RenderContext::GetVideoMode
        */
        virtual void Setup2DProjection(const Math::Transform2Df& viewTransform, const Math::Size2f& viewAreaSize);
        /**
        Sets the projection for 2D drawing with the current render context resolution as view area size.
        \see Setup2DProjection(const Math::Transform2Df&, const Math::Size2f&)
        */
        virtual void Setup2DProjection(const Math::Transform2Df& viewTransform = {});
        /**
        Sets the projection for 3D drawing.
        \remarks This will currently only set the view- and world matrix.
        \param[in] viewTransform Specifies an optional 3D base view transformation.
        This will be used for the world matrix.
        */
        virtual void Setup3DProjection(const Math::Transform3Df& viewTransform = {});

        /* --- Primitives --- */

        /**
        Draws a 2D point.
        \remarks This is equivalent to:
        \code
        DrawPoint({ position.x, position.y, 0, 1 }, color);
        \endcode
        \see DrawPoint(const Math::Point4f&, const Video::ColorRGBAf&)
        */
        virtual void DrawPoint(const Math::Point2f& position, const Video::ColorRGBAf& color = {});
        /**
        Draws a 3D point.
        \param[in] position Specifies the global position (in world space).
        \param[in] color Specifies the color for the point. By default white.
        */
        virtual void DrawPoint(const Math::Point4f& position, const Video::ColorRGBAf& color = {});

        /**
        Draws a 2D line with a single color.
        \see DrawLine(const Math::Line4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawLine(
            const Math::Line2f& line, const Video::ColorRGBAf& color = {}
        );
        /**
        Draws a 2D line with two colors.
        \see DrawLine(const Math::Line4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawLine(
            const Math::Line2f& line, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB
        );
        /**
        Draws a 3D line with a single color.
        \see DrawLine(const Math::Line4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawLine(
            const Math::Line4f& line, const Video::ColorRGBAf& color = {}
        );
        /**
        Draws a 3D line with two colors.
        \param[in] line Specifies the line (in world space).
        \param[in] colorA Specifies the color for the line start point.
        \param[in] colorB Specifies the color for the line end point.
        */
        virtual void DrawLine(
            const Math::Line4f& line, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB
        );

        /**
        Draws a rectangle with a single color.
        \remarks This is equivalent to:
        \code
        DrawRect(rect, color, color, color, color, useTexture);
        \endcode
        \see DrawRect(const Math::Rectf&, const Video::ColorRGBAf&, const Video::ColorRGBAf&, const Video::ColorRGBAf&, const Video::ColorRGBAf&, bool)
        */
        virtual void DrawRect(
            const Math::Rectf& rect, const Video::ColorRGBAf& color = {}, bool useTexture = false
        );
        /**
        Draws a rectangle with multiple colors.
        \param[in] rect Specifies the rectangle.
        \param[in] colorLT Specifies the color for the left-top corner.
        \param[in] colorRT Specifies the color for the right-top corner.
        \param[in] colorRB Specifies the color for the right-bottom corner.
        \param[in] colorLB Specifies the color for the left-bottom corner.
        \param[in] useTexture Specifies whether the currently bound texture will be mapped
        onto the rectangle. If this is true, also the texture coordinates will be set, which will be:
        { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }. By default false.
        */
        virtual void DrawRect(
            const Math::Rectf& rect,
            const Video::ColorRGBAf& colorLT, const Video::ColorRGBAf& colorRT,
            const Video::ColorRGBAf& colorRB, const Video::ColorRGBAf& colorLB,
            bool useTexture = false
        );

        /**
        Draws arbitrary primitives. This depends on the currently
        set drawing mode (see Video::RenderSystem::SetupDrawMode).
        \param[in] primitives Specifies the primitive type.
        \param[in] coords Specifies the vertex coordinates.
        \param[in] texCoords Specifies the vertex texture coordinates.
        \param[in] colors Specifies the vertex colors.
        \param[in] useTexture Specifies whether the currently bound texture will be mapped onto the polygon.
        \see Video::RenderSystem::SetupDrawMode
        */
        virtual void Draw(
            const GeometryPrimitives primitives,
            const std::vector<Math::Point4f>& coords,
            const std::vector<Math::Vector4f>& texCoords,
            const std::vector<Video::ColorRGBAf>& colors,
            bool useTexture = false
        );

        /**
        Draws an abitrary polygon with a single color.
        \see void Draw(const std::vector<Math::Vector4f>&, const std::vector<Math::Vector4f>&, const std::vector<Video::ColorRGBAf>&, bool)
        */
        virtual void Draw(
            const GeometryPrimitives primitives,
            const std::vector<Math::Point4f>& coords, const Video::ColorRGBAf& color
        );

        /* --- Curves --- */

        /**
        Draws a 2D bezier curve (with 4th order, i.e. 4 control points) with a singel color.
        \see DrawBezier(const BezierCurve4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawBezier(
            const BezierCurve2f& curve, const Video::ColorRGBAf& color = {}, unsigned int segments = 32
        );
        /**
        Draws a 2D bezier curve (with 4th order, i.e. 4 control points) with a color transition.
        \see DrawBezier(const BezierCurve4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawBezier(
            const BezierCurve2f& curve, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB, unsigned int segments = 32
        );
        /**
        Draws a 3D bezier curve (with 4th order, i.e. 4 control points) with a singel color.
        \see DrawBezier(const BezierCurve4f&, const Video::ColorRGBAf&, const Video::ColorRGBAf&)
        */
        virtual void DrawBezier(
            const BezierCurve4f& curve, const Video::ColorRGBAf& color = {}, unsigned int segments = 32
        );
        /**
        Draws a 3D bezier curve (with 4th order, i.e. 4 control points) with a color transition.
        \param[in] curve Specifies the bezier curve. This is a type alias
        to the Math::StaticBezier<Math::Vector4f, 4> template instantiation.
        But actually only the bezier control points are used from this class object.
        \param[in] colorA Specifies the color for the first control point.
        \param[in] colorB Specifies the color for the last control point.
        \param[in] segments Specifies the number of line segments which are to be drawn to shape the bezier curve.
        This will set the number of lines used to draw the bezier curve. By default 32.
        \remarks The colors will be interpolated between the bezier curve.
        \remarks If the bezier curve is distributed over an entire 1080p Full-HD screen with a large bending,
        'segments' should be higher than 32, e.g. 100 or higher.
        \see Math::StaticBezier
        */
        virtual void DrawBezier(
            const BezierCurve4f& curve, const Video::ColorRGBAf& colorA, const Video::ColorRGBAf& colorB, unsigned int segments = 32
        );

        /* --- Images --- */

        /**
        Draws a 2D image.
        \param[in] texture Constant raw-pointer to the 2D texture which is to be used to draw the image.
        If this is null, only a single colored rectangle will be drawn.
        \param[in] position Specifies the image position (in screen space).
        \param[in] size Specifies the image size (in screen space).
        \param[in] color Specifies the image color. Each image pixel will be multiplied by this color. By default white.
        */
        virtual void DrawImage(
            const Texture2D* texture,
            const Math::Point2f& position, const Math::Size2f& size,
            const Video::ColorRGBAf& color = {}
        );
        /**
        Draws a 2D image with the size of the used texture.
        \see DrawImage(const Texture2D*, const Math::Point2f&, const Math::Size2f&, const Video::ColorRGBAf&)
        \see Texture2D::GetSize
        */
        virtual void DrawImage(
            const Texture2D* texture, const Math::Point2f& position, const Video::ColorRGBAf& color = {}
        );

        /**
        Initializes all image array entries. This must be called before "DrawImageArray" can be used.
        It must be called only once, but can also be called every frame (if the data is to be changed).
        \param[in] entries Specifies the list of all image array entries.
        The number of entries determines the number of images which will be drawn with "DrawImageArray".
        The maximal number is 2048.
        \see DrawImageArray
        \see ImageArrayEntry
        */
        virtual void SetupImageArray(const std::vector<ImageArrayEntry>& entries);

        /**
        Draws an image array.
        \param[in] texture Constant raw-pointer to a 2D array texture.
        \param[in] offsetPosition Specifies the offset position which will be added
        to each image array entry position.
        \param[in] baseSize Specifies the base image size. This could be 'texture->GetSize()'.
        \param[in] baseColor Specifies the base image color which will be multiplied
        with each image array entry color. By default white.
        \remark Before this function can be used, one has to call "SetupImageArray"
        to initialize all image array entries.
        \throws InvalidArgumentException If 'texture' is null or not a valid 2D array texture.
        \see SetupImageArray
        */
        virtual void DrawImageArray(
            const Texture2D* texture, const Math::Point2f& offsetPosition,
            const Math::Size2f& baseSize, const Video::ColorRGBAf& baseColor = {}
        );

        /* --- Font --- */

        /**
        Creates a texture font with the specified description.
        \param[in] fontDesc Specifies the font description.
        \return Shared pointer to the texture font object or null if the font creation failed.
        \see Font::Description
        \see TextureFont
        */
        TextureFontPtr CreateTextureFont(const Font::Description& fontDesc);
        /**
        Releases the specified texture font.
        \throws PartitionContainerException If the specified font is not contained inside the internal list.
        \todo Incomplete
        */
        void ReleaseTextureFont(TextureFontPtr& font);
        
        /**
        Draws 2-dimensional UTF-8 text with the specified textured font.
        \see FontDrawFlags
        */
        virtual void DrawText2D(
            const TextureFont* font, Math::Point2f position,
            const std::string& textUTF8, const Video::ColorRGBAf& color = {},
            const FontDrawFlags::DataType& flags = 0
        );

        /**
        Draws 2-dimensional UTF-16 text with the specified textured font.
        \see FontDrawFlags
        */
        virtual void DrawText2D(
            const TextureFont* font, Math::Point2f position,
            const std::wstring& textUTF16, const Video::ColorRGBAf& color = {},
            const FontDrawFlags::DataType& flags = 0
        );

        /* === Static functions === */

        //! Returns the vertex format for a textured font.
        static VertexFormat FontVertexFormat();

        /* === Inline functions === */

        //! Returns a raw-pointer to the render system which is used by this primitive renderer.
        inline RenderSystem* GetRenderSystem() const
        {
            return renderSystem_;
        }

        //! Returns a constant raw-pointer to the depth-stencil state for drawing operations.
        inline const DepthStencilState* GetDepthStencilState() const
        {
            return depthStencilState_.get();
        }

    private:
        
        /* === Structures === */

        struct FontVertex
        {
            Math::Vector2f coord;
            Math::Vector2f texCoord;
        };

        struct FontGlyphVertices
        {
            FontVertex vertices[4];
        };

        /* === Functions === */

        void UpdateFontVertexBufferAtlas();

        void BeginDrawingText(const TextureFont* font);
        void EndDrawingText(const TextureFont* font);

        void DrawFontGlyph(
            const FontGlyphSet& glyphSet, const unsigned int vertexIndexOffset,
            StandardShader::FontDrawingShader::ConstBuffer& constBuffer, float& move, wchar_t chr
        );

        /* === Members === */

        //! Reference to the render system.
        RenderSystem*               renderSystem_           = nullptr;

        DepthStencilStatePtr        depthStencilState_;
        const DepthStencilState*    prevDepthStencilState_  = nullptr;

        FORK_DEBUG_3STATE_ID(0);
        FORK_DEBUG_3STATE_ID(1);

        /* --- Drawing objects --- */

        struct FORK_EXPORT BasicDrawing
        {
            void CreateShader(Video::ShaderProvider* shaderProvider);

            ShaderCompositionPtr    shader;
            ConstantBufferPtr       pixelConstBuffer;
            ConstantBufferPtr       vertexConstBuffer;

            StandardShader::PrimitiveDrawingShader::VertexConstBuffer
                vertexConstBufferData;
        };

        BasicDrawing basicDrawing_;

        /* --- Font drawing objects --- */

        struct FORK_EXPORT FontDrawing
        {
            void CreateShader(Video::ShaderProvider* shaderProvider);

            ShaderCompositionPtr                shader;
            ConstantBufferPtr                   constBuffer;

            TextureFont::PartitionContianerType vertexDataAtlas;
            VertexBufferPtr                     vertexBufferAtlas;
        };

        FontDrawing fontDrawing_;

        /* --- Bezier drawing objects --- */

        struct FORK_EXPORT BezierDrawing
        {
            void CreateShader(Video::ShaderProvider* shaderProvider);

            ShaderCompositionPtr    shader;
            ConstantBufferPtr       constBuffer;

            StandardShader::BezierDrawingShader::VertexConstBuffer
                constBufferData;
        };

        BezierDrawing bezierDrawing_;

        /* --- Image array drawing objects --- */

        struct FORK_EXPORT ImageArrayDrawing
        {
            void CreateShader(Video::ShaderProvider* shaderProvider);
            
            ShaderCompositionPtr    shader;
            ConstantBufferPtr       mainConstBuffer;
            ConstantBufferPtr       entriesConstBuffer;

            StandardShader::ImageArrayDrawingShader::MainConstBuffer
                mainConstBufferData;

            unsigned int            numEntries = 0;
        };

        ImageArrayDrawing imageArrayDrawing_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================