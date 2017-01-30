/*
 * Transformation 2D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TRANSFORM2D_H__
#define __FORK_TRANSFORM2D_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Matrix4.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Base 2D transformation class. This class contains a 3D position,
3D scaling, quaternion and 4x4 matrix for the final transformation.
\note This class has a 'mutable' member which will be modified inside the "GetMatrix" function.
Recall that when you use this class with multi-threading!
\ingroup math_core
*/
template <typename T = float> class Transform2D
{
    
    public:
        
        Transform2D() = default;
        //! Implicit 4x4 matrix conversion.
        Transform2D(const Matrix4<T>& matrix) :
            position_   { matrix.GetPosition().Vec2()               },
            rotation_   { matrix.GetRotation().GetEulerRotation().z },
            scale_      { matrix.GetScale().Vec2()                  },
            matrix_     { matrix                                    }
        {
        }
        Transform2D(const Point2<T>& position, const T& rotation, const Vector2<T>& scale) :
            position_   { position },
            rotation_   { rotation },
            scale_      { scale    },
            changed_    { true     }
        {
        }

        /* === Operators === */

        Transform2D<T>& operator *= (const Transform2D<T>& other)
        {
            *this = (other.GetMatrix() * GetMatrix());
            return *this;
        }

        /* === Functions === */

        /**
        Returns the transformation 4x4 matrix. This function has the interface of a getter,
        but internally it works differently. This is due to an efficient matrix calculations and easy access.
        */
        const Matrix4<T>& GetMatrix() const
        {
            if (changed_)
            {
                /* Update matrix with current transformation settings */
                matrix_.MakeIdentity();
                matrix_.SetPosition({ position_.x, position_.y, 0 });
                matrix_.RotateZ(rotation_);
                matrix_.Scale({ scale_.x, scale_.y, 1 });
                changed_ = false;
            }
            return matrix_;
        }

        //! Returns this transformation as inverse.
        Transform2D<T> Inverse() const
        {
            Matrix4<T> mat = GetMatrix();
            mat.MakeInverse();
            return Transform2D<T>(mat);
        }

        inline void MakeInverse()
        {
            *this = Inverse();
        }

        //! Interpolates this transformation between the two others.
        void Interpolate(const Transform2D<T>& from, const Transform2D<T>& to, const T& t)
        {
            Math::Lerp(position_, from.position_, to.position_, t);
            Math::Lerp(rotation_, from.rotation_, to.rotation_, t);
            Math::Lerp(scale_   , from.scale_   , to.scale_   , t);
            changed_ = true;
        }

        //! Sets the position of this transformation.
        inline void SetPosition(const Point2<T>& position)
        {
            position_ = position;
            changed_ = true;
        }
        //! Returns the position of this transformation.
        inline const Point2<T>& GetPosition() const
        {
            return position_;
        }

        //! Sets the rotation of this transformation.
        inline void SetRotation(const T& rotation)
        {
            rotation_ = rotation;
            changed_ = true;
        }
        //! Returns the rotation of this transformation.
        inline const T& GetRotation() const
        {
            return rotation_;
        }

        //! Sets the scale of this transformation.
        inline void SetScale(const Vector2<T>& scale)
        {
            scale_ = scale;
            changed_ = true;
        }
        //! Returns the scale of this transformation.
        inline const Vector2<T>& GetScale() const
        {
            return scale_;
        }

        /**
        Translates the position into the specified (global) direction.
        To translate the position into the local direction -> use the "Move" function.
        \see MoveLocal
        */
        inline void MoveGlobal(const Vector2<T>& vec)
        {
            position_ += vec;
            changed_ = true;
        }
        /**
        Translates the position into the specified (local) direction.
        To translate the position into the global direction -> use the "Translate" function.
        This is equivalent to the following code:
        \code
        transform.MoveGlobal(transform.GetRotation() * vec);
        \endcode
        \see MoveGlobal
        \todo TEST
        */
        inline void MoveLocal(const Vector2<T>& vec)
        {
            MoveGlobal(
                { std::sin(rotation_) * vec.x,
                  std::cos(rotation_) * vec.y }
            );
        }

        /**
        Turns the transformation with the specified (relative) angles.
        \param[in] rotation Specifies the rotation which will be added to the transformation's rotation.
        \see Turn(const Quaternion<T>&, const Vector2<T>&)
        */
        inline void Turn(const T& rotation)
        {
            rotation_ += rotation;
            changed_ = true;
        }

        /**
        Turns the transformation with the specified (reltative) angles around the (global) pivot.
        \param[in] rotation Specifies the relative rotation (in radians).
        \param[in] pivot Specifies the global pivot.
        \todo Test me.
        */
        void Turn(const T& rotation, const Point2<T>& pivot)
        {
            /* Compute origin and movement offset */
            const Vector2<T> pivotOffset = GetPosition() - pivot;
            const Vector2<T> moveOffset(
                std::sin(rotation) * pivotOffset.x - pivotOffset.x,
                std::cos(rotation) * pivotOffset.y - pivotOffset.y
            );

            /* Set new rotation and translate object */
            position_ += moveOffset;
            rotation_ += rotation;

            changed_ = true;
        }

        /**
        Scales (or rather resizes) the transformation.
        \param[in] scale Specifies the size vector which will be added to the current scale vector.
        \see SetScale
        */
        inline void Scale(const Vector2<T>& scale)
        {
            scale_ += scale;
            changed_ = true;
        }

        /*void Scale(const Vector2<T>& scale, const Point2<T>& pivot)
        {
            //todo...
        }*/

    private:
        
        /* === Members === */

        Point2<T> position_;
        T rotation_ { 0 };
        Vector2<T> scale_ { 1 };

        mutable Matrix4<T> matrix_;

        mutable bool changed_ = false;

};


template <typename T> inline Transform2D<T> operator * (const Transform2D<T>& a, const Transform2D<T>& b)
{
    Transform2D<T> result = a;
    result *= b;
    return result;
}


DEFAULT_MATH_TYPEDEFS(Transform2D)


} // /namespace Math

} // /namespace Fork


#endif



// ========================