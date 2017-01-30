/*
 * Scene manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_MANAGER_H__
#define __FORK_SCENE_MANAGER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Core/Container/SharedHashMap.h"
#include "Scene/Node/SceneNode.h"
#include "Scene/LightSource/LightSource.h"
#include "Scene/Geometry/Node/Geometry.h"
#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Scene/FileHandler/ModelReader.h"

#include <vector>
#include <string>


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(SceneManager);

//! Scene node base class.
class FORK_EXPORT SceneManager
{
    
    public:
        
        SceneManager() = default;
        virtual ~SceneManager();

        /* === Functions === */

        /* --- Geometry functions --- */

        /**
        Creates a geometry from the specified file.
        \param[in] filename Specifies the filename from which the geometry is to be created.
        \param[in] flags Optional geometry reading flags.
        This can be a bitwise combination of all "ModelReader::Flags" entries. By default 0.
        \see ModelReader::Flags
        */
        GeometryPtr CreateGeometryFromFile(const std::string& filename, const ModelReader::Flags::DataType flags = 0);
        /**
        Loads a geometry and creates it from file if not already done.
        \see CreateGeometryFromFile
        */
        GeometryPtr LoadGeometry(const std::string& filename, const ModelReader::Flags::DataType flags = 0);

        GeometryGenerator::GeometryTypePtr GenerateCube     (const GeometryGenerator::CubeDescription&      desc = GeometryGenerator::CubeDescription       ());
        GeometryGenerator::GeometryTypePtr GenerateWireCube (const GeometryGenerator::CubeDescription&      desc = GeometryGenerator::CubeDescription       ());
        GeometryGenerator::GeometryTypePtr GenerateCone     (const GeometryGenerator::ConeDescription&      desc = GeometryGenerator::ConeDescription       ());
        GeometryGenerator::GeometryTypePtr GenerateCylinder (const GeometryGenerator::CylinderDescription&  desc = GeometryGenerator::CylinderDescription   ());
        GeometryGenerator::GeometryTypePtr GenerateUVSphere (const GeometryGenerator::UVSphereDescription&  desc = GeometryGenerator::UVSphereDescription   ());
        GeometryGenerator::GeometryTypePtr GenerateIcoSphere(const GeometryGenerator::IcoSphereDescription& desc = GeometryGenerator::IcoSphereDescription  ());
        GeometryGenerator::GeometryTypePtr GenerateTorus    (const GeometryGenerator::TorusDescription&     desc = GeometryGenerator::TorusDescription      ());
        GeometryGenerator::GeometryTypePtr GenerateSpiral   (const GeometryGenerator::SpiralDescription&    desc = GeometryGenerator::SpiralDescription     ());
        GeometryGenerator::GeometryTypePtr GenerateCapsule  (const GeometryGenerator::CapsuleDescription&   desc = GeometryGenerator::CapsuleDescription    ());

        /* --- Scene node functions --- */

        GeometryNodePtr CreateGeometryNode();
        GeometryNodePtr CreateGeometryNode(const GeometryPtr& geometry);
        GeometryNodePtr CreateGeometryNodeFromFile(const std::string& filename);

        CameraNodePtr CreateCameraNode();
        
        LightNodePtr CreateLightNode();
        LightNodePtr CreateLightNode(const LightSourcePtr& lightSource);

        /* --- Common functions --- */

        void ReleaseSceneNode(const SceneNode* sceneNode);
        void ReleaseAllSceneNodes();

        /* === Members === */

        /**
        Path dictionary to find textures when a model is loaded.
        By default the model's file path is used only.
        \see CreateGeometryFromFile
        \see LoadGeometry
        */
        IO::PathDictionary texturePathDict;

    protected:
        
        /* === Structures === */

        template <class T> struct CompareGeometrySWO
        {
            bool operator () (const T& a, const T& b) const
            {
                return a.CompareSWO(b);
            }

            using first_argument_type   = T;    //!< std::map::key_compare interface member.
            using second_argument_type  = T;    //!< std::map::key_compare interface member.
            using result_type           = bool; //!< std::map::key_compare interface member.
        };

        template <class Key, class Compare = std::less<Key>>
        using SharedGeometryHashMap = SharedHashMap<Key, Geometry, Compare>;

        template <class Key>
        using SharedBasicGeometryHashMap = SharedHashMap<Key, GeometryGenerator::GeometryType, CompareGeometrySWO<Key>>;

        struct BasicGeometryContainer
        {
            SharedBasicGeometryHashMap< GeometryGenerator::CubeDescription      > cube;
            SharedBasicGeometryHashMap< GeometryGenerator::CubeDescription      > wireCube;
            SharedBasicGeometryHashMap< GeometryGenerator::ConeDescription      > cone;
            SharedBasicGeometryHashMap< GeometryGenerator::CylinderDescription  > cylinder;
            SharedBasicGeometryHashMap< GeometryGenerator::UVSphereDescription  > uvSphere;
            SharedBasicGeometryHashMap< GeometryGenerator::IcoSphereDescription > icoSphere;
            SharedBasicGeometryHashMap< GeometryGenerator::TorusDescription     > torus;
            SharedBasicGeometryHashMap< GeometryGenerator::SpiralDescription    > spiral;
            SharedBasicGeometryHashMap< GeometryGenerator::CapsuleDescription   > capsule;
        };

        /* === Functions === */

        template <class T> inline T AddSceneNode(const T& sceneNode)
        {
            sceneNodes.push_back(sceneNode);
            return sceneNode;
        }

        /* === Members === */

        SharedGeometryHashMap<std::string> geometries;

        BasicGeometryContainer basicGeometryContainer;

        std::vector<SceneNodePtr> sceneNodes;

    private:

        template <class GenProc, class Desc, class Container>
        GeometryGenerator::GeometryTypePtr GenerateBasicGeometry(GenProc genProc, const Desc& desc, Container& container);

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================