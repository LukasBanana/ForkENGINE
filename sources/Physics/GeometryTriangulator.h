/*
 * Physics geometry triangulator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_GEOMETRY_TRIANGULATOR_H__
#define __FORK_PHYSICS_GEOMETRY_TRIANGULATOR_H__


#include "Scene/Node/DefaultSceneVisitor.h"
#include "Math/Geometry/Triangle.h"

#include <vector>


namespace Fork
{

namespace Physics
{


//! Triangles a given geometry hierarchy.
class GeometryTriangulator final : private Scene::DefaultSceneVisitor
{
    
    public:
        
        GeometryTriangulator() = default;

        GeometryTriangulator(const GeometryTriangulator&) = delete;
        GeometryTriangulator& operator = (const GeometryTriangulator&) = delete;

        /**
        Triangles the entire geometry hierarchy.
        \param[in] geometry Specifies the root node of the geometry hierarchy which is to be triangulated.
        \param[out] triangles Specifies the resulting triangle list.
        */
        void Triangulate(const Scene::Geometry& geometry, std::vector<Math::Triangle3f>& triangles);

    private:
        
        void VisitSimple3DMeshGeometry      ( Scene::Simple3DMeshGeometry*     node ) override;
        void VisitTangentSpaceMeshGeometry  ( Scene::TangentSpaceMeshGeometry* node ) override;
        void VisitCommonMeshGeometry        ( Scene::CommonMeshGeometry*       node ) override;

        std::vector<Math::Triangle3f>* triangles_ = nullptr;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================