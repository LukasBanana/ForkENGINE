/*
 * Quaternion header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_QUATERNION_H__
#define __FORK_QUATERNION_H__


#include "Arithmetic/VectorArithmetic.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Math/Core/MathConstants.h"

#include <cmath>


namespace Fork
{

namespace Math
{


template <typename T> class Matrix3;
template <typename T> class Matrix4;

//! Quaternion base class with components x, y, z and w.
template <typename T = float> class Quaternion
{
    
    public:
        
        //! Number of components: 4.
        static const size_t num = 4;

        Quaternion() = default;
        /**
        Sets up the quaternion with the specified vector components x, y, and z as euler rotation.
        \see SetEulerRotation
        */
        Quaternion(const T& vX, const T& vY, const T& vZ)
        {
            SetupEulerRotation({ vX, vY, vZ });
        }
        //! Directly assigns the components x, y, z, and w to the quaternion.
        Quaternion(const T& vX, const T& vY, const T& vZ, const T& vW) :
            x{ vX },
            y{ vY },
            z{ vZ },
            w{ vW }
        {
        }
        /**
        Sets up the quaternion with the specified vector as euler rotation.
        \see SetEulerRotation
        */
        explicit Quaternion(const Vector3<T>& vec)
        {
            SetupEulerRotation(vec);
        }
        //! Directly converts the 4D vector into a quaternion.
        explicit Quaternion(const Vector4<T>& vec) :
            Quaternion(vec.x, vec.y, vec.z, vec.w)
        {
        }
        explicit Quaternion(const Matrix3<T>& mat);
        explicit Quaternion(const Matrix4<T>& mat);

        /* === Operators === */

        Quaternion<T>& operator += (const Quaternion<T>& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Quaternion<T>& operator -= (const Quaternion<T>& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Quaternion<T>& operator *= (const Quaternion<T>& other)
        {
            Quaternion<T> result;
            
            result.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
            result.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
            result.y = (other.w * y) - (other.x * z) + (other.y * w) + (other.z * x);
            result.z = (other.w * z) + (other.x * y) - (other.y * x) + (other.z * w);
            
            *this = result;
            return *this;
        }

        Quaternion<T>& operator *= (const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        //! Rotates the specified vector with this quaternion.
        Vector3<T> operator * (const Vector3<T>& vec) const
        {
            Vector3<T> qvec(x, y, z);

            auto uv = Math::Cross(qvec, vec);
            auto uuv = Math::Cross(qvec, uv);
            uv *= (T(2) * w);
            uuv *= T(2);

            /* Result := vec + uv + uuv */
            uv += uuv;
            uv += vec;

            return uv;
        }

        //! Returns a constant reference to the specified component by index. Index must be 0, 1, 2 or 3.
        inline const T& operator [] (const size_t index) const
        {
            return *((&x) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0, 1, 2 or 3.
        inline T& operator [] (const size_t index)
        {
            return *((&x) + index);
        }

        /* === Functions === */

        inline Quaternion<T>& Normalize()
        {
            Math::Normalize(*this);
            return *this;
        }

        //! Makes this quaternion to its inverse rotation.
        void MakeInverse()
        {
            x = -x;
            y = -y;
            z = -z;
        }

        //! Returns the inverse rotation of this quaternion.
        Quaternion<T> Inverse() const
        {
            return Quaternion<T>(-x, -y, -z, w);
        }

        /**
        Sets this quaternions to the specified euler rotation.
        \param[in] vec Specifies the rotation angles (in radians).
        */
        Quaternion<T>& SetupEulerRotation(const Vector3<T>& vec)
        {
            const T cr = std::cos(vec.x/2);
            const T cp = std::cos(vec.y/2);
            const T cy = std::cos(vec.z/2);

            const T sr = std::sin(vec.x/2);
            const T sp = std::sin(vec.y/2);
            const T sy = std::sin(vec.z/2);

            const T cpcy = cp * cy;
            const T spsy = sp * sy;
            const T cpsy = cp * sy;
            const T spcy = sp * cy;

            x = sr * cpcy - cr * spsy;
            y = cr * spcy + sr * cpsy;
            z = cr * cpsy - sr * spcy;
            w = cr * cpcy + sr * spsy;

            Normalize();

            return *this;
        }

        /**
        Returns this quaternion as euler rotation.
        \note In euler rotations some information can get lost!
        */
        Vector3<T> EulerRotation() const
        {
            const T sqX = x*x;
            const T sqY = y*y;
            const T sqZ = z*z;
            const T sqW = w*w;

            return Vector3<T>(
                std::atan2(T(2) * (y*z + x*w), -sqX - sqY + sqZ + sqW),
                std::asin(Math::Clamp(T(2) * (y*w - x*z), T(-1), T(1))),
                std::atan2(T(2) * (x*y + z*w), sqX - sqY - sqZ + sqW)
            );
        }

        /**
        Sets this quaternion to the specified angle axis rotation.
        \todo Test me.
        */
        Quaternion<T>& SetupAngleAxis(const T& angle, const Vector3<T>& axis)
        {
            const T halfAngle   = angle / 2;
            const T sine        = std::sin(halfAngle);

            x = sine * axis.x;
            y = sine * axis.y;
            z = sine * axis.z;
            w = cos(halfAngle);

            return *this;
        }

        /**
        Returns this quaternion as angle axis rotation.
        \todo Test me.
        */
        void AngleAxis(T& angle, Vector3<T>& axis) const
        {
            const T scale = sqrt(x*x + y*y + z*z);

            if ( ( std::abs(scale) < Math::Epsilon<T>() ) || w > T(1) || w < T(-1) )
            {
                axis.x  = T(0);
                axis.y  = T(1);
                axis.z  = T(0);
                angle   = T(0);
            }
            else
            {
                const T invScale = T(1) / scale;
                axis.x  = x * invScale;
                axis.y  = y * invScale;
                axis.z  = z * invScale;
                angle   = T(2) * std::acos(w);
            }
        }

        /**
        Computes a linear interpolation.
        \see SLerp
        */
        void Lerp(const Quaternion<T>& from, const Quaternion<T>& to, const T& t)
        {
            const T invT = T(1) - t;
            *this = (from * invT) + (to * t);
        }

        /**
        Computes a Spherical linear interpolation.
        \param[in] t Specifies the interpolation factor. Should be in the range [0.0 .. 1.0].
        \param[in] from Specifies the start rotation.
        \param[in] to Specifies the end rotation.
        */
        void SLerp(const Quaternion<T>& from, const Quaternion<T>& to, const T& t)
        {
            /* Temporary variables */
            T to1[4];
            T omega, cosom, sinom;
            T scale0, scale1;

            /* Calculate cosine */
            cosom = Math::Dot(from, to);

            /* Adjust signs (if necessary) */
            if (cosom < T(0))
            {
                cosom = -cosom;
                to1[0] = -to.x;
                to1[1] = -to.y;
                to1[2] = -to.z;
                to1[3] = -to.w;
            }
            else
            {
                to1[0] = to.x;
                to1[1] = to.y;
                to1[2] = to.z;
                to1[3] = to.w;
            }

            /* Calculate coefficients */
            if ((T(1) - cosom) > Math::Epsilon<T>()) 
            {
                /* Standard case (slerp) */
                omega = std::acos(cosom);
                sinom = std::sin(omega);
                scale0 = std::sin((T(1) - t) * omega) / sinom;
                scale1 = std::sin(t * omega) / sinom;
            }
            else
            {        
                /*
                "from" and "to" quaternions are very close 
                ... so we can do a linear interpolation
                */
                scale0 = T(1) - t;
                scale1 = t;
            }

            /* Calculate final values */
            x = scale0*from.x + scale1*to1[0];
            y = scale0*from.y + scale1*to1[1];
            z = scale0*from.z + scale1*to1[2];
            w = scale0*from.w + scale1*to1[3];
        }

        /**
        Converts this quaternion rotation into a 3x3 matrix rotation.
        \see Math::ConvertQuaternionToMatrix
        */
        inline Matrix3<T> Mat3() const;
        /**
        Converts this quaternion rotation into a transposed 3x3 matrix rotation.
        \see Math::ConvertQuaternionToMatrixTransposed
        */
        inline Matrix3<T> Mat3Transposed() const;

        /**
        Converts this quaternion rotation into a 4x4 matrix rotation.
        \see Math::ConvertQuaternionToMatrix
        */
        inline Matrix4<T> Mat4() const;
        /**
        Converts this quaternion rotation into a transposed 4x4 matrix rotation.
        \see Math::ConvertQuaternionToMatrixTransposed
        */
        inline Matrix4<T> Mat4Transposed() const;

        //! Loads the quaternion's identity.
        inline void MakeIdentity()
        {
            x = y = z = T(0);
            w = T(1);
        }

        /* === Members === */

        T x = 0, y = 0, z = 0, w = 1;

};


/* --- Global operators --- */

DEFAULT_SCALAR_OPERATORS(Quaternion)
DEFAULT_MATH_OPERATOR(Quaternion, +, +=)
DEFAULT_MATH_OPERATOR(Quaternion, -, -=)
DEFAULT_MATH_OPERATOR(Quaternion, *, *=)
DEFAULT_MATH_TYPEDEFS(Quaternion)


} // /namespace Math

} // /namespace Fork


#endif



// ========================