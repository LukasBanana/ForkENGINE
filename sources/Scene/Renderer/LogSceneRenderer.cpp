/*
 * Log scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/LogSceneRenderer.h"
#include "Scene/Node/SceneNode.h"
#include "IO/Core/Log.h"
#include "IO/Core/Printer.h"
#include "Core/StringModifier.h"
#include "../Geometry/ImportGeometries.h"
#include "../Node/ImportNodes.h"


namespace Fork
{

namespace Scene
{


static void PrintNode(
    const NodeMetaData& metaData, const Transform* transform, std::string text,
    const LogSceneRenderer::RenderFlags::DataType& flags)
{
    if ((flags & LogSceneRenderer::RenderFlags::ShowMetaData) != 0)
    {
        /* Append meta data */
        if (!metaData.name.empty())
            text += " \"" + metaData.name + "\"";
        if (metaData.userDataFlags != 0)
            text += " (Flags: 0x" + NumToHex(metaData.userDataFlags) + ")";
    }

    if (transform && (flags & LogSceneRenderer::RenderFlags::ShowDetails) != 0)
    {
        /* Append transformation */
        const auto& pos = transform->GetPosition();
        text += " @ " + IO::Printer::Print(pos);
    }
    
    /* Print final text line */
    IO::Log::Message(text);
}

void LogSceneRenderer::RenderScene(SceneNode* sceneGraph)
{
    if (sceneGraph)
        sceneGraph->Visit(this);
}

/* --- Scene nodes --- */

void LogSceneRenderer::VisitSceneNode(SceneNode* node)
{
    PrintNode(node->metaData, nullptr, "SceneNode", renderFlags);

    if (!node->GetChildren().empty())
    {
        IO::Log::ScopedIndent indent;
        for (auto& subNode : node->GetChildren())
            subNode->Visit(this);
    }
}

void LogSceneRenderer::VisitGeometryNode(GeometryNode* node)
{
    PrintNode(node->metaData, &node->transform, "GeometryNode", renderFlags);

    if (node->geometry)
    {
        IO::Log::ScopedIndent indent;
        node->geometry->Visit(this);
    }
}

void LogSceneRenderer::VisitLightNode(LightNode* node)
{
    PrintNode(node->metaData, &node->transform, "LightNode", renderFlags);

    /*if (node->lightSource)
    {
        IO::Log::ScopedIndent indent;
        node->lightSource->Visit(this);
    }*/
}

void LogSceneRenderer::VisitCameraNode(CameraNode* node)
{
    PrintNode(node->metaData, &node->transform, "CameraNode", renderFlags);
}

/* --- Geometries --- */

void LogSceneRenderer::VisitLODGeometry(LODGeometry* node)
{
    std::string text = "LODGeometry";

    if ((renderFlags & RenderFlags::ShowDetails) != 0)
        text += " (Sub Geometries: " + ToStr(node->lodGeometries.size()) + ")";

    PrintNode(node->metaData, nullptr, text, renderFlags);

    if (!node->lodGeometries.empty())
    {
        IO::Log::ScopedIndent indent;
        for (auto& geometry : node->lodGeometries)
            geometry->Visit(this);
    }
}

void LogSceneRenderer::VisitBillboardGeometry(BillboardGeometry* node)
{
    PrintNode(node->metaData, nullptr, "BillboardGeometry", renderFlags);
}

void LogSceneRenderer::VisitTerrainGeometry(TerrainGeometry* node)
{
    PrintNode(node->metaData, nullptr, "TerrainGeometry", renderFlags);

    if (node->templateGeometry)
        node->templateGeometry->Visit(this);
}

void LogSceneRenderer::VisitMeshGeometry(MeshGeometry* node)
{
    PrintNode(node->metaData, nullptr, "MeshGeometry", renderFlags);
}

void LogSceneRenderer::VisitCompositionGeometry(CompositionGeometry* node)
{
    std::string text = "CompositionGeometry";

    if ((renderFlags & RenderFlags::ShowDetails) != 0)
        text += " (Sub Geometries: " + ToStr(node->subGeometries.size()) + ")";

    PrintNode(node->metaData, nullptr, text, renderFlags);

    if (!node->subGeometries.empty())
    {
        IO::Log::ScopedIndent indent;
        for (auto& geometry : node->subGeometries)
            geometry->Visit(this);
    }
}

void LogSceneRenderer::VisitTexturedGeometry(TexturedGeometry* node)
{
    std::string text = "TexturedGeometry";

    if ((renderFlags & RenderFlags::ShowDetails) != 0)
        text += " (Textures: " + ToStr(node->textures.size()) + ")";

    PrintNode(node->metaData, nullptr, text, renderFlags);

    if (node->actualGeometry)
    {
        IO::Log::ScopedIndent indent;
        node->actualGeometry->Visit(this);
    }
}

/* --- Mesh geometries --- */

void LogSceneRenderer::VisitSimple2DMeshGeometry(Simple2DMeshGeometry* node)
{
    PrintBaseMeshGeometry(node->metaData, "Simple2DMeshGeometry", node->vertices.size(), node->indices.size());
}

void LogSceneRenderer::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    PrintBaseMeshGeometry(node->metaData, "Simple3DMeshGeometry", node->vertices.size(), node->indices.size());
}

void LogSceneRenderer::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    PrintBaseMeshGeometry(node->metaData, "TangentSpaceMeshGeometry", node->vertices.size(), node->indices.size());
}

void LogSceneRenderer::VisitCommonMeshGeometry(CommonMeshGeometry* node)
{
    PrintBaseMeshGeometry(node->metaData, "CommonMeshGeometry", node->vertices.size(), node->indices.size());
}

void LogSceneRenderer::VisitTerrainMeshGeometry(TerrainMeshGeometry* node)
{
    PrintBaseMeshGeometry(node->metaData, "TerrainMeshGeometry", node->vertices.size(), node->indices.size());
}


/*
 * ======= Private: =======
 */

void LogSceneRenderer::PrintBaseMeshGeometry(
    const NodeMetaData& metaData, std::string text, size_t numVertices, size_t numIndices)
{
    if ((renderFlags & RenderFlags::ShowDetails) != 0)
    {
        text += " (Vertices: ";
        text += ToStr(numVertices);
        text += ", Indices: ";
        text += ToStr(numIndices);
        text += ")";
    }
    PrintNode(metaData, nullptr, text, renderFlags);
}


} // /namespace Scene

} // /namespace Fork



// ========================