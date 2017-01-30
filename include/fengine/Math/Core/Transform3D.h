/*
 * Transformation 3D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TRANSFORM3D_H__
#define __FORK_TRANSFORM3D_H__


#include "Math/Core/Vector3.h"
#include "Math/Core/Quaternion.h"
#include "Math/Core/Matrix4.h"
#include "Math/Common/ExtMathFunctions.h"


namespace Fork
{

namespace Math
{


/**
Base 3D transformation class. This class contains a 3D position,
3D scaling, quaternion and 4x4 matrix for the final transformation.
\note This class has a 'mutable' member which will be modified inside the "GetMatrix" function.
Recall that when you use this class with multi-threading!
\ingroup math_core
*/
template <typename T = float> class Transform3D
{
    
    public:
        
        Transform3D() = default;
        //! Implicit 4x4 matrix conversion.
        Transform3D(const Matrix4<T>& matrix) :
            position_   { matrix.GetPosition() },
            rotation_   { matrix.GetRotation() },
            scale_      { matrix.GetScale()    },
            matrix_     { matrix               }
        {
        }
        Transform3D(const Point3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale) :
            position_   { position },
            rotation_   { rotation },
            scale_      { scale    },
            changed_    { true     }
        {
        }

        /* === Operators === */

        Transform3D<T>& operator *= (const Transform3D<T>& other)
        {
            *this = (other.GetMatrix() * GetMatrix());
            return *this;
        }

        /* === Functions === */

        /**
        Returns the transformation 4x4 matrix. This function has the interface of a getter,
        but internally it works differently. This is due to efficient matrix calculations and easy access.
        */
        const Matrix4<T>& GetMatrix() const
        {
            if (changed_)
            {
                /* Update matrix with current transformation settings */
                matrix_.MakeIdentity();
                matrix_.SetPosition(position_);
                matrix_ *= rotation_.Mat4();
                matrix_.Scale(scale_);
                changed_ = false;
            }
            return matrix_;
        }

        /**
        Returns this transformation as inverse. This will be computed as inverse affine matrix.
        \see Matrix4::MakeAffineInverse
        */
        Transform3D<T> Inverse() const
        {
            Matrix4<T> mat = GetMatrix();
            //mat.MakeAffineInverse();
            mat.MakeInverse();
            return Transform3D<T>(mat);
        }

        /**
        Makes this transformation to an inverse one. This is equivalent to the following code:
        \code
        transform = transform.Inverse();
        \endcode
        */
        inline void MakeInverse()
        {
            *this = Inverse();
        }

        //! Interpolates this transformation between the two others.
        void Interpolate(const Transform3D<T>& from, const Transform3D<T>& to, const T& t)
        {
            Math::Lerp(position_, from.position_, to.position_, t);
            Math::Lerp(scale_, from.scale_, to.scale_, t);
            rotation_.SLerp(from.rotation_, to.rotation_, t);
            changed_ = true;
        }

        //! Sets the position of this transformation.
        inline void SetPosition(const Point3<T>& position)
        {
            position_ = position;
            changed_ = true;
        }
        //! Returns the position of this transformation.
        inline const Point3<T>& GetPosition() const
        {
            return position_;
        }

        //! Sets the rotation of this transformation.
        inline void SetRotation(const Quaternion<T>& rotation)
        {
            rotation_ = rotation;
            changed_ = true;
        }
        //! Returns the rotation of this transformation.
        inline const Quaternion<T>& GetRotation() const
        {
            return rotation_;
        }

        //! Sets the scale of this transformation.
        inline void SetScale(const Vector3<T>& scale)
        {
            scale_ = scale;
            changed_ = true;
        }
        //! Returns the scale of this transformation.
        inline const Vector3<T>& GetScale() const
        {
            return scale_;
        }

        /**
        Moves the position into the specified (global) direction.
        To move the position into the local direction -> use the "MoveLocal" function.
        \see MoveLocal
        */
        inline void MoveGlobal(const Vector3<T>& vec)
        {
            position_ += vec;
            changed_ = true;
        }
        /**
        Moves the position into the specified (local) direction.
        To move the position into the global direction -> use the "MoveGlobal" function.
        This is equivalent to the following code:
        \code
        transform.MoveGlobal(transform.GetRotation() * vec);
        \endcode
        \see MoveGlobal
        */
        inline void MoveLocal(const Vector3<T>& vec)
        {
            MoveGlobal(GetRotation() * vec);
        }

        /**
        Turns the transformation with the specified (relative) angles (in object space).
        \param[in] rotation Specifies the rotation which will be multiplied with the transformation's rotation.
        \see Turn(const Quaternion<T>&, const Vector3<T>&)
        */
        inline void Turn(const Quaternion<T>& rotation)
        {
            rotation_ *= rotation;
            changed_ = true;
        }

        /**
        Turns the transformation with the specified (relative) angles (in object- or world space).
        \param[in] rotation Specifies the rotation which will be multiplied with the transformation's rotation.
        \param[in] useWorldSpace Specifies whether the rotation is to be performed in world or object space.
        If true, world space is used, otherwise object space (which is the default for the other "Turn" functions).
        \see Turn(const Quaternion<T>&, const Vector3<T>&)
        */
        void Turn(const Quaternion<T>& rotation, bool useWorldSpace)
        {
            if (useWorldSpace)
                rotation_ = rotation * rotation_;
            else
                rotation_ *= rotation;
            changed_ = true;
        }

        /**
        Turns the transformation with the specified (reltative) angles around the (global) pivot.
        \param[in] rotation Specifies the relative rotation.
        \param[in] pivot Specifies the global pivot.
        */
        void Turn(const Quaternion<T>& rotation, const Point3<T>& pivot)
        {
            /* Compute origin and movement offset */
            const auto pivotOffset = GetPosition() - pivot;
            const auto moveOffset = rotation * pivotOffset - pivotOffset;

            /* Set new rotation and translate object */
            position_ += moveOffset;
            rotation_ *= rotation;

            changed_ = true;
        }

        /**
        Scales (or rather resizes) the transformation.
        \param[in] scale Specifies the size vector which will be added to the current scale vector.
        \see SetScale
        */
        inline void Scale(const Vector3<T>& scale)
        {
            scale_ += scale;
            changed_ = true;
        }

        /**
        Scales (or rather resizes) the transformation around the (global) pivot.
        \param[in] scale Specifies the size vector which will be added to the current scale vector.
        \param[in] pivot Specifies the global pivot.
        */
        void Scale(const Vector3<T>& scale, const Point3<T>& pivot)
        {
            /* Compute pivot offset, scale, then move along the offset vector */
            auto pivotOffset = GetMatrix().Inverse() * pivot;
            scale_ += scale;
            MoveLocal(-pivotOffset * scale);
        }

        /**
        Lets the transformation look at the specified global position.
        \param[in] focusPosition Specifies the global position, to which the transformation will look.
        */
        void LookAt(const Point3<T>& focusPosition)
        {
            const auto& pos = position_;
            Vector3<T> rot;

            /* Calculate rotation */
            if (!Equal(focusPosition.y, pos.y))
            {
                auto dist = Distance(focusPosition, pos);
                rot.x = -std::asin( (focusPosition.y - pos.y) / dist );
            }

            if (!Equal(focusPosition.x, pos.x))
            {
                auto dist = Distance(Point2<T>(focusPosition.x, focusPosition.z), Point2<T>(pos.x, pos.z));
                rot.y = -std::asin( (focusPosition.x - pos.x) / dist );
            }

            if (pos.z < focusPosition.z)
                rot.y = T(pi*2) - rot.y;

            /* Set euler rotation */
            SetRotation(Quaternion<T>(rot));
        }

        /**
        Returns the transformation without scaling as a 4x4 matrix.
        \remarks A non-scaled matrix transformation is required for physics simulation,
        since a physics engine only simulates the translation and rotation but no scaling.
        */
        Matrix4<T> NonScaledMatrix() const
        {
            Matrix4<T> matrix;
            matrix.SetPosition(position_);
            matrix.SetRotation(Math::Quaternionf(rotation_).Normalize());
            return matrix;
        }

        /**
        Extracts the translation, rotation and scaling from the specified matrix.
        \note The specified matrix must be an affine matrix,
        i.e. the last row must be { 0, 0, 0, 1 }. Otherwise the behavior is undefined!
        \throws TransformException (Only in debug mode) when this is not an affine matrix.
        \see EnsureAffine
        */
        void SetupMatrix(const Matrix4<T>& matrix)
        {
            #ifdef FORK_DEBUG
            matrix.EnsureAffine();
            #endif
            position_   = matrix.GetPosition();
            rotation_   = matrix.GetRotation();
            scale_      = matrix.GetScale();
            matrix_     = matrix;
            changed_    = false;
        }

        /**
        Extracts the translation and rotation but not the scaling from the specified matrix.
        \remarks This should be used, when a scene node gets a transformation from a physics body,
        since a physics engine only simulates the translation and rotation but no scaling.
        */
        void SetupNonScaleMatrix(const Matrix4<T>& matrix)
        {
            position_   = matrix.GetPosition();
            rotation_   = matrix.GetRotation();
            changed_    = true;
        }

    private:
        
        /* === Members === */

        Point3<T> position_;
        Quaternion<T> rotation_;
        Vector3<T> scale_ { 1 };

        mutable Matrix4<T> matrix_;

        mutable bool changed_ = false;

};


template <typename T> inline Transform3D<T> operator * (const Transform3D<T>& a, const Transform3D<T>& b)
{
    Transform3D<T> result = a;
    result *= b;
    return result;
}


DEFAULT_MATH_TYPEDEFS(Transform3D)


} // /namespace Math

} // /namespace Fork


#endif



// ========================