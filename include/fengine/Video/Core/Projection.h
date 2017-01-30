/*
 * Projection header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PROJECTION_H__
#define __FORK_PROJECTION_H__


#include "Video/Core/Viewport.h"
#include "Math/Core/Matrix4.h"
#include "Math/Core/Equal.h"
#include "Math/Core/MathConstants.h"
#include "Math/Geometry/Ray.h"

#include <cmath>


namespace Fork
{

namespace Video
{


/**
3D projection class. Contains a viewport, information about the near and far clipping plane distances,
field-of-view (FOV) and pre-projected left-handed (LH)- and right-handed (RH) 4x4 projection matrices.
\see Video::Viewport
*/
template <typename T = float> class Projection
{
    
    public:
        
        Projection() :
            fieldOfView_{ Math::DefaultFOVrad<T>() }
        {
        }
        Projection(const Viewport& viewport) :
            viewport_   { viewport                 },
            fieldOfView_{ Math::DefaultFOVrad<T>() }
        {
        }
        Projection(const Viewport& viewport, const T& nearPlane, const T& farPlane, const T& fieldOfView = T(Math::defaultFOVrad64), bool isOrtho = false) :
            viewport_   { viewport    },
            nearPlane_  { nearPlane   },
            farPlane_   { farPlane    },
            fieldOfView_{ fieldOfView },
            isOrtho_    { isOrtho     },
            changedLH_  { true        },
            changedRH_  { true        },
        {
        }

        /* === Static functions === */

        /**
        Builds a perspective, left-handed projection matrix.
        \param[out] projectionMatrix Specifies the resulting projection matrix.
        \param[in] aspect Specifies the aspect ratio. This is commonly: aspect = width/height.
        \param[in] near Specifies the distance of the near clipping plane.
        \param[in] far Specifies the distance of the far clipping plane.
        \param[in] fieldOfView Specifies the field-of-view (FOV) angle (in radians). This is the vertical opening angle of the view.
        \return True if the projection matrix could be created successful. Otherwise "near" and "far" are almost equal.
        \remarks With this projection matrix the Z values can be transformed to
        normalized device coordinates (NDC) in the range [0.0 .. 1.0], like it's used in Direct3D.
        */
        static bool BuildPerspectiveProjectionLH(Math::Matrix4<T>& projectionMatrix, const T& aspect, const T& near, const T& far, const T& fieldOfView)
        {
            if (Math::Equal<T, T>(near, far))
                return false;

            const T h = T(1) / std::tan(fieldOfView / T(2));
            const T w = h / aspect;
            
            projectionMatrix = Math::Matrix4<T>(
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, far/(far - near), -(far*near)/(far - near),
                0, 0, 1, 0
            );

            return true;
        }

        /**
        Builds a perspective, right-handed projection matrix.
        \see BuildPerspectiveProjectionLH
        \remarks With this projection matrix the Z values can be transformed to
        normalized device coordinates (NDC) in the range [-1.0 .. 1.0], like it's used in OpenGL.
        */
        static bool BuildPerspectiveProjectionRH(Math::Matrix4<T>& projectionMatrix, const T& aspect, const T& near, const T& far, const T& fieldOfView)
        {
            if (Math::Equal<T, T>(near, far))
                return false;

            const T h = T(1) / std::tan(fieldOfView / T(2));
            const T w = h / aspect;
            
            projectionMatrix = Math::Matrix4<T>(
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, -(far + near)/(far - near), -(T(2)*far*near)/(far - near),
                0, 0, -1, 0
            );

            return true;
        }

        /**
        Builds an orthogonal, left-handed projection matrix.
        \see BuildPerspectiveProjectionLH
        \remarks With this projection matrix the Z values can be transformed to
        normalized device coordinates (NDC) in the range [0.0 .. 1.0], like it's used in Direct3D.
        */
        static bool BuildOrthogonalProjectionLH(Math::Matrix4<T>& projectionMatrix, const T& width, const T& height, const T& near, const T& far)
        {
            if (Math::Equal<T, T>(near, far))
                return false;

            projectionMatrix = Math::Matrix4<T>(
                T(2)/width, 0, 0, 0,
                0, T(2)/height, 0, 0,
                0, 0, T(1)/(far - near), -near/(far - near),
                0, 0, 0, 1
            );

            return true;
        }

        /**
        Builds an orthogonal, right-handed projection matrix.
        \see BuildPerspectiveProjectionLH
        \remarks With this projection matrix the Z values can be transformed to
        normalized device coordinates (NDC) in the range [-1.0 .. 1.0], like is used in OpenGL.
        */
        static bool BuildOrthogonalProjectionRH(Math::Matrix4<T>& projectionMatrix, const T& width, const T& height, const T& near, const T& far)
        {
            if (Math::Equal<T, T>(near, far))
                return false;

            projectionMatrix = Math::Matrix4<T>(
                T(2)/width, 0, 0, 0,
                0, T(2)/height, 0, 0,
                0, 0, -T(2)/(far - near), -(far + near)/(far - near),
                0, 0, 0, 1
            );

            return true;
        }

        /**
        Builds a 2D projection matrix with origin on the left-top (LT).
        This is typical for MS/Windows drawing operations.
        \param[out] projectionMatrix Specifies the resulting 2D projection matrix.
        \param[in] size Specifis the size (or rather 'resolution') of the 2D grid.
        */
        static void Build2DProjectionLT(Math::Matrix4<T>& projectionMatrix, const Math::Size2<T>& size)
        {
            projectionMatrix = Math::Matrix4<T>(
                T(2)/size.width, 0, 0, -1,
                0, T(-2)/size.height, 0, 1,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        /**
        Builds a 2D projection matrix with origin on the left-bottom (LB).
        A left-bottom projection is the default for 3D graphics hardware.
        \param[out] projectionMatrix Specifies the resulting 2D projection matrix.
        \param[in] size Specifis the size (or rather 'resolution') of the 2D grid.
        */
        static void Build2DProjectionLB(Math::Matrix4<T>& projectionMatrix, const Math::Size2<T>& size)
        {
            projectionMatrix = Math::Matrix4<T>(
                T(2)/size.width, 0, 0, -1,
                0, T(2)/size.height, 0, -1,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        /**
        Extracts the near- and far clipping planes from the specified left-handed projection matrix.
        \param[in] projectionMatrix Specifies the left-handed projection matrix whose clipping planes are to be extracted.
        \param[out] nearPlanes Specifies the near clipping plane.
        \param[out] farPlanes Specifies the far clipping plane.
        */
        static void ExtractClippingPlanesLH(const Math::Matrix4<T>& projectionMatrix, T& nearPlane, T& farPlane)
        {
            /*
            -> Multiply the unit cube Z values (of DirectX) with the inverse
            -> projection matrix, to extract the frustum bounding box Z values.
               The ranges here of the normalized device coordinates (NDC) Z are [0.0 .. 1.0].
            */
            auto invProjection = projectionMatrix.Inverse();

            auto nearVec = invProjection * Math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
            nearPlane = nearVec.z / nearVec.w;

            auto farVec = invProjection * Math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
            farPlane = farVec.z / farVec.w;
        }

        /**
        Extracts the near- and far clipping planes from the specified left-handed projection matrix.
        \param[in] projectionMatrix Specifies the left-handed projection matrix whose clipping planes are to be extracted.
        \param[out] nearPlanes Specifies the near clipping plane.
        \param[out] farPlanes Specifies the far clipping plane.
        */
        static void ExtractClippingPlanesRH(const Math::Matrix4<T>& projectionMatrix, T& nearPlane, T& farPlane)
        {
            /*
            -> Multiply the unit cube Z values (of OpenGL) with the inverse
               projection matrix, to extract the frustum bounding box Z values.
            -> The ranges here of the normalized device coordinates (NDC) Z are [-1.0 .. 1.0].
            -> Also negate the planes since the OpenGL projections are fliped on the Z axis.
            */
            auto invProjection = projectionMatrix.Inverse();

            auto nearVec = invProjection * Math::Vector4f(0.0f, 0.0f, -1.0f, 1.0f);
            nearPlane = nearVec.z / -nearVec.w;

            auto farVec = invProjection * Math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
            farPlane = farVec.z / -farVec.w;
        }

        /**
        Projects the specified point from view-space into clipping-space.
        \param[in] projectionMatrix Specifies the projection matrix. This can be a left-handed or right-handed coordinate system.
        \param[in] pointVS Specifies the point in view-space.
        \return Projected point in clipping-space.
        \remarks The clipping-space depends on the specified projection matrix.
        */
        static Math::Point4<T> ProjectPoint(const Math::Matrix4<T>& projectionMatrix, Math::Point4<T> pointVS)
        {
            pointVS = projectionMatrix * pointVS;
            pointVS /= pointVS.w;
            return pointVS;
        }

        /**
        Unprojects the specified point from clipping-space into view-space.
        \param[in] projectionMatrix Specifies the projection matrix. This can be a left-handed or right-handed coordinate system.
        \param[in] pointCS Specifies the point in clipping-space.
        \return Unprojected point in view-space.
        \remarks The clipping-space depends on the specified projection matrix.
        */
        static Math::Point4<T> UnprojectPoint(const Math::Matrix4<T>& projectionMatrix, Math::Point4<T> pointCS)
        {
            pointCS = projectionMatrix.Inverse() * pointCS;
            pointCS /= pointCS.w;
            return pointCS;
        }

        /**
        Projects the specified point from view-space into screen-space.
        \param[in] projectionMatrix Specifies the projection matrix. This can be a left-handed or right-handed coordinate system.
        \param[in] pointVS Specifies the point in view-space.
        \param[in] viewport Specifies the viewport for screen-space transformation.
        \param[in] isCSUnitCube Specifies whether the clipping-space is a unit cube or not.
        This must be 'true' for a right-handed projection matrix (in OpenGL) and it must be 'false' for a left-handed projection matrix (in Direct3D).
        \return Projected point in screen-space.
        */
        static Math::Point4<T> ProjectPoint(
            const Math::Matrix4<T>& projectionMatrix, Math::Point4<T> pointVS, const Viewport& viewport, bool isCSUnitCube)
        {
            /* Project point */
            pointVS = projectionMatrix * pointVS;
            pointVS /= pointVS.w;

            /* Transform into screen-space with viewport */
            pointVS.x = (pointVS.x*T( 0.5) + T(0.5)) * viewport.size.width  + viewport.position.x;
            pointVS.y = (pointVS.y*T(-0.5) + T(0.5)) * viewport.size.height + viewport.position.y;

            if (isNDCUnitCube)
                pointVS.z = pointVS.z*T(0.5) + T(0.5);

            pointVS.z = pointVS.z * static_cast<T>(viewport.maxDepth - viewport.minDepth) + static_cast<T>(viewport.minDepth);

            return pointVS;
        }

        /**
        Unprojects the specified point from screen-space into view-space.
        \param[in] projectionMatrix Specifies the projection matrix. This can be a left-handed or right-handed coordinate system.
        \param[in] pointSS Specifies the point in screen-space.
        \param[in] viewport Specifies the viewport for screen-space transformation.
        \param[in] isCSUnitCube Specifies whether the clipping-space is a unit cube or not.
        This must be 'true' for a right-handed projection matrix (in OpenGL) and it must be 'false' for a left-handed projection matrix (in Direct3D).
        \return Unprojected point in view-space.
        */
        static Math::Point4<T> UnprojectPoint(
            const Math::Matrix4<T>& projectionMatrix, Math::Point4<T> pointSS, const Viewport& viewport, bool isCSUnitCube)
        {
            /* Transform into clipping-space with viewport */
            pointSS.x = ((pointSS.x - viewport.position.x) / static_cast<T>(viewport.size.width )) * T( 2.0) - T(1.0);
            pointSS.y = ((pointSS.y - viewport.position.y) / static_cast<T>(viewport.size.height)) * T(-2.0) - T(1.0);
            pointSS.z = (pointSS.z - static_cast<T>(viewport.minDepth)) / static_cast<T>(viewport.maxDepth - viewport.minDepth);

            if (isNDCUnitCube)
                pointSS.z = pointSS.z*T(2.0) - T(1.0);

            /* Unproject point */
            pointSS = projectionMatrix.Inverse() * pointCS;
            pointSS /= pointSS.w;

            return pointSS;
        }

        /* === Functions === */

        /**
        Returns the left-handed (LH) 4x4 projection matrix. This function has the interface of a getter,
        but internally it works differently. This is due to efficient matrix calculations and easy access.
        \note If you switch continuously between "GetMatrixLH" and "GetMatrixRH", the matrices will be
        computed again every time. So try to use only alternative to avoid a performance hit.
        */
        const Math::Matrix4<T>& GetMatrixLH() const
        {
            if (changedLH_)
            {
                if (isOrtho_)
                {
                    Projection<T>::BuildOrthogonalProjectionLH(
                        matrixLH_, orthoSize_.width * viewport_.AspectRatio<T>(), orthoSize_.height, nearPlane_, farPlane_
                    );
                }
                else
                {
                    Projection<T>::BuildPerspectiveProjectionLH(
                        matrixLH_, viewport_.AspectRatio<T>(), nearPlane_, farPlane_, fieldOfView_
                    );
                }
                changedLH_ = false;
            }
            return matrixLH_;
        }

        /**
        This is the counterpart of "GetMatrixLH" but it returns a right-handed (RH) 4x4 projection matrix.
        \see GetMatrixLH
        */
        const Math::Matrix4<T>& GetMatrixRH() const
        {
            if (changedRH_)
            {
                if (isOrtho_)
                {
                    Projection<T>::BuildOrthogonalProjectionRH(
                        matrixRH_, orthoSize_.width * viewport_.AspectRatio<T>(), orthoSize_.height, nearPlane_, farPlane_
                    );
                }
                else
                {
                    Projection<T>::BuildPerspectiveProjectionRH(
                        matrixRH_, viewport_.AspectRatio<T>(), nearPlane_, farPlane_, fieldOfView_
                    );
                }
                changedRH_ = false;
            }
            return matrixRH_;
        }

        /**
        Interpolates between the two left-handed projection matrices.
        \see Math::Lerp
        \see GetMatrixLH
        */
        void InterpolateLH(const Projection<T>& from, const Projection<T>& to, const T& t)
        {
            Math::Lerp(matrixLH_, from.GetMatrixLH(), to.GetMatrixLH(), t);
            changedLH_ = false;
        }
        /**
        Interpolates between the two right-handed projections matrices.
        \see Math::Lerp
        \see GetMatrixRH
        */
        void InterpolateRH(const Projection<T>& from, const Projection<T>& to, const T& t)
        {
            Math::Lerp(matrixRH_, from.GetMatrixRH(), to.GetMatrixRH(), t);
            changedRH_ = false;
        }

        /**
        Interpolates between the two right-handed projections matrices.
        \see InterpolateLH
        \see InterpolateRH
        */
        void Interpolate(const Projection<T>& from, const Projection<T>& to, const T& t)
        {
            InterpolateLH(from, to, t);
            InterpolateRH(from, to, t);
        }

        /**
        Transforms the input point with this projection (including the viewport).
        \param[in,out] point Specifies the point (in view-space) which is to be projected.
        This point must already be transformed by the world and view matrix.
        If the point is outside the view area, the result will be { 0, 0, 0, 0 }.
        \return True if the point could be projected. Otherwise the point is outside the view area.
        */
        bool ProjectPoint(Math::Point4<T>& point) const
        {
            /* Transform point with projection matrix */
            point = GetMatrixLH() * point;

            if (point.w <= T(0))
            {
                point = { 0, 0, 0, 0 };
                return false;
            }

            /* Apply RHW (Reciprocal Homogeneous W) coordinate */
            point.x /= point.w;
            point.y /= point.w;
            point.z /= point.w;

            /* Transform point to viewport */
            const auto viewportOrigin   = GetViewport().position.Cast<T>();
            const auto viewportSize     = GetViewport().size.Cast<T>();

            point.x =  point.x * viewportSize.width  * T(0.5) + (viewportOrigin.x + viewportSize.width  * T(0.5));
            point.y = -point.y * viewportSize.height * T(0.5) + (viewportOrigin.y + viewportSize.height * T(0.5));
            point.z =  point.z * (GetFarPlane() - GetNearPlane())*T(0.5) + (GetFarPlane() + GetNearPlane())*T(0.5);

            return true;
        }

        /**
        Comfort function for "ProjectPoint". 
        \see ProjectPoint
        */
        inline Math::Point4<T> ComputeProjectedPoint(Math::Point4<T> point) const
        {
            ProjectPoint(point)
            return point;
        }

        /**
        Builds a view ray for the specified point.
        \param[in] point Specifies the point (in screen space) for which the view ray is to be built.
        \param[in] cameraMatrix Specifies the camera matrix (in world space).
        \return View ray (in world space), which can be used for intersection tests for instance.
        \note This also depends on the projection's viewport.
        */
        Math::Ray3<T> ViewRay(Math::Point2<T> point, Math::Matrix4<T> cameraMatrix) const
        {
            /* Adjust position for viewport origin */
            point -= GetViewport().position.Cast<T>();

            /* Transform point into unit device coordinates of the unit cube */
            auto halfSize = GetViewport().size.Cast<T>() * T(0.5);

            point.x = ( point.x - halfSize.width ) / halfSize.width;
            point.y = (-point.y + halfSize.height) / halfSize.height;

            Math::Ray3<T> ray;

            if (GetOrtho())
            {
                /* Build final ray */
                ray.origin.x    = point.x * orthoSize_.width * GetViewport().AspectRatio<T>() * T(0.5);
                ray.origin.y    = point.y * orthoSize_.height * T(0.5);

                ray.origin      = cameraMatrix * ray.origin;

                ray.direction   = cameraMatrix.GetColumn(2).Vec3().Normalize();
            }
            else
            {
                /* Clear position from camera matrix and make it to a view matrix */
                ray.origin = cameraMatrix.GetPosition();
                cameraMatrix.SetPosition({});
                cameraMatrix.MakeInverse();

                /* Build inverse-view-projection matrix */
                auto invViewProjection = GetMatrixLH();
                invViewProjection *= cameraMatrix;
                invViewProjection.MakeInverse();

                /* Build view ray direction */
                Math::Vector4<T> direction { point.x, point.y, 1, 1 };
                direction = invViewProjection * direction;

                /* Build final ray */
                ray.direction = direction.Vec3().Normalize();
            }

            return ray;
        }

        /* === Inline functions === */

        /**
        Returns true if this is a valid projection. This is the case if the viewport is valid.
        Viewport::IsValid
        */
        inline bool IsValid() const
        {
            return viewport_.IsValid();
        }

        //! Sets the projection viewport.
        inline void SetViewport(const Viewport& viewport)
        {
            if (viewport_ != viewport)
            {
                viewport_ = viewport;
                changedLH_ = true;
                changedRH_ = true;
            }
        }
        inline const Viewport& GetViewport() const
        {
            return viewport_;
        }

        inline void SetNearPlane(const T& nearPlane)
        {
            nearPlane_ = nearPlane;
            changedLH_ = true;
            changedRH_ = true;
        }
        inline const T& GetNearPlane() const
        {
            return nearPlane_;
        }

        inline void SetFarPlane(const T& farPlane)
        {
            farPlane_ = farPlane;
            changedLH_ = true;
            changedRH_ = true;
        }
        inline const T& GetFarPlane() const
        {
            return farPlane_;
        }

        /**
        Sets both the near- and far clipping plane distances.
        \see SetNearPlane
        \see SetFarPlane
        */
        inline void SetPlanes(const T& nearPlane, const T& farPlane)
        {
            nearPlane_ = nearPlane;
            farPlane_ = farPlane;
            changedLH_ = true;
            changedRH_ = true;
        }

        //! Sets the field-of-view (FOV) in radians.
        inline void SetFOV(const T& fieldOfView)
        {
            fieldOfView_ = fieldOfView;
            changedLH_ = true;
            changedRH_ = true;
        }
        //! Returns the field-of-view (FOV) in radians.
        inline const T& GetFOV() const
        {
            return fieldOfView_;
        }

        //! Sets the size for the orthogonal projection. By default { 3, 3 }.
        inline void SetOrthoSize(const Math::Size2<T>& size)
        {
            if (orthoSize_ != size)
            {
                orthoSize_ = size;
                changedLH_ = true;
                changedRH_ = true;
            }
        }
        //! Returns the size for the orthogonal projection.
        inline const Math::Size2<T>& GetOrthoSize() const
        {
            return orthoSize_;
        }

        //! Sets the projection to orthogonal if enabled. Otherwise (and by default) perspective projection is used.
        inline void SetOrtho(bool isOrtho)
        {
            if (isOrtho_ != isOrtho)
            {
                isOrtho_ = isOrtho;
                changedLH_ = true;
                changedRH_ = true;
            }
        }
        //! Returns true if the projection is set to orthogonal. Otherwise (and by default) perspective projection is used.
        inline bool GetOrtho() const
        {
            return isOrtho_;
        }

    private:

        /* === Members === */

        Viewport viewport_;
        
        T nearPlane_ = 1;
        T farPlane_ = 100;
        T fieldOfView_;

        Math::Size2<T> orthoSize_ { 3, 3 };

        bool isOrtho_ = false;

        mutable Math::Matrix4<T> matrixLH_, matrixRH_;
        mutable bool changedLH_ = true, changedRH_ = true;

};


typedef Projection<float> Projectionf;
typedef Projection<double> Projectiond;


} // /namespace Video

} // /namespace Fork


#endif



// ========================