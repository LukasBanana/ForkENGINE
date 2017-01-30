/*
 * Geometry auto UV map modifier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_AUTO_UV_MAP_MODIFIER_H__
#define __FORK_GEOMETRY_AUTO_UV_MAP_MODIFIER_H__


#include "GeometryModifier.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Matrix4.h"


namespace Fork
{

namespace Scene
{


//! Optimizes a geometry node graph by cleaning up redundant geometry nodes.
class FORK_EXPORT GeometryAutoUVMapModifier : public GeometryModifier
{
    
    public:
        
        //! Specifies the UV map offset. By default { 0, 0 }.
        Math::Vector2f uvMapOffset;

        //! Specifies the UV map scaling. By default { 1, 1 }.
        Math::Vector2f uvMapScale { 1, 1 };

        //! Specifies the UV map transformation. This can be set from a scene node's transformation.
        Math::Matrix4f uvMapTransform;

    private:
        
        void VisitSimple2DMeshGeometry      ( Simple2DMeshGeometry*     node ) override;
        void VisitSimple3DMeshGeometry      ( Simple3DMeshGeometry*     node ) override;
        void VisitTangentSpaceMeshGeometry  ( TangentSpaceMeshGeometry* node ) override;

        void GenerateTexCoord(Math::Point3f coord, const Math::Vector3f& normal, Math::Vector2f& texCoord);

        Math::Vector2f TexCoord(float u, float v) const;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================