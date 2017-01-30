/*
 * Frustum header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FRUSTUM_H__
#define __FORK_FRUSTUM_H__


#include "Math/Geometry/ConvexHull.h"
#include "Math/Core/Matrix4.h"
#include "Math/Collision/PlaneCollisions.h"


namespace Fork
{

namespace Math
{


/**
Frustum plane indices. The index order of these planes is used for faster culling tests,
i.e. first test against the near plane and last test against the far plane.
\see Frustum
*/
enum class FrustumPlanes
{
    Near = 0,   //!< Near frustum plane.
    Left,       //!< Left frustum plane.
    Right,      //!< Right frustum plane.
    Top,        //!< Top frustum plane.
    Bottom,     //!< Bottom frustum plane.
    Far,        //!< Far frustum plane.
};


/**
The frustum class is a convex hull, consisting of 6 planes.
\see FrustumPlanes
*/
template <typename T = float> class Frustum : public ConvexHull<T, 6>
{
    
    public:
        
        Frustum() = default;
        explicit Frustum(const Matrix4<T>& matrix)
        {
            Build(matrix);
        }

        /* === Functions === */

        /**
        Builds the frustum by the specified 4x4 matrix.
        \param[in] matrix Specifies the matrix from which the frustum will be constructed.
        This must be a projection matrix, a view-projection or a world-view-projection matrix.
        The matrix must also be a left-handed coordinate system!
        */
        void Build(const Matrix4<T>& matrix)
        {
            /*
            Build planes with Plane{ x, y, z, d } constructor.
            Planes will be normalized automatically.
            The distance is negated because the plane equation is "x + y + z = d" and not "x + y + z + d = 0".
            */
            GetPlane(FrustumPlanes::Right) =
            {
                  matrix[ 3] - matrix[ 0],
                  matrix[ 7] - matrix[ 4],
                  matrix[11] - matrix[ 8],
                -(matrix[15] - matrix[12])
            };

            GetPlane(FrustumPlanes::Left) =
            {
                  matrix[ 3] + matrix[ 0],
                  matrix[ 7] + matrix[ 4],
                  matrix[11] + matrix[ 8],
                -(matrix[15] + matrix[12])
            };

            GetPlane(FrustumPlanes::Top) =
            {
                  matrix[ 3] - matrix[ 1],
                  matrix[ 7] - matrix[ 5],
                  matrix[11] - matrix[ 9],
                -(matrix[15] - matrix[13])
            };

            GetPlane(FrustumPlanes::Bottom) =
            {
                  matrix[ 3] + matrix[ 1],
                  matrix[ 7] + matrix[ 5],
                  matrix[11] + matrix[ 9],
                -(matrix[15] + matrix[13])
            };

            GetPlane(FrustumPlanes::Near) =
            {
                               matrix[ 2],
                               matrix[ 6],
                               matrix[10],
                              -matrix[14]
            };

            GetPlane(FrustumPlanes::Far) =
            {
                  matrix[ 3] - matrix[ 2],
                  matrix[ 7] - matrix[ 6],
                  matrix[11] - matrix[10],
                -(matrix[15] - matrix[14])
            };
        }

        //! Returns the left-top point of the frustum.
        inline Vector3<T> LeftTopPoint() const
        {
            Vector3<T> point;
            Math::CheckIntersectionWithPlane(GetPlane(FrustumPlanes::Far), GetPlane(FrustumPlanes::Top), GetPlane(FrustumPlanes::Left), point);
            return point;
        }
        //! Returns the left-bottom point of the frustum.
        inline Vector3<T> LeftBottomPoint() const
        {
            Vector3<T> point;
            Math::CheckIntersectionWithPlane(GetPlane(FrustumPlanes::Far), GetPlane(FrustumPlanes::Bottom), GetPlane(FrustumPlanes::Left), point);
            return point;
        }
        //! Returns the right-top point of the frustum.
        inline Vector3<T> RightTopPoint() const
        {
            Vector3<T> point;
            Math::CheckIntersectionWithPlane(GetPlane(FrustumPlanes::Far), GetPlane(FrustumPlanes::Top), GetPlane(FrustumPlanes::Right), point);
            return point;
        }
        //! Returns the right-buttom point of the frustum.
        inline Vector3<T> RightBotomPoint() const
        {
            Vector3<T> point;
            Math::CheckIntersectionWithPlane(GetPlane(FrustumPlanes::Far), GetPlane(FrustumPlanes::Bottom), GetPlane(FrustumPlanes::Right), point);
            return point;
        }

        //! Returns the specified frustum plane.
        inline const Plane<T>& GetPlane(const FrustumPlanes plane) const
        {
            return planes[static_cast<size_t>(plane)];
        }
        //! Returns the specified frustum plane.
        inline Plane<T>& GetPlane(const FrustumPlanes plane)
        {
            return planes[static_cast<size_t>(plane)];
        }

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================