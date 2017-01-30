/*
 * Matrix 2x2 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATRIX2x2_H__
#define __FORK_MATRIX2x2_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Arithmetic/MatrixArithmetic.h"
#include "Vector2.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Matrix 2x2 base class with 4 entries. This is a column major matrix.
\code
// / 0 2 \
// \ 1 3 /
\endcode
\note OpenGL uses column major matrices (like it's typical in mathematical notations), but Direct3D uses row major matrices.
\ingroup math_core
*/
template <typename T> class Matrix2
{
    
    public:
        
        //! Number of columns: 2.
        static const size_t num = 2;

        //! Default constructor of the 2x2 matrix. This will initialize the matrix to its identity.
        Matrix2()
        {
            col[0] = { 1, 0 };
            col[1] = { 0, 1 };
        }
        Matrix2(const Vector2<T>& col0, const Vector2<T>& col1)
        {
            col[0] = col0;
            col[1] = col1;
        }
        /**
        Initializes the 2x2 matrix with the specified entries.
        'c' specifies the column and 'r' specifies the row.
        */
        Matrix2(
            const T& c0r0, const T& c1r0,
            const T& c0r1, const T& c1r1)
        {
            col[0].x = c0r0; col[1].x = c1r0;
            col[0].y = c0r1; col[1].y = c1r1;
        }

        /* === Operators === */

        Matrix2<T>& operator += (const Matrix2<T>& other)
        {
            Math::Add(*this, *this, other);
            return *this;
        }
        Matrix2<T>& operator -= (const Matrix2<T>& other)
        {
            Math::Sub(*this, *this, other);
            return *this;
        }
        Matrix2<T>& operator *= (const Matrix2<T>& other)
        {
            auto prev = *this;
            Math::Mul(*this, prev, other);
            return *this;
        }

        Matrix2<T>& operator *= (const T& scalar)
        {
            auto prev = *this;
            Math::Mul(*this, prev, scalar);
            return *this;
        }

        Matrix2<T>& operator /= (const T& scalar)
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

        Vector2<T> operator * (const Vector2<T>& vec) const
        {
            return Vector2<T>(
                vec.x*col[0].x + vec.y*col[1].x,
                vec.x*col[0].y + vec.y*col[1].y
            );
        }

        /* === Functions === */

        inline Vector2<T> RotateVector(const Vector2<T>& vec) const
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
        \see Determinant2x2
        */
        T Determinant() const
        {
            return Determinant2x2(*this);
        }

        /**
        Computes the inverse matrix.
        \param[out] inv Specifies the resulting inverse matrix.
        This must not be the same instance as this matrix. Otherwise the behaviour is undefined!
        \see InverseMatrix2x2
        */
        bool Inverse(Matrix2<T>& inv) const
        {
            return InverseMatrix2x2(inv, *this);
        }

        bool MakeInverse()
        {
            Matrix2<T> inv;
            if (Inverse(inv))
            {
                *this = inv;
                return true;
            }
            return false;
        }

        Matrix2<T> Inverse() const
        {
            auto inv = *this;
            inv.MakeInverse();
            return inv;
        }

        inline void Transpose(Matrix2<T>& trans)
        {
            Math::Transpose(trans, *this);
        }

        void MakeTranspose()
        {
            Matrix2<T> trans;
            Transpose(trans);
            *this = trans;
        }

        Matrix2<T> Transpose() const
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
        // / a c \   / x 0 \   / ax cy \
        // \ b d / x \ 0 y / = \ bx dy /
        \endcode
        */
        Matrix2<T>& Scale(const Vector3<T>& vec)
        {
            col[0].x *= vec.x; col[0].y *= vec.x;
            col[1].x *= vec.y; col[1].y *= vec.y;
            return *this;
        }

        Matrix2<T>& Rotate(const T& angle)
        {
            //...
            return *this;
        }
        
        Vector2<T> GetRow(const size_t index) const
        {
            switch (index)
            {
                case 0: return Vector2<T>(col[0].x, col[1].x);
                case 1: return Vector2<T>(col[0].y, col[1].y);
            }
            return Vector2<T>();
        }
        
        void SetRow(const size_t index, const Vector2<T>& vec)
        {
            switch (index)
            {
                case 0: col[0].x = vec.x, col[1].x = vec.y; break;
                case 1: col[0].y = vec.x, col[1].y = vec.y; break;
            }
            return Vector2<T>();
        }

        inline const Vector2<T>& GetColumn(const size_t index) const
        {
            return col[index];
        }

        inline Vector2<T>& GetColumn(const size_t index)
        {
            return col[index];
        }

        inline void SetColumn(const size_t index, const Vector2<T>& vec)
        {
            col[index] = vec;
        }

        Vector2<T> GetScale(bool sgn = false) const
        {
            return Vector2<T>(
                mat.GetColumn(0).Length() * Sgn( Dot(mat.GetColumn(0), { 1, 0 }) ),
                mat.GetColumn(1).Length() * Sgn( Dot(mat.GetColumn(1), { 0, 1 }) )
            );
        }

        //! Resizes each matrix column.
        void SetScale(const Vector2<T>& size)
        {
            GetColumn(0).Resize(size.x);
            GetColumn(1).Resize(size.y);
        }

        T GetRotation() const
        {
            //...
            return T(0);
        }

        void SetRotation(const T& rot)
        {
            //...
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

        template <typename C> Matrix2<C> Cast() const
        {
            return Matrix2<C>(
                col[0].Cast<C>(),
                col[1].Cast<C>()
            );
        }

        /* === Members === */

        Vector2<T> col[2];

};


DEFAULT_MATH_BASE_OPERATORS(Matrix2)
DEFAULT_SCALAR_OPERATORS(Matrix2)
DEFAULT_MATH_TYPEDEFS(Matrix2)


} // /namespace Math

} // /namespace Fork


#endif



// ========================