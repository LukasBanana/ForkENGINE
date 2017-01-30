/*
 * Matrix 3x3 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATRIX3x3_H__
#define __FORK_MATRIX3x3_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Arithmetic/MatrixArithmetic.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix2.h"
#include "Quaternion.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Matrix 3x3 base class with 9 entries. This is a column major matrix.
\code
// / 0 3 6 \
// | 1 4 7 |
// \ 2 5 8 /
\endcode
\note OpenGL uses column major matrices (like it's typical in mathematical notations), but Direct3D uses row major matrices.
\ingroup math_core
*/
template <typename T> class Matrix3
{
    
    public:
        
        //! Number of columns: 3.
        static const size_t num = 3;

        //! Default constructor of the 3x3 matrix. This will initialize the matrix to its identity.
        Matrix3()
        {
            col[0] = { 1, 0, 0 };
            col[1] = { 0, 1, 0 };
            col[2] = { 0, 0, 1 };
        }
        Matrix3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2)
        {
            col[0] = col0;
            col[1] = col1;
            col[2] = col2;
        }
        /**
        Initializes the 3x3 matrix with the specified entries.
        'c' specifies the column and 'r' specifies the row.
        */
        Matrix3(
            const T& c0r0, const T& c1r0, const T& c2r0,
            const T& c0r1, const T& c1r1, const T& c2r1,
            const T& c0r2, const T& c1r2, const T& c2r2)
        {
            col[0].x = c0r0; col[1].x = c1r0; col[2].x = c2r0;
            col[0].y = c0r1; col[1].y = c1r1; col[2].y = c2r1;
            col[0].z = c0r2; col[1].z = c1r2; col[2].z = c2r2;
        }

        /* === Operators === */

        Matrix3<T>& operator += (const Matrix3<T>& other)
        {
            Math::Add(*this, *this, other);
            return *this;
        }
        Matrix3<T>& operator -= (const Matrix3<T>& other)
        {
            Math::Sub(*this, *this, other);
            return *this;
        }
        Matrix3<T>& operator *= (const Matrix3<T>& other)
        {
            auto prev = *this;
            Math::Mul(*this, prev, other);
            return *this;
        }

        Matrix3<T>& operator *= (const T& scalar)
        {
            auto prev = *this;
            Math::Mul(*this, prev, scalar);
            return *this;
        }

        Matrix3<T>& operator /= (const T& scalar)
        {
            auto prev = *this;
            Math::Mul(*this, prev, T(1) / scalar);
            return *this;
        }

        inline const T& operator [] (const size_t index) const
        {
            return *((&col[0].x) + index);
        }
        inline T& operator [] (const size_t index)
        {
            return *((&col[0].x) + index);
        }

        inline const T& operator () (const size_t column, const size_t row) const
        {
            return (col[column])[row];
        }
        inline T& operator () (const size_t column, const size_t row)
        {
            return (col[column])[row];
        }

        //! Multiplys the specified vector with this matrix. This performs a vector rotation.
        Vector2<T> operator * (const Vector2<T>& vec) const
        {
            return Vector2<T>(
                vec.x*col[0].x + vec.y*col[1].x,
                vec.x*col[0].y + vec.y*col[1].y
            );
        }

        //! Multiplys the specified vector with this matrix. This performs a vector rotation.
        Vector3<T> operator * (const Vector3<T>& vec) const
        {
            return Vector3<T>(
                vec.x*col[0].x + vec.y*col[1].x + vec.z*col[2].x,
                vec.x*col[0].y + vec.y*col[1].y + vec.z*col[2].y,
                vec.x*col[0].z + vec.y*col[1].z + vec.z*col[2].z
            );
        }

        /* === Functions === */

        inline Vector2<T> RotateVector(const Vector2<T>& vec) const
        {
            return *this * vec;
        }

        inline Vector3<T> RotateVector(const Vector3<T>& vec) const
        {
            return *this * vec;
        }

        Vector2<T> RotateVectorInv(const Vector2<T>& vec) const
        {
            return Vector2<T>(
                vec.x*col[0].x + vec.y*col[0].y,
                vec.x*col[1].x + vec.y*col[1].y
            );
        }

        Vector3<T> RotateVectorInv(const Vector3<T>& vec) const
        {
            return Vector3<T>(
                vec.x*col[0].x + vec.y*col[0].y + vec.z*col[0].z,
                vec.x*col[1].x + vec.y*col[1].y + vec.z*col[1].z,
                vec.x*col[2].x + vec.y*col[2].y + vec.z*col[2].z
            );
        }

        //! Returns the trace of this matrix, i.e. the sum of all matrix elements on the main diagonal.
        inline T Trace() const
        {
            return Math::Trace(*this);
        }

        inline bool HasIdentity() const
        {
            return Math::HasIdentity(*this);
        }

        /**
        Returns the determinant of this matrix. If this value is zero, the matrix can not be inverted.
        \see Determinant3x3
        */
        T Determinant() const
        {
            return Determinant3x3(*this);
        }

        /**
        Computes the inverse matrix.
        \param[out] inv Specifies the resulting inverse matrix.
        This must not be the same instance as this matrix. Otherwise the behaviour is undefined!
        \see InverseMatrix3x3
        */
        bool Inverse(Matrix3<T>& inv) const
        {
            return InverseMatrix3x3(inv, *this);
        }

        bool MakeInverse()
        {
            Matrix3<T> inv;
            if (Inverse(inv))
            {
                *this = inv;
                return true;
            }
            return false;
        }

        Matrix3<T> Inverse() const
        {
            auto inv = *this;
            inv.MakeInverse();
            return inv;
        }

        inline void Transpose(Matrix3<T>& trans)
        {
            Math::Transpose(trans, *this);
        }

        void MakeTranspose()
        {
            Matrix3<T> trans;
            Transpose(trans);
            *this = trans;
        }

        Matrix3<T> Transpose() const
        {
            auto trans = *this;
            trans.MakeTranspose();
            return trans;
        }

        inline void MakeIdentity()
        {
            Math::MakeIdentity(*this);
            return *this;
        }

        /**
        \code
        // / a d g \   / x 0 0 \   / ax dy gz \
        // | b e h | x | 0 y 0 | = | bx ey hz |
        // \ c f i /   \ 0 0 z /   \ cx fy iz /
        \endcode
        */
        Matrix3<T>& Scale(const Vector3<T>& vec)
        {
            col[0].x *= vec.x; col[0].y *= vec.x; col[0].z *= vec.x;
            col[1].x *= vec.y; col[1].y *= vec.y; col[1].z *= vec.y;
            col[2].x *= vec.z; col[2].y *= vec.z; col[2].z *= vec.z;
            return *this;
        }

        /**
        Performs an arbitrary rotation.
        \param[in] angle Specifies the rotation angle (in radian).
        \param[in] axis Specifies the rotation axis.
        */
        Matrix3<T>& RotateFree(const T& angle, const Vector3<T>& axis)
        {
            Math::RotateFree(*this, angle, axis);
            return *this;
        }
        
        /**
        \code
        // / 1 0  0 \
        // | 0 c -s |
        // \ 0 s  c /
        \endcode
        */
        Matrix3<T>& RotateX(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);
            
            /* Temporaries */
            const auto col1 = col[1];
            
            /* Rotation */
            col[1].x = col[1].x*c + col[2].x*s;
            col[1].y = col[1].y*c + col[2].y*s;
            col[1].z = col[1].z*c + col[2].z*s;
            
            col[2].x = col[2].x*c - col1.x*s;
            col[2].y = col[2].y*c - col1.y*s;
            col[2].z = col[2].z*c - col1.z*s;
            
            return *this;
        }
        
        /**
        \code
        // /  c 0 s \
        // |  0 1 0 |
        // \ -s 0 c /
        \endcode
        */
        Matrix3<T>& RotateY(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);
            
            /* Temporaries */
            const auto col0 = col[0];
            
            /* Rotation */
            col[0].x = col[0].x*c - col[2].x*s;
            col[0].y = col[0].y*c - col[2].y*s;
            col[0].z = col[0].z*c - col[2].z*s;
            
            col[2].x = col0.x*s + col[2].x*c;
            col[2].y = col0.y*s + col[2].y*c;
            col[2].z = col0.z*s + col[2].z*c;
            
            return *this;
        }
        
        /**
        \code
        // / c -s 0 \
        // | s  c 0 |
        // \ 0  0 1 /
        \endcode
        */
        Matrix3<T>& RotateZ(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);

            /* Temporaries */
            const auto col0 = col[0];

            /* Rotation */
            col[0].x = col[0].x*c + col[1].x*s;
            col[0].y = col[0].y*c + col[1].y*s;
            col[0].z = col[0].z*c + col[1].z*s;

            col[1].x = col[1].x*c - col0.x*s;
            col[1].y = col[1].y*c - col0.y*s;
            col[1].z = col[1].z*c - col0.z*s;

            return *this;
        }

        Vector3<T> GetRow(const size_t index) const
        {
            switch (index)
            {
                case 0: return Vector3<T>(col[0].x, col[1].x, col[2].x);
                case 1: return Vector3<T>(col[0].y, col[1].y, col[2].y);
                case 2: return Vector3<T>(col[0].z, col[1].z, col[2].z);
            }
            return Vector3<T>();
        }
        
        void SetRow(const size_t index, const Vector3<T>& vec)
        {
            switch (index)
            {
                case 0: col[0].x = vec.x, col[1].x = vec.y, col[2].x = vec.z; break;
                case 1: col[0].y = vec.x, col[1].y = vec.y, col[2].y = vec.z; break;
                case 2: col[0].z = vec.x, col[1].z = vec.y, col[2].z = vec.z; break;
            }
            return Vector3<T>();
        }

        inline const Vector3<T>& GetColumn(const size_t index) const
        {
            return col[index];
        }

        inline Vector3<T>& GetColumn(const size_t index)
        {
            return col[index];
        }

        inline void SetColumn(const size_t index, const Vector3<T>& vec)
        {
            col[index] = vec;
        }

        /**
        \see Math::ComputeMatrixScale
        \see Math::ComputeMatrixScaleSgn
        */
        Vector3<T> GetScale(bool sgn = false) const
        {
            return sgn ? Math::ComputeMatrixScaleSgn(*this) : Math::ComputeMatrixScale(*this);
        }

        //! Resizes each matrix column.
        void SetScale(const Vector3<T>& size)
        {
            GetColumn(0).Resize(size.x);
            GetColumn(1).Resize(size.y);
            GetColumn(2).Resize(size.z);
        }

        Quaternion<T> GetRotation() const
        {
            return Quaternion<T>(*this);
        }

        void SetRotation(const Quaternion<T>& rot)
        {
            *this = rot.Mat3();
        }

        Vector3<T> GetEulerRotation() const
        {
            //...
            return Vector3<T>();
        }

        void SetEulerRotation(const Vector3<T>& rot)
        {
            //...
        }

        //! Returns the 2x2 matrix out of this 3x3 matrix.
        Matrix2<T> Mat2() const
        {
            return Matrix2<T>(col[0].Vec2(), col[1].Vec2());
        }

        //! Returns this matrix as constant raw pointer.
        inline const T* Ptr() const
        {
            return col[0].Ptr();
        }
        //! Returns this matrix as raw pointer.
        inline T* Ptr()
        {
            return col[0].Ptr();
        }

        template <typename C> Matrix3<C> Cast() const
        {
            return Matrix3<C>(
                col[0].Cast<C>(),
                col[1].Cast<C>(),
                col[2].Cast<C>()
            );
        }

        /* === Members === */

        Vector3<T> col[3];

};


DEFAULT_MATH_BASE_OPERATORS(Matrix3)
DEFAULT_SCALAR_OPERATORS(Matrix3)
DEFAULT_MATH_TYPEDEFS(Matrix3)


} // /namespace Math

} // /namespace Fork


#endif



// ========================