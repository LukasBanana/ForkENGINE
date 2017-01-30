/*
 * Common model reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommonModelReader.h"
#include "IO/Core/Log.h"
#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Geometry/Node/TangentSpaceMeshGeometry.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Core/StringModifier.h"
#include "Platform/Core/FilePath.h"

#ifdef FORK_IMPORT_ASSIMP
#   include <assimp/Importer.hpp>
#   include <assimp/postprocess.h>
#endif


namespace Fork
{

namespace Scene
{


#ifndef FORK_IMPORT_ASSIMP

GeometryPtr CommonModelReader::ReadModel(const std::string& filename)
{
    IO::Log::Error("Engine was not compiled with the \"Assimp\" module");
    return nullptr;
}

#else

/*
 * Internal functions
 */

static inline Math::Vector2f Vec2(const aiVector3D& vec)
{
    return { vec.x, vec.y };
}

static inline Math::Vector3f Vec3(const aiVector3D& vec)
{
    return { vec.x, vec.y, vec.z };
}

static inline std::string AIStr(const aiString& str)
{
    return std::string(str.C_Str());
}

template <typename T> typename std::map<std::string, T>::iterator
FindBone(std::map<std::string, T>& list, const aiString& boneName)
{
    auto name = AIStr(boneName);
    auto it = list.find(name);
    if (it == list.end())
        IO::Log::Error("Undefined skeleton joint \"" + name + "\"");
    return it;
}


/*
 * CommonModelReader class
 */

GeometryPtr CommonModelReader::ReadModel(
    const std::string& filename, const IO::PathDictionary& texPathDict, const Flags::DataType flags)
{
    return ReadModelExt(filename, texPathDict, flags, /*false*/true).model; //!!!
}

ModelReader::AnimatedModel CommonModelReader::ReadAnimatedModel(
    const std::string& filename, const IO::PathDictionary& texPathDict, const Flags::DataType flags)
{
    return ReadModelExt(filename, texPathDict, flags, true);
}


/*
 * ======= Private: =======
 */

ModelReader::AnimatedModel CommonModelReader::ReadModelExt(
    const std::string& filename, const IO::PathDictionary& texPathDict,
    const Flags::DataType flags, bool loadAnimation)
{
    AnimatedModel animModel;

    /* Setup reading flags */
    createTangentSpaceMeshes_ = ((flags & Flags::GenerateTangentSpace) != 0);

    /* Setup import flags */
    unsigned int importFlags =
        aiProcess_Triangulate           |
        aiProcess_GenSmoothNormals      |
      //aiProcess_JoinIdenticalVertices |
        aiProcess_FlipWindingOrder      |
        aiProcess_ConvertToLeftHanded   |
        aiProcess_SortByPType;

    if (createTangentSpaceMeshes_)
        importFlags |= aiProcess_CalcTangentSpace;

    /* Import entire scene with assimp */
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, importFlags);

    /* Check for errors */
    if (!scene)
    {
        IO::Log::Error(importer.GetErrorString());
        return animModel;
    }

    /* Store file path to load textures from this directory */
    texPathDict_ = texPathDict;
    texPathDict_.AddSearchPath(ExtractFilePath(filename));

    /* Analyze imported scene */
    try
    {
        /* Create skeleton */
        if (loadAnimation)
            skeleton_ = std::make_unique<Skeleton>();

        /* Create scene geometry */
        animModel.model = VisitScene(scene, loadAnimation);
        importer.FreeScene();

        /* Setup skeleton */
        if (loadAnimation)
            animModel.model->skeleton = std::move(skeleton_);
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    return animModel;
}

static Video::TexturePtr LoadTextureFromFile(const std::string& filename)
{
    return Video::RenderSystem::Active()->GetTextureManager()->LoadTexture2D(filename);
}

Video::TexturePtr CommonModelReader::LoadTexture(std::string filename) const
{
    if (!texPathDict_.FindFile(filename))
    {
        IO::Log::Error("Searching texture \"" + filename + "\" failed");
        return nullptr;
    }
    return LoadTextureFromFile(filename);
}

#ifdef _DEB_ASSIMP_
void CommonModelReader::_DebNode(const aiNode* node)
{
    if (node)
    {
        auto it = FindBone(bones_, node->mName);

        IO::Log::Debug("ASSIMP", "Node Name: \"" + AIStr(node->mName) + "\", Necessary: " + ToStr(it->second.necessary ? "yes" : "no"));
        IO::Log::ScopedIndent indent;

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            _DebNode(node->mChildren[i]);
    }
}
#endif

GeometryPtr CommonModelReader::VisitScene(const aiScene* scene, bool loadAnimation)
{
    /* Visit scene materials */
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
        VisitMaterial(scene->mMaterials[i]);

    if (loadAnimation)
    {
        /* Visit scene nodes */
        VisitNode(scene->mRootNode);

        /* Mark necessary bones */
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
            MarkNecessaryMeshBones(scene->mMeshes[i]);

        #ifdef _DEB_ASSIMP_
        _DebNode(scene->mRootNode);
        #endif

        /* Visit animations */
        if (scene->mNumAnimations > 0 && scene->mAnimations)
        {
            for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
                VisitAnimation(scene->mAnimations[i]);
        }
    }

    if (scene->mNumMeshes == 1)
    {
        /* Only create a single geometry */
        return VisitMesh(scene->mMeshes[0], loadAnimation);
    }
    
    /* Create a composition geometry */
    auto geometry = std::make_shared<CompositionGeometry>();

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        /* Add sub geometry to composition geometry */
        auto subGeometry = VisitMesh(scene->mMeshes[i], loadAnimation);
        geometry->subGeometries.push_back(subGeometry);
    }

    return geometry;
}

void CommonModelReader::VisitMaterial(const aiMaterial* material)
{
    Material modelMaterial;

    /* Load material textures */
    auto LoadTextureType = [&](const aiTextureType type, unsigned int index)
    {
        aiString filename;
        if (material->GetTexture(type, index, &filename) == AI_SUCCESS)
            modelMaterial.AddTexture(LoadTexture(AIStr(filename)));
    };

    auto LoadAllTextures = [&](const aiTextureType type)
    {
        for (unsigned int i = 0, n = material->GetTextureCount(type); i < n; ++i)
            LoadTextureType(type, i);
    };

    for (int type : { aiTextureType_DIFFUSE, aiTextureType_NORMALS, aiTextureType_HEIGHT, aiTextureType_SPECULAR })
        LoadAllTextures(static_cast<aiTextureType>(type));

    materials_.push_back(modelMaterial);
}

void CommonModelReader::VisitNode(const aiNode* node)
{
    if (node)
    {
        /* Store reference to node in hash-map with its name */
        bones_[AIStr(node->mName)] = { node, false };

        /* Visit child nodes */
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            VisitNode(node->mChildren[i]);
    }
}

void CommonModelReader::VisitAnimation(const aiAnimation* anim)
{
    /* Visit node ("bone" or "joint") animation channels */
    for (unsigned int i = 0; i < anim->mNumChannels; ++i)
        VisitNodeAnim(anim->mChannels[i]);
}

void CommonModelReader::VisitNodeAnim(const aiNodeAnim* anim)
{
    /* Store reference to node animation in hash-map with its name */
    jointAnimations_[AIStr(anim->mNodeName)] = anim;
}

GeometryPtr CommonModelReader::VisitMesh(const aiMesh* mesh, bool loadAnimation)
{
    /* Create mesh geometry */
    GeometryPtr meshGeometry;
    if (createTangentSpaceMeshes_)
        meshGeometry = CreateTangentSpaceMeshGeometry(mesh);
    else
        meshGeometry = CreateSimple3DMeshGeometry(mesh);

    /* Check if mesh has material */
    if (mesh->mMaterialIndex < materials_.size())
    {
        auto& material = materials_[mesh->mMaterialIndex];

        if (!material.textures.empty())
        {
            /* Create textured geometry */
            auto texturedGeometry = std::make_shared<TexturedGeometry>();
            texturedGeometry->actualGeometry = meshGeometry;

            /* Append textures to the geoemtry */
            texturedGeometry->textures = material.textures;

            return texturedGeometry;
        }
    }

    /* Create skeleton joints */
    if (loadAnimation && mesh->mBones && mesh->mNumBones > 0)
    {
        for (unsigned int i = 0; i < mesh->mNumBones; ++i)
        {
            /* Find node animation for this bone */
            auto bone = mesh->mBones[i];
            auto it = FindBone(jointAnimations_, bone->mName);

            if (it != jointAnimations_.end())
            {
                // !!! to be continued...
            }
        }
    }

    return meshGeometry;
}

void CommonModelReader::MarkNecessaryMeshBones(const aiMesh* mesh)
{
    const auto meshName = AIStr(mesh->mName);

    for (unsigned int i = 0; i < mesh->mNumBones; ++i)
    {
        /* Find node in hierarchy for this bone */
        auto bone = mesh->mBones[i];
        MarkNecessaryBone(bone->mName, meshName);
    }
}

void CommonModelReader::MarkNecessaryBone(const aiString& boneName, const std::string& meshName)
{
    /* Find input bone in the hierarchy */
    auto it = FindBone(bones_, boneName);
    if (it == bones_.end())
        return;

    auto& bone = it->second;
    auto node = bone.node;

    /* Check if we have no parent of we found the mesh's node */
    if (!node || AIStr(node->mName) == meshName)
        return;

    /* Mark this bone 'as necessary' */
    bone.necessary = true;

    if (node->mParent)
    {
        /* Repeat for its parents until we found the mesh's node */
        MarkNecessaryBone(node->mParent->mName, meshName);
    }
}

Simple3DMeshGeometryPtr CommonModelReader::CreateSimple3DMeshGeometry(const aiMesh* mesh)
{
    /* Create geometry object */
    auto geometry = std::make_shared<Simple3DMeshGeometry>();

    /* Create vertices */
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        geometry->AddVertex(
            Vec3(mesh->mVertices[i]),
            mesh->mNormals != nullptr ? Vec3(mesh->mNormals[i]) : Math::Vector3f(),
            mesh->mTextureCoords[0] != nullptr ? Vec2(mesh->mTextureCoords[0][i]) : Math::Vector2f()
        );
    }

    /* Create indices */
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const auto& face = FetchTriangle(mesh, i);
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            geometry->AddIndex(face.mIndices[j]);
    }

    /* Setup final geometry data */
    geometry->SetupHardwareBuffer();

    return geometry;
}

TangentSpaceMeshGeometryPtr CommonModelReader::CreateTangentSpaceMeshGeometry(const aiMesh* mesh)
{
    /* Create geometry object */
    auto geometry = std::make_shared<TangentSpaceMeshGeometry>();

    /* Create vertices */
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        geometry->AddVertex(
            Vec3(mesh->mVertices[i]),
            mesh->mNormals          != nullptr ? Vec3(mesh->mNormals         [i]) : Math::Vector3f(),
            mesh->mTextureCoords[0] != nullptr ? Vec2(mesh->mTextureCoords[0][i]) : Math::Vector2f(),
            mesh->mTangents         != nullptr ? Vec3(mesh->mTangents        [i]) : Math::Vector3f(),
            mesh->mBitangents       != nullptr ? Vec3(mesh->mBitangents      [i]) : Math::Vector3f()
        );
    }

    /* Create indices */
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const auto& face = FetchTriangle(mesh, i);
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            geometry->AddIndex(face.mIndices[j]);
    }

    /* Setup final geometry data */
    geometry->SetupHardwareBuffer();
    //geometry->GenerateTangentSpaceFlat(true);//!TEST!

    return geometry;
}

const aiFace& CommonModelReader::FetchTriangle(const aiMesh* mesh, unsigned int index) const
{
    const aiFace& face = mesh->mFaces[index];
    if (face.mNumIndices != 3)
        throw std::runtime_error("Wrong number of indices in face (Queried 'Assimp' to triangulate mesh)");
    return face;
}

#endif


} // /namespace Scene

} // /namespace Fork



// ========================