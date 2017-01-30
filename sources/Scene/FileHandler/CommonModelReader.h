/*
 * Common model reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMMON_MODEL_READER_H__
#define __FORK_COMMON_MODEL_READER_H__


#include "Scene/FileHandler/ModelReader.h"
#include "Core/StaticConfig.h"
#include "Video/RenderSystem/Texture/Texture.h"

#ifdef FORK_IMPORT_ASSIMP
#   include <assimp/scene.h>
#endif


namespace Fork
{

namespace Scene
{


//#define _DEB_ASSIMP_//!!!

class CommonModelReader : public ModelReader
{
    
    public:
        
        GeometryPtr ReadModel(
            const std::string& filename,
            const IO::PathDictionary& texPathDict = {},
            const Flags::DataType flags = 0
        ) override;

        AnimatedModel ReadAnimatedModel(
            const std::string& filename,
            const IO::PathDictionary& texPathDict = {},
            const Flags::DataType flags = 0
        ) override;

    private:
        
        #ifdef FORK_IMPORT_ASSIMP

        /* === Structures === */

        struct Material
        {
            void AddTexture(const Video::TexturePtr& texture)
            {
                if (texture)
                    textures.push_back(texture);
            }
            std::vector<Video::TexturePtr> textures;
        };

        struct Bone
        {
            const aiNode*   node;
            bool            necessary;
        };

        /* === Functions === */

        AnimatedModel ReadModelExt(
            const std::string& filename,
            const IO::PathDictionary& texPathDict,
            const Flags::DataType flags,
            bool loadAnimation
        );

        Video::TexturePtr LoadTexture(std::string filename) const;

        GeometryPtr VisitScene(const aiScene* scene, bool loadAnimation);
        void VisitMaterial(const aiMaterial* material);
        void VisitNode(const aiNode* node);
        void VisitAnimation(const aiAnimation* anim);
        void VisitNodeAnim(const aiNodeAnim* anim);
        GeometryPtr VisitMesh(const aiMesh* mesh, bool loadAnimation);

        void MarkNecessaryMeshBones(const aiMesh* mesh);
        void MarkNecessaryBone(const aiString& boneName, const std::string& meshName);

        Simple3DMeshGeometryPtr CreateSimple3DMeshGeometry(const aiMesh* mesh);
        TangentSpaceMeshGeometryPtr CreateTangentSpaceMeshGeometry(const aiMesh* mesh);

        const aiFace& FetchTriangle(const aiMesh* mesh, unsigned int index) const;

        #ifdef _DEB_ASSIMP_
        void _DebNode(const aiNode* node);
        #endif

        /* === Members === */

        std::vector<Material>                       materials_;
        IO::PathDictionary                          texPathDict_;
        bool                                        createTangentSpaceMeshes_ = false;
        
        std::unique_ptr<Skeleton>                   skeleton_;
        std::map<std::string, const aiNodeAnim*>    jointAnimations_;
        std::map<std::string, Bone>                 bones_;

        #endif

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================