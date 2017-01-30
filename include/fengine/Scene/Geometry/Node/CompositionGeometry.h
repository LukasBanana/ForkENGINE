/*
 * Composition geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMPOSITION_GEOMETRY_H__
#define __FORK_COMPOSITION_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"

#include <vector>


namespace Fork
{

namespace Scene
{


/**
Composition geometry class. This class provides a list of sub geometries.
This can be used to render a single geometry node with several different geometries (and different textures).
\ingroup std_geometries
*/
class FORK_EXPORT CompositionGeometry : public Geometry
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Composition;

        CompositionGeometry() = default;

        CompositionGeometry(const CompositionGeometry&) = delete;
        CompositionGeometry& operator = (const CompositionGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Composition.
        Types Type() const override;

        //! Computes the maximal bounding volume of all sub-geometries.
        void ComputeBoundingVolume() override;

        /* === Members === */

        std::vector<GeometryPtr> subGeometries;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================