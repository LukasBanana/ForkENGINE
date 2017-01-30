/*
 * Matrix arithmetic header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATRIX_ARITHMETIC_H__
#define __FORK_MATRIX_ARITHMETIC_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/DefaultMathOperators.h"
#include "Math/Core/Arithmetic/VectorArithmetic.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Math
{


/**
Matrix multiplication main function.
\tparam num Specifies the number of matrix rows and columns (i.e. the matrix has num x num elements, e.g. num = 4 -> 4x4 Matrix).
\tparam T Specifies the base data type.
\param[out] out Pointer to the output matrix. Must not be the same pointer as for parameters "a" and "b"!
\param[in] a Pointer to the first matrix.
\param[in] b Pointer to the second matrix.
\return True if the matrices could be multiplied. Otherwise at least on of the two parameters ("a" or "b")
are the same pointer as for the output matrix.
\note These pointers must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline void Mul(T* const out, T const * const a, T const * const b)
{
    for (size_t i = 0, n = num*num; i < n; ++i)
    {
        out[i] = T(0);
            
        for (size_t j = 0; j < num; ++j)
            out[i] += a[ i % num + j * num ] * b[ i - (i % num) + j ];
    }
}

/**
Matrix multiplication function.
\tparam M Specifies the matrix type. This type must have a static constant field
named "num" specifying the number of rows and columns of the matrix type.
It must also implement a function called "T* Ptr()" and "const T* Ptr() const" returning the
matrix as raw pointer. This could be Matrix2, Matrix3 or Matrix4.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[out] out Specifies the output matrix. Must not be the same object as for parameters "a" and "b"!
\param[in] a Specifies the first matrix.
\param[in] b Specifies the second matrix.
\return True if the matrices could be multiplied. Otherwise at least on of the two parameters ("a" or "b")
is the same object as for the output matrix.
\ingroup math_core
*/
template < template <typename> class M, typename T > inline void Mul(M<T>& out, const M<T>& a, const M<T>& b)
{
    Mul<M<T>::num, T>(out.Ptr(), a.Ptr(), b.Ptr());
}

template <size_t num, typename T> inline void Mul(T* const out, T const * const in, const T& scalar)
{
    for (size_t i = 0, n = num*num; i < n; ++i)
        out[i] = in[i] * scalar;
}

//! \see Math::Mul
template < template <typename> class M, typename T > inline void Mul(M<T>& out, const M<T>& in, const T& scalar)
{
    Mul<M<T>::num, T>(out.Ptr(), in.Ptr(), scalar);
}

//! \see Math::Mul
template <size_t num, typename T> void Add(T* out, T const * const a, T const * const b)
{
    for (size_t i = 0, n = num*num; i < n; ++i)
        out[i] = a[i] + b[i];
}

//! \see Math::Mul
template < template <typename> class M, typename T > inline void Add(M<T>& out, const M<T>& a, const M<T>& b)
{
    Add<M<T>::num, T>(out.Ptr(), a.Ptr(), b.Ptr());
}

//! \see Math::Mul
template <size_t num, typename T> void Sub(T* out, T const * const a, T const * const b)
{
    for (size_t i = 0, n = num*num; i < n; ++i)
        out[i] = a[i] - b[i];
}

//! \see Math::Mul
template < template <typename> class M, typename T > inline void Sub(M<T>& out, const M<T>& a, const M<T>& b)
{
    Sub<M<T>::num, T>(out.Ptr(), a.Ptr(), b.Ptr());
}

/**
Loads the matrix identity. Thereafter, the matrix includes all zeros except
for the diagonal, where it contains only ones.
\param[out] out Pointer to the output matrix.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> void MakeIdentity(T* const out)
{
    for (size_t i = 0, n = num*num; i < n; ++i)
        out[i] = T(i % (num + 1) == 0 ? 1 : 0);
}

/**
Loads the matrix identity. For more information see the first "LoadIdentity" function.
\ingroup math_core
*/
template < template <typename> class M, typename T > inline void MakeIdentity(M<T>& out)
{
    MakeIdentity<M<T>::num, T>(out.Ptr());
}

/**
Determines whether the specified matrix has the identity or not.
\param[in] Matrix Constant pointer to the matrix which is to be checked for identity.
\return True if the specified matrix has the identity. This happens when all
elements are zero, except the elements on the main diagonal.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> bool HasIdentity(T const * const mat)
{
    for (size_t col = 0; col < num; ++col)
    {
        for (size_t row = 0; row < num; ++row)
        {
            if (!Math::Equal(mat[col * num + row], T(col == row ? 1 : 0)))
                return false;
        }
    }
    return true;
}

template < template <typename> class M, typename T > inline bool HasIdentity(const M<T>& mat)
{
    return HasIdentity<M<T>::num, T>(mat.Ptr());
}

/**
Returns the trace of the specified matrix. This is the sum of the elements on the main diagonal.
\param[in] mat Pointer to the matrix whose trace is to be computed.
\return Trace of the matrix (M(0, 0) + M(1, 1) + ... + M(n, n)).
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> T Trace(T const * const mat)
{
    T trace = T(0);
    
    for (size_t i = 0; i < num; ++i)
        trace += mat[i * num + i];
    
    return trace;
}

template < template <typename> class M, typename T > inline T Trace(const M<T>& mat)
{
    return Trace<M<T>::num, T>(mat.Ptr());
}

/*
Transposes the specified matrix. Only the relevant matrix elements will be swaped.
\tparam num Specifies the number of matrix rows and columns (i.e. the matrix has num x num elements, e.g. num = 4 -> 4x4 matrix).
\tparam T Specifies the base data type.
\param[in,out] mat Pointer to the matrix which is to be transposed.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> void Transpose(T* const mat)
{
    for (size_t i = 0; i + 1 < num; ++i)
    {
        for (size_t j = 1; j + i < num; ++j)
        {
            std::swap(
                mat[i*(num + 1) + j],
                mat[(j + i)*num + i]
            );
        }
    }
}

/**
Transposes the specified matrix. For more information see the other "transpose" function.
\ingroup math_core
*/
template < template <typename> class M, typename T > inline void Transpose(M<T>& mat)
{
    Transpose<M<T>::num, T>(mat.Ptr());
}

/**
Makes a transposed copy of the specified matrix.
\param[out] out Pointer to the output matrix. Must not be the same pointer as "in"!
\param[in] in Pointer to the input matrix which is to be transposed.
\note These pointers must never be null!
\ingroup math_core
*/
template <size_t num, typename T> void Transpose(T* const out, T const * const in)
{
    for (size_t col = 0; col < num; ++col)
    {
        for (size_t row = 0; row < num; ++row)
            out[col*num + row] = in[row*num + col];
    }
}

/**
Transposes the specified matrix. For more information see the other "Transpose" function.
\ingroup math_core
*/
template < template <typename> class M, typename T > inline void Transpose(M<T>& out, const M<T>& in)
{
    Transpose<M<T>::num, T>(out.Ptr(), in.Ptr());
}

/**
Rotates the specified matrix in a free direction.
\tparam M Specifies the matrix type. This type must overload the following operator:
\code
// 'column' and 'row' can be 0, 1 or 2.
T& operator () (size_t column, size_t row);
\endcode
This could be Matrix3 or Matrix4.
\tparam V Specifies the vector type. This must be suitable for the "Normalize" function.
This could be Vector3 or Vector4.
\tparam T Specifies the base data type (.
\param[out] matrix Specifies the matrix which is to be rotated.
\param[in] angle Specifies the rotation angle (in radian).
\param[in] axis Specifies the rotation axis.
\ingroup math_core
*/
template < template <typename> class M, template <typename> class V, typename T > void RotateFree(
    M<T>& matrix, const T& angle, V<T> axis)
{
    /* Normalize rotation vector */
    Normalize(axis);

    const T& x = axis.x;
    const T& y = axis.y;
    const T& z = axis.z;

    /* Setup rotation values */
    const T c   = std::cos(angle);
    const T s   = std::sin(angle);
    const T cc  = T(1) - c;

    /* Perform matrix rotation */
    M<T> rot;

    rot(0, 0) = x*x*cc + c;   rot(1, 0) = x*y*cc - z*s; rot(2, 0) = x*z*cc + y*s;
    rot(0, 1) = y*x*cc + z*s; rot(1, 1) = y*y*cc + c;   rot(2, 1) = y*z*cc - x*s;
    rot(0, 2) = x*z*cc - y*s; rot(1, 2) = y*z*cc + x*s; rot(2, 2) = z*z*cc + c;

    matrix *= rot;
}

/**
Computes the (signum) scaling of the upper 3x3 part of the specified matrix.
\tparam M Specifies the matrix type. This type must have one of the the following functions:
\code
// index may be 0, 1 or 2.
Vector3<T> GetColumn(size_t index) const;
Vector4<T> GetColumn(size_t index) const;
\endcode
This could be Matrix3 or Matrix4.
\tparam T Specifies the data type (should be float or double).
\param[in] mat Specifies the matrix whose scaling is to be computed.
\param[in] unitVecX Specifies the unit vector in X direction for signum comparision.
If the input matrix 'mat' is rotated, the three unit vectors should also be rotated.
Otherwise this function may produce a wrong result, if the matrix is rotated too much
from the identity matrix (i.e. rotated 90 degrees in any direction).
\ingroup math_core
*/
template < template <typename> class M, typename T > Vector3<T> ComputeMatrixScaleSgn(
    const M<T>& mat,
    const Vector3<T>& unitVecX = Vector3<T>(1, 0, 0),
    const Vector3<T>& unitVecY = Vector3<T>(0, 1, 0),
    const Vector3<T>& unitVecZ = Vector3<T>(0, 0, 1))
{
    return Vector3<T>(
        mat.GetColumn(0).Length() * Sgn( Dot(mat.GetColumn(0), unitVecX) ),
        mat.GetColumn(1).Length() * Sgn( Dot(mat.GetColumn(1), unitVecY) ),
        mat.GetColumn(2).Length() * Sgn( Dot(mat.GetColumn(2), unitVecZ) )
    );
}

/**
Computes the scaling of the upper 3x3 part of the specified matrix.
\tparam M Specifies the matrix type. This type must have one of the the following functions:
\code
// index may be 0, 1 or 2.
Vector3<T> GetColumn(size_t index) const;
Vector4<T> GetColumn(size_t index) const;
\endcode
This could be Matrix3 or Matrix4.
\tparam T Specifies the data type (should be float or double).
\param[in] mat Specifies the matrix whose scaling is to be computed.
\ingroup math_core
*/
template < template <typename> class M, typename T > Vector3<T> ComputeMatrixScale(const M<T>& mat)
{
    return Vector3<T>(
        mat.GetColumn(0).Length(),
        mat.GetColumn(1).Length(),
        mat.GetColumn(2).Length()
    );
}

/**
Computes the determinant of the specified left-upper 2x2 matrix.
\tparam M Specifies the matrix type. This type must overload the following operators:
\code
// 'column' and 'row' are 0 or 1.
template <typename T> T operator () (size_t column, size_t row) const;
\endcode
This type must also implement a static constant member "size_t num",
whose value must be greater than or equal to 2. This should be Matrix2, Matrix3 or Matrix4.
\tparam T Specifies the data type. This should be float or double.
\param[in] m Specifies the matrix.
\return The matrix determinant.
\see Matrix2::Determinant
\see Matrix3::Determinant
\see Matrix4::Determinant
*/
template < template <typename> class M, typename T > T Determinant2x2(const M<T>& m)
{
    static_assert(M<T>::num >= 2, "Matrix type must have at least 2 columns and rows");
    return m(0, 0)*m(1, 1) - m(1, 0)*m(0, 1);
}

/**
Computes the determinant of the specified left-upper 3x3 matrix.
\tparam M Specifies the matrix type. This type must overload the following operators:
\code
// 'column' and 'row' are 0, 1 or 2.
template <typename T> T operator () (size_t column, size_t row) const;
\endcode
This type must also implement a static constant member "size_t num",
whose value must be greater than or equal to 3. This should be Matrix3 or Matrix4.
\tparam T Specifies the data type. This should be float or double.
\param[in] m Specifies the matrix.
\return The matrix determinant.
\see Matrix3::Determinant
\see Matrix4::Determinant
*/
template < template <typename> class M, typename T > T Determinant3x3(const M<T>& m)
{
    static_assert(M<T>::num >= 3, "Matrix type must have at least 3 columns and rows");
    return
        ( m(0, 0) * m(1, 1) * m(2, 2) ) + ( m(1, 0) * m(2, 1) * m(0, 2) ) + ( m(2, 0) * m(0, 1) * m(1, 2) ) -
        ( m(0, 2) * m(1, 1) * m(2, 0) ) - ( m(0, 2) * m(2, 1) * m(0, 0) ) - ( m(2, 2) * m(0, 1) * m(1, 0) );
}

/**
Computes the determinant of the specified left-upper 4x4 matrix.
\tparam M Specifies the matrix type. This type must overload the following operators:
\code
// 'column' and 'row' are 0, 1, 2 or 3.
template <typename T> T operator () (size_t column, size_t row) const;
\endcode
This type must also implement a static constant member "size_t num",
whose value must be greater than or equal to 4. This should be Matrix4.
\tparam T Specifies the data type. This should be float or double.
\param[in] m Specifies the matrix.
\return The matrix determinant.
\see Matrix4::Determinant
*/
template < template <typename> class M, typename T > T Determinant4x4(const M<T>& m)
{
    static_assert(M<T>::num >= 4, "Matrix type must have at least 3 columns and rows");
    return
        ( m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1) ) * ( m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3) ) -
        ( m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1) ) * ( m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3) ) +
        ( m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1) ) * ( m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3) ) +
        ( m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1) ) * ( m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3) ) -
        ( m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1) ) * ( m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3) ) +
        ( m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1) ) * ( m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3) );
}

/**
Computes the inverse of the specified left-upper 2x2 matrix.
\see Determinant2x2
*/
template < template <typename> class MOut, template <typename> class MIn, typename T >
bool InverseMatrix2x2(MOut<T>& out, const MIn<T>& in)
{
    static_assert(MOut<T>::num >= 2 && MIn<T>::num >= 2, "Both matrix types must have at least 2 columns and rows");

    /* Get the inverse determinant */
    T d = Determinant2x2(in);

    if (d == T(0))
        return false;

    d = T(1) / d;

    /* Compute inverse 2x2 matrix */
    auto& inv = out;
    const auto& m = in;

    inv(0, 0) = d * (  m(1, 1) );
    inv(0, 1) = d * ( -m(0, 1) );
    inv(1, 0) = d * ( -m(1, 0) );
    inv(1, 1) = d * (  m(0, 0) );

    return true;
}

/**
Computes the inverse of the specified left-upper 3x3 matrix.
\see Determinant3x3
*/
template < template <typename> class MOut, template <typename> class MIn, typename T >
bool InverseMatrix3x3(MOut<T>& out, const MIn<T>& in)
{
    static_assert(MOut<T>::num >= 3 && MIn<T>::num >= 3, "Both matrix types must have at least 3 columns and rows");

    /* Get the inverse determinant */
    T d = Determinant3x3(in);

    if (d == T(0))
        return false;

    d = T(1) / d;

    /* Compute inverse 3x3 matrix */
    auto& inv = out;
    const auto& m = in;

    inv(0, 0) = d * ( m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2) );
    inv(1, 0) = d * ( m(2, 0) * m(1, 2) - m(1, 0) * m(2, 2) );
    inv(2, 0) = d * ( m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1) );
    inv(0, 1) = d * ( m(1, 1) * m(0, 2) - m(0, 1) * m(2, 2) );
    inv(1, 1) = d * ( m(0, 0) * m(2, 2) - m(2, 0) * m(0, 2) );
    inv(2, 1) = d * ( m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1) );
    inv(0, 2) = d * ( m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2) );
    inv(1, 2) = d * ( m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2) );
    inv(2, 2) = d * ( m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1) );

    return true;
}

/**
Computes the inverse of the specified left-upper 4x4 matrix.
\see Determinant4x4
*/
template < template <typename> class MOut, template <typename> class MIn, typename T >
bool InverseMatrix4x4(MOut<T>& out, const MIn<T>& in)
{
    static_assert(MOut<T>::num >= 4 && MIn<T>::num >= 4, "Both matrix types must have at least 4 columns and rows");

    /* Get the inverse determinant */
    T d = Determinant4x4(in);

    if (d == T(0))
        return false;

    d = T(1) / d;

    /* Compute inverse 4x4 matrix */
    auto& inv = out;
    const auto& m = in;

    inv(0, 0) = d * ( m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)) );
    inv(1, 0) = d * ( m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)) );
    inv(2, 0) = d * ( m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) );
    inv(3, 0) = d * ( m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)) );
    inv(0, 1) = d * ( m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)) );
    inv(1, 1) = d * ( m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)) );
    inv(2, 1) = d * ( m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)) );
    inv(3, 1) = d * ( m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)) );
    inv(0, 2) = d * ( m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)) );
    inv(1, 2) = d * ( m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)) );
    inv(2, 2) = d * ( m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)) );
    inv(3, 2) = d * ( m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)) );
    inv(0, 3) = d * ( m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)) );
    inv(1, 3) = d * ( m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)) );
    inv(2, 3) = d * ( m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)) );
    inv(3, 3) = d * ( m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)) );

    return true;
}

#if 0

/**
Computes the inverse of the specified left-upper 4x3 matrix.
\remarks This only works for matrices where the last row is { 0, 0, 0, 1 }.
The inversion is computed as follows:
\code
// A = [ M b ]
//     [ 0 1 ]
// inv(A) = [ inv(M) -inv(M) * b ]
//          [   0          1     ]
\endcode
Therefore the computation intensive inversion is reduced to a 3x3 matrix.
\remarks Only the left-upper 4x3 matrix will be written to 'out'.
Therefore the matrix type must have at least 4 columns and 3 rows. This can be Matrix4.
\see InverseMatrix4x4
*/
template < template <typename> class MOut, template <typename> class MIn, typename T >
bool InverseAffineMatrix4x3(MOut<T>& out, const MIn<T>& in)
{
    static_assert(MOut<T>::num >= 4 && MIn<T>::num >= 4, "Both matrix types must have at least 4 columns and 3 rows");
    /* Compute inverse of the left-upper 3x3 matrix */
    if (InverseMatrix3x3(out, in))
    {
        /* Get references for better equation overview */
        const auto& x = in(3, 0);
        const auto& y = in(3, 1);
        const auto& z = in(3, 2);

        /* Compute inverse of last column [ -inv(M) * b ] */
        out(3, 0) = -( out(0, 0)*x + out(1, 0)*y + out(2, 0)*z );
        out(3, 1) = -( out(0, 1)*x + out(1, 1)*y + out(2, 1)*z );
        out(3, 2) = -( out(0, 2)*x + out(1, 2)*y + out(2, 2)*z );

        return true;
    }
    return false;
}

#endif

/**
Compares the two specified matrices a and b for equalilty.
\tparam M Specifies the matrix type. This class needs a static constant member called "num"
holding the number of matrix rows and columns (e.g. Matrix2, Matrix3 or Matrix4).
\tparam T Specifies the data type. This can be floating point type (e.g. float or double) or
an integer type (e.g. int, unsigned int etc.).
\param[in] a Specifies the first matrix.
\param[in] b Specifies the second matrix.
\return True if the two matrices a and b are equal.
\note This function is only used as base function for the matrix comparision operators (for Matrix2, Matrix3 and Matrix4).
\see Math::Equal
*/
template < template <typename> class M, typename T > inline bool CompareMatEqual(const M<T>& a, const M<T>& b)
{
    for (size_t i = 0, n = M<T>::num * M<T>::num; i < n; ++i)
    {
        if (!Math::Equal<T>(a[i], b[i]))
            return false;
    }
    return true;
}

//! \see Math::CompareMatEqual
template < template <typename> class M, typename T > inline bool CompareMatNotEqual(const M<T>& a, const M<T>& b)
{
    return !CompareMatEqual(a, b);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================