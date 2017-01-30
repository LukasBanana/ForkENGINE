/*
 * Geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_H__
#define __FORK_GEOMETRY_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Core/TreeHierarchy/NodeMetaData.h"
#include "Scene/Node/SceneVisitor.h"
#include "Scene/Geometry/BoundingVolume.h"
#include "Scene/Geometry/Skeleton.h"
#include "Math/Geometry/AABB.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(Geometry);

//! Geometry node interface.
class FORK_EXPORT Geometry
{
    
    public:
        
        //! Geometry types.
        enum class Types
        {
            /* --- Internal base types --- */
            CommonMesh,         //!< Common mesh geometry. \see CommonMeshGeometry
            Simple3DMesh,       //!< Simple 3D mesh geometry. \see Simple3DMeshGeometry
            Simple2DMesh,       //!< Simple 2D mesh geometry. \see Simple2DMeshGeometry
            TangentSpaceMesh,   //!< Tangent space mesh geometry. \see TangentSpaceMeshGeometry
            Billboard,          //!< Billboard geometry. \see BillboardGeometry
            Terrain,            //!< Terrain geometry. \see TerrainGeometry
            TerrainMesh,        //!< Terrain mesh geometry. \see TerrainMeshGeometry

            /* --- Internal hierarchical types --- */
            Composition,        //!< Composition geometry. \see CompositionGeometry
            LOD,                //!< LOD (Level-of-detail) geometry. \see LODGeometry
            Textured,           //!< Textured geometry. \see TexturedGeometry

            /* --- Custom types --- */
            CustomA,            //!< Custom type, can be used for custom geometry class.
            CustomB,            //!< Custom type, can be used for custom geometry class.
            CustomC,            //!< Custom type, can be used for custom geometry class.
            CustomD,            //!< Custom type, can be used for custom geometry class.
            CustomE,            //!< Custom type, can be used for custom geometry class.
            CustomF,            //!< Custom type, can be used for custom geometry class.
        };

        virtual ~Geometry()
        {
        }

        Geometry(const Geometry&) = delete;
        Geometry& operator = (const Geometry&) = delete;

        /* === Functions === */

        //! Visit function for the scene visitor (Visitor Pattern).
        virtual void Visit(SceneVisitor* visitor)
        {
            /* Dummy */
        }

        /**
        Returns the geometry type.
        \see Types
        */
        virtual Types Type() const = 0;

        /**
        Computes the bounding AABB (axis aligned bounding box) and bounding sphere for this geometry.
        \see boundingVolume
        */
        virtual void ComputeBoundingVolume() = 0;

        /* === Members === */

        //! Geometry meta data.
        NodeMetaData                metaData;

        /**
        Bounding volume data model.
        \see BoundingVolume
        */
        BoundingVolume              boundingVolume;

        //! Geometry skeleton for skinning (also called "skeletal"- or "bone" animation).
        std::unique_ptr<Skeleton>   skeleton;

    protected:
        
        Geometry() = default;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================