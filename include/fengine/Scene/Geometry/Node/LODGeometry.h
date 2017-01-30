/*
 * LOD geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LOD_GEOMETRY_H__
#define __FORK_LOD_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"

#include <vector>


namespace Fork
{

namespace Scene
{


/**
LOD (Level-of-Detail) geometry class.
\ingroup std_geometries
*/
class FORK_EXPORT LODGeometry : public Geometry
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::LOD;

        LODGeometry() = default;

        LODGeometry(const LODGeometry&) = delete;
        LODGeometry& operator = (const LODGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::LOD.
        Types Type() const override;

        //! Computes the maximal bounding volume of all sub-geometries.
        void ComputeBoundingVolume() override;

        /**
        Selects a specific level-of-detail geometry by the given distance.
        \param[in] distance Specifies the distance from which the level-of-detail geometry is to be selected.
        \param[in] minDistance Specifies the minimal distance.
        If (distance <= minDistance) the first geometry will be selected. Must be smaller than 'maxDistance'.
        \param[in] maxDistance Specifies the maximal distance.
        If (distance >= maxDistance) the last geometry will be selected. Must be greater than 'minDistance'.
        \return Raw-pointer to the selected goemetry or null if the geometry list (lodGeometries) is empty.
        \see SetThreshold
        \see lodGeometries
        */
        virtual Geometry* SelectLOD(float distance) const;

        /**
        Sets up the minimal and maximal distances.
        \param[in] minDistance Specifies the minimal distance in which applies: 0 <= minDistance < maxDistance.
        \param[in] maxDistance Specifies the maximal distance in which applies. 0 <= minDistance < maxDistance.
        */
        void SetupRange(float minDistance, float maxDistance);

        /**
        Sets the LOD-selection threshold exponent.
        \param[in] threshold Specifies the exponent for the linear-to-non-linear transformation.
        The distance will be transformed from [min .. max] to [0 .. 1] first. Then a transformation from
        linear to non-linear will be performed. This exponentis used for the calculation: y = x^threshold.
        This should be a value smaller 1.0 to provide frequently selection switches near the 'minDistance'.
        If this value is exactly 1.0, the selection between the level-of-details will be linear.
        The input value is clamped to (0 .. +inf). By default 0.4.
        */
        void SetupThreshold(float threshold);

        //! Returns the threshold exponent. By default 0.4.
        inline float GetThreshold() const
        {
            return threshold_;
        }

        //! Returns the minimal distance. By default 10.
        inline float GetMinDistance() const
        {
            return minDistance_;
        }
        //! Returns the maximal distance. By default 250.
        inline float GetMaxDistance() const
        {
            return maxDistance_;
        }

        /* === Members === */

        /**
        Level-of-detail geometry list. The first entry (lodGeometries.front()) represents the
        highest level-of-detail and the last entry (lodGeometries.back()) represents the lowest level-of-detail.
        */
        std::vector<GeometryPtr> lodGeometries;

    private:
        
        float minDistance_  = 10.0f;    //!< Minimal distance.
        float maxDistance_  = 250.0f;   //!< Maximal distance.

        float threshold_    = 0.4f;     //!< Threshold to select a specific level-of-detail geometry by a given distance.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================