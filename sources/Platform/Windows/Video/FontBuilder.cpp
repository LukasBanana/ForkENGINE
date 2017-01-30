/*
 * WIN32: Font builder file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Video/FontBuilder.h"
#include "Video/Font/FontBuildException.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Core/TreeHierarchy/ImageTreeNode.h"
#include "Math/Core/Arithmetic/BaseArithmetic.h"

#include <Windows.h>
#include <memory>
#include <array>
#include <cmath>


namespace Fork
{

namespace Platform
{

namespace FontBuilder
{


/* === Internal structures === */

struct Win32FontGlyph;

typedef ImageTreeNode<Win32FontGlyph> Win32FontGlyphNode;
typedef std::unique_ptr<Win32FontGlyphNode> Win32FontGlyphNodePtr;

struct Win32FontGlyph
{
    Win32FontGlyph(HDC dc, wchar_t chr)
    {
        /* Query glyph metrics */
        SIZE sz;
        GetTextExtentPoint32(dc, &chr, 1, &sz);

        ABC abc;
        if (!GetCharABCWidths(dc, chr, chr, &abc))
            throw std::string("Getting font glyph metrics failed");

        /* Setup glyph metrics */
        glyph.startOffset   = abc.abcA;
        glyph.drawnWidth    = abc.abcB;
        glyph.whiteSpace    = abc.abcC;

        size = { glyph.drawnWidth + 2, sz.cy + 2 };
    }

    void ConfirmInsertion(Win32FontGlyphNode* node)
    {
        /* Dummy */
    }

    Video::FontGlyph glyph;
    Math::Size2i size;
};

typedef std::unique_ptr<Win32FontGlyph> Win32FontGlyphPtr;


/* === Internal functions === */

static HFONT CreateWin32Font(const Video::Font::Description& fontDesc)
{
    typedef Video::Font::Flags Flags;
    return CreateFontW(
        fontDesc.size.height,                                               // Height of font
        fontDesc.size.width,                                                // Width of font
        0,                                                                  // Angle of escapement
        0,                                                                  // Orientation angle
        ( fontDesc.flags & Flags::Bold       ) != 0 ? FW_BOLD : FW_NORMAL,  // Font weight
        ( fontDesc.flags & Flags::Italic     ) != 0 ? TRUE : FALSE,         // Italic
        ( fontDesc.flags & Flags::Underlined ) != 0 ? TRUE : FALSE,         // Underline
        ( fontDesc.flags & Flags::StrikeOut  ) != 0 ? TRUE : FALSE,         // Strikeout
        ANSI_CHARSET,                                                       // Character set identifier
        OUT_TT_PRECIS,                                                      // Output precision
        CLIP_DEFAULT_PRECIS,                                                // Clipping precision
        ANTIALIASED_QUALITY,                                                // Output quality
        FF_DONTCARE | DEFAULT_PITCH,                                        // Family and pitch
        fontDesc.name.c_str()                                               // Font name
    );
}

static Math::Size2i ApproximateFontAtlasSize(int visualArea)
{
    visualArea = static_cast<int>(std::sqrt(static_cast<float>(visualArea)));
    auto size = Math::Size2i(Math::RoundPow2(visualArea));

    if (size.width < visualArea)
        size.width *= 2;

    return size;
}


/* === Global functions === */

/*
This function works in serveral phases:
1.) The Win32 font object will be created.
2.) For each font glyph:
  2.1.) Create a respective Win32 font glyph with its font metrics.
3.) Build the font atlas tree hierarchy.
   Create the tree root node.
   For each font glyph:
  3.1.) Insert its image tree node into the root tree node.
4.) Create Win32 bitmap to draw the glyphs into it.
    For each font glyph:
  4.1.) Draw font glyph into the bitmap.
5.) Convert the bitmap into an engine's image buffer object.
*/
FORK_EXPORT Video::ImageUBytePtr BuildFontAtlasImage(const Video::Font::Description& fontDesc, Video::FontGlyphSet& fontGlyphSet)
{
    /* Get active render context */
    if (!Video::RenderContext::Active())
        throw RenderContextException("\"" + std::string(__FUNCTION__) + "\" requires an active render context");

    auto renderContext = Video::RenderContext::Active();
    
    /*
    (Phase 1)
    Get the device context and create a Win32 font object.
    Then select the Win32 font object and store the previously font state.
    */
    HDC deviceContext = *reinterpret_cast<const HDC*>(renderContext->GetFrame()->GetNativeDeviceContext());

    HFONT fontHandle = CreateWin32Font(fontDesc);

    if (!fontHandle)
        throw FontBuildException(fontDesc, "Creating Win32 font failed");

    HGDIOBJ prevFontHandle = SelectObject(deviceContext, fontHandle);

    /**
    (Phase 2)
    Generate all font glyphs and store their metrics.
    If an error occured, throw an exception to the client.
    */
    const auto firstChar = fontGlyphSet.firstGlyph = fontDesc.firstGlyph;
    const auto lastChar = fontGlyphSet.lastGlyph = fontDesc.lastGlyph;

    const size_t numGlyphs = lastChar - firstChar + 1;

    std::vector<Win32FontGlyphPtr> win32GlyphSet(numGlyphs);

    int visualArea = 0;

    try
    {
        /* Create all character glyphs */
        for (wchar_t chr = firstChar; chr <= lastChar; ++chr)
        {
            if (!IsDBCSLeadByte(static_cast<CHAR>(chr)))
            {
                const wchar_t index = chr - firstChar;
                win32GlyphSet[index] = std::make_unique<Win32FontGlyph>(deviceContext, chr);
                visualArea += win32GlyphSet[index]->size.Area();
            }
        }
    }
    catch (const std::string& err)
    {
        /* Release font handle */
        SelectObject(deviceContext, prevFontHandle);
        DeleteObject(fontHandle);

        /* Throw exception to the client */
        throw FontBuildException(fontDesc, err);
    }

    /**
    (Phase 3)
    Now construct the image tree.
    1. Approximate the required font atlas size.
    2. Try to insert each glyph into the tree.
    3. If the size is not large enough, increase the minimal size component by the factor 2 and goto step (3.2).
    */
    auto fontAtlasSize = ApproximateFontAtlasSize(visualArea);

    Win32FontGlyphNodePtr glyphRootNode;
    bool fillTreeCompleted = false;

    while (!fillTreeCompleted)
    {
        /* Create a new glyph tree root node */
        glyphRootNode = std::make_unique<Win32FontGlyphNode>(fontAtlasSize);

        fillTreeCompleted = true;

        /* Insert all font glyphs into the tree */
        for (wchar_t chr = firstChar; chr <= lastChar; ++chr)
        {
            /* Get current glyph */
            const auto index = chr - firstChar;
            auto& win32Glyph = win32GlyphSet[index];

            if (!win32Glyph)
                continue;

            auto node = glyphRootNode->Insert(win32Glyph.get(), win32Glyph->size);
            
            if (!node)
            {
                /*
                Glyph could not be inserted into the tree.
                -> Break insertion
                */
                fillTreeCompleted = false;

                /* Increase texture size and try tree-building again */
                if (fontAtlasSize.width < fontAtlasSize.height)
                    fontAtlasSize.width *= 2;
                else
                    fontAtlasSize.height *= 2;

                break;
            }
            
            /* Store final glyph location inside the glyph atlas */
            win32Glyph->glyph.rect = node->GetRect();
        }
    }

    /*
    (Phase 4)
    Create font bitmap and all required Win32 resources
    to draw each font glyph into this bitmap.
    */
    HBITMAP bitmap          = CreateCompatibleBitmap(deviceContext, fontAtlasSize.width, fontAtlasSize.height);
    HDC bitmapDC            = CreateCompatibleDC(deviceContext);
    
    LOGBRUSH logBrush;
    logBrush.lbStyle        = BS_SOLID;
    logBrush.lbColor        = RGB(0, 0, 0);
    logBrush.lbHatch        = 0;
    
    HBRUSH brush            = CreateBrushIndirect(&logBrush);
    HPEN pen                = CreatePen(PS_NULL, 0, 0);
    
    HGDIOBJ prevBitmap      = SelectObject(bitmapDC, bitmap);
    HGDIOBJ prevBmpPen      = SelectObject(bitmapDC, pen);
    HGDIOBJ prevBmpBrush    = SelectObject(bitmapDC, brush);
    HGDIOBJ prevBmpFont     = SelectObject(bitmapDC, fontHandle);
    
    SetTextColor(bitmapDC, RGB(255, 255, 255));
    
    Rectangle(bitmapDC, 0, 0, fontAtlasSize.width, fontAtlasSize.height);
    SetBkMode(bitmapDC, TRANSPARENT);
    
    auto ReleaseWin32Objects = [&]()
    {
        SelectObject(bitmapDC, prevBitmap);
        SelectObject(bitmapDC, prevBmpPen);
        SelectObject(bitmapDC, prevBmpBrush);
        SelectObject(bitmapDC, prevBmpFont);
    
        SelectObject(deviceContext, prevFontHandle);
        DeleteObject(fontHandle);
    
        DeleteDC(bitmapDC);
        DeleteObject(brush);
        DeleteObject(pen);
        DeleteObject(bitmap);
    };

    fontGlyphSet.glyphs.resize(numGlyphs);

    /*
    Now draw each font glyph into the bitmap and
    store the final image location into the output glyph set
    */
    for (wchar_t chr = firstChar; chr <= lastChar; ++chr)
    {
        /* Get current glyph */
        const auto index = chr - firstChar;
        auto& win32Glyph = win32GlyphSet[index];
        
        if (!win32Glyph)
            continue;
        
        auto& glyph = win32Glyph->glyph;

        /* Draw glyph to bitmap */
        TextOut(
            bitmapDC,
            glyph.rect.left - glyph.startOffset + 1,
            glyph.rect.top + 1,
            &chr, 1
        );
        
        /* Copy glyph information */
        fontGlyphSet[chr] = glyph;

        /* Decrease rectangle area size by 1 pixel to avoid texture bleeding */
        fontGlyphSet[chr].rect.ResizeLinear(-1);
    }

    /*
    (Phase 5)
    Create final font atlas image with the created bitmap.
    */
    /* Get bitmap information */
    BITMAP bmInfo;
    GetObject(bitmap, sizeof(bmInfo), &bmInfo);
    
    /* Get the bitmap's image buffer */
    BITMAPINFOHEADER bi = { 0 };
    bi.biSize           = sizeof(BITMAPINFOHEADER);
    bi.biWidth          = bmInfo.bmWidth;
    bi.biHeight         = bmInfo.bmHeight;
    bi.biPlanes         = 1;
    bi.biBitCount       = 24;
    bi.biCompression    = BI_RGB;
    
    /* Create final */
    auto fontAtlasImage = std::make_shared<Video::ImageUByte>(
        Math::Size3st(bi.biWidth, bi.biHeight, 1),
        Video::ImageColorFormats::RGB
    );

    try
    {
        /* Get the size of the image data */
        if (!GetDIBits(bitmapDC, bitmap, 0, bmInfo.bmHeight, 0, (BITMAPINFO*)&bi, DIB_RGB_COLORS))
            throw std::string("Getting device bitmap information failed");
    
        /* Retrieve the image data */
        if (!GetDIBits(bitmapDC, bitmap, 0, bmInfo.bmHeight, fontAtlasImage->RawBuffer(), (BITMAPINFO*)&bi, DIB_RGB_COLORS))
            throw std::string("Getting device bitmap's image buffer failed");
    }
    catch (const std::string& err)
    {
        /* Clean up and throw exception to the client */
        ReleaseWin32Objects();
        throw FontBuildException(fontDesc, err);
    }

    /* Flip image vertical */
    Video::ImageConverter::FlipImageY(fontAtlasImage->RawBuffer(), fontAtlasImage->GetSize(), 3);

    /* Clean up */
    ReleaseWin32Objects();

    return fontAtlasImage;
}


} // /namespace FontBuilder

} // /namespace Platform

} // /namespace Fork



// ========================