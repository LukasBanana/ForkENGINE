/*
 * Matrix 4x4 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATRIX4x4_H__
#define __FORK_MATRIX4x4_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Arithmetic/MatrixArithmetic.h"
#include "Arithmetic/VectorArithmetic.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/OBB.h"
#include "Core/Exception/TransformException.h"
#include "Core/StaticConfig.h"

#include <cmath>
#include <algorithm>


namespace Fork
{

namespace Math
{


/**
Matrix 4x4 base class with 16 entries. This is a column major matrix.
\code
// / 0 4  8 12 \
// | 1 5  9 13 |
// | 2 6 10 14 |
// \ 3 7 11 15 /
\endcode
\note OpenGL uses column major matrices (like it's typical in mathematical notations), but Direct3D uses row major matrices.
\ingroup math_core
*/
template <typename T> class Matrix4
{
    
    public:
        
        //! Number of columns: 4.
        static const size_t num = 4;

        //! Default constructor of the 4x4 matrix. This will initialize the matrix to its identity.
        Matrix4()
        {
            col[0] = { 1, 0, 0, 0 };
            col[1] = { 0, 1, 0, 0 };
            col[2] = { 0, 0, 1, 0 };
            col[3] = { 0, 0, 0, 1 };
        }
        Matrix4(const Vector4<T>& col0, const Vector4<T>& col1, const Vector4<T>& col2, const Vector4<T>& col3) :
        {
            col[0] = col0;
            col[1] = col1;
            col[2] = col2;
            col[3] = col3;
        }
        /**
        Initializes the 4x4 matrix with the specified entries.
        'c' specifies the column and 'r' specifies the row.
        \code
        Math::Matrix4f matrix
        {
            c0r0, c1r0, c2r0, c3r0, // row 0
            c0r1, c1r1, c2r1, c3r1, // row 1
            c0r2, c1r2, c2r2, c3r2, // row 2
            c0r3, c1r3, c2r3, c3r3  // row 3
        };
        \endcode
        */
        Matrix4(
            const T& c0r0, const T& c1r0, const T& c2r0, const T& c3r0,
            const T& c0r1, const T& c1r1, const T& c2r1, const T& c3r1,
            const T& c0r2, const T& c1r2, const T& c2r2, const T& c3r2,
            const T& c0r3, const T& c1r3, const T& c2r3, const T& c3r3)
        {
            col[0].x = c0r0; col[1].x = c1r0; col[2].x = c2r0; col[3].x = c3r0;
            col[0].y = c0r1; col[1].y = c1r1; col[2].y = c2r1; col[3].y = c3r1;
            col[0].z = c0r2; col[1].z = c1r2; col[2].z = c2r2; col[3].z = c3r2;
            col[0].w = c0r3; col[1].w = c1r3; col[2].w = c2r3; col[3].w = c3r3;
        }
        explicit Matrix4(const Matrix3<T>& other)
        {
            const auto& m = other;
            col[0] = { m[0], m[1], m[2], 0 };
            col[1] = { m[3], m[4], m[5], 0 };
            col[2] = { m[6], m[7], m[8], 0 };
            col[3] = {    0,    0,    0, 1 };
        }
        explicit Matrix4(const OBB<Vector3, T>& box)
        {
            col[0].x = box.axis.x.x * box.halfSize.x;
            col[0].y = box.axis.x.y * box.halfSize.x;
            col[0].z = box.axis.x.z * box.halfSize.x;
            col[0].w = 0;

            col[1].x = box.axis.y.x * box.halfSize.y;
            col[1].y = box.axis.y.y * box.halfSize.y;
            col[1].z = box.axis.y.z * box.halfSize.y;
            col[1].w = 0;

            col[2].x = box.axis.z.x * box.halfSize.z;
            col[2].y = box.axis.z.y * box.halfSize.z;
            col[2].z = box.axis.z.z * box.halfSize.z;
            col[2].w = 0;

            col[3].x = box.center.x;
            col[3].y = box.center.y;
            col[3].z = box.center.z;
            col[3].w = 1;
        }

        /* === Operators === */

        Matrix4<T>& operator += (const Matrix4<T>& other)
        {
            Math::Add(*this, *this, other);
            return *this;
        }
        Matrix4<T>& operator -= (const Matrix4<T>& other)
        {
            Math::Sub(*this, *this, other);
            return *this;
        }
        Matrix4<T>& operator *= (const Matrix4<T>& other)
        {
            auto prev = *this;
            Math::Mul(*this, prev, other);
            return *this;
        }

        Matrix4<T>& operator *= (const T& scalar)
        {
            auto prev = *this;
            Math::Mul(*this, prev, scalar);
            return *this;
        }

        Matrix4<T>& operator /= (const T& scalar)
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

        //! Multiplys the specified vector with this matrix. This performs a vector rotation and translation.
        Vector2<T> operator * (const Vector2<T>& vec) const
        {
            return Vector2<T>(
                vec.x*col[0].x + vec.y*col[1].x + col[3].x,
                vec.x*col[0].y + vec.y*col[1].y + col[3].y
            );
        }

        //! Multiplys the specified vector with this matrix. This performs a vector rotation and translation.
        Vector3<T> operator * (const Vector3<T>& vec) const
        {
            return Vector3<T>(
                vec.x*col[0].x + vec.y*col[1].x + vec.z*col[2].x + col[3].x,
                vec.x*col[0].y + vec.y*col[1].y + vec.z*col[2].y + col[3].y,
                vec.x*col[0].z + vec.y*col[1].z + vec.z*col[2].z + col[3].z
            );
        }

        //! Multiplys the specified vector with this matrix. This performs a vector rotation and translation.
        Vector4<T> operator * (const Vector4<T>& vec) const
        {
            return Vector4<T>(
                vec.x*col[0].x + vec.y*col[1].x + vec.z*col[2].x + vec.w*col[3].x,
                vec.x*col[0].y + vec.y*col[1].y + vec.z*col[2].y + vec.w*col[3].y,
                vec.x*col[0].z + vec.y*col[1].z + vec.z*col[2].z + vec.w*col[3].z,
                vec.x*col[0].w + vec.y*col[1].w + vec.z*col[2].w + vec.w*col[3].w
            );
        }

        /* === Functions === */

        Vector2<T> RotateVector(const Vector2<T>& vec) const
        {
            return Vector2<T>(
                vec.x*col[0].x + vec.y*col[1].x,
                vec.x*col[0].y + vec.y*col[1].y
            );
        }

        Vector3<T> RotateVector(const Vector3<T>& vec) const
        {
            return Vector3<T>(
                vec.x*col[0].x + vec.y*col[1].x + vec.z*col[2].x,
                vec.x*col[0].y + vec.y*col[1].y + vec.z*col[2].y,
                vec.x*col[0].z + vec.y*col[1].z + vec.z*col[2].z
            );
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
        \see Determinant4x4
        */
        T Determinant() const
        {
            return Determinant4x4(*this);
        }

        /**
        Computes the inverse matrix.
        \param[out] inv Specifies the resulting inverse matrix.
        This must not be the same instance as this matrix. Otherwise the behaviour is undefined!
        \remarks A faster method to inverse an affine matrix is "InverseAffine".
        \see InverseMatrix4x4
        \see InverseAffine
        */
        bool Inverse(Matrix4<T>& inv) const
        {
            return InverseMatrix4x4(inv, *this);
        }

        /**
        Makes this matrix to an inversed matrix.
        \remarks A faster method to inverse an affine matrix is "MakeInverseAffine".
        \see Inverse
        \see MakeInverseAffine
        */
        bool MakeInverse()
        {
            Matrix4<T> inv;
            if (Inverse(inv))
            {
                *this = inv;
                return true;
            }
            return false;
        }

        /**
        Returns the inversed matrix.
        \see MakeInverse
        */
        Matrix4<T> Inverse() const
        {
            auto inv = *this;
            inv.MakeInverse();
            return inv;
        }

        #if 0

        /**
        Computes the inverse affine matrix.
        \remarks This is a faster method than "Inverse", but it only works for matrices
        where the last row is { 0, 0, 0, 1 }. This is the case for usual world transformations.
        \throws TransformException (Only in debug mode) when this is not an affine matrix.
        \see InverseAffineMatrix4x3
        \see EnsureAffine
        */
        bool InverseAffine(Matrix4<T>& inv)
        {
            #ifdef FORK_DEBUG
            EnsureAffine();
            #endif
            return InverseAffineMatrix4x3(inv, *this);
        }

        /**
        Makes this affine matrix to an inversed matrix.
        \see InverseAffine
        */
        bool MakeInverseAffine()
        {
            Matrix4<T> inv;
            if (InverseAffine(inv))
            {
                *this = inv;
                return true;
            }
            return false;
        }

        /**
        Returns the inversed affine matrix.
        \see MakeInverseAffine
        */
        Matrix4<T> InverseAffine() const
        {
            auto inv = *this;
            inv.MakeInverseAffine();
            return inv;
        }

        #endif

        inline void Transpose(Matrix4<T>& trans)
        {
            Math::Transpose(trans, *this);
        }

        void MakeTranspose()
        {
            Matrix4<T> trans;
            Transpose(trans);
            *this = trans;
        }

        Matrix4<T> Transpose() const
        {
            auto trans = *this;
            trans.MakeTranspose();
            return trans;
        }

        inline void MakeIdentity()
        {
            Math::MakeIdentity(*this);
        }

        /**
        \code
        // / a e i m \   / 1 0 0 x \   / a e i (ax+ey+iz+m) \
        // | b f j n |   | 0 1 0 y |   | b f j (bx+fy+jz+n) |
        // | c g k o | x | 0 0 1 z | = | c g k (cx+gy+kz+o) |
        // \ d h l p /   \ 0 0 0 1 /   \ d h l (dx+hy+lz+p) /
        \endcode
        */
        Matrix4<T>& Translate(const Vector3<T>& vec)
        {
            col[3].x += ( col[0].x*vec.x + col[1].x*vec.y + col[2].x*vec.z );
            col[3].y += ( col[0].y*vec.x + col[1].y*vec.y + col[2].y*vec.z );
            col[3].z += ( col[0].z*vec.x + col[1].z*vec.y + col[2].z*vec.z );
            col[3].w += ( col[0].w*vec.x + col[1].w*vec.y + col[2].w*vec.z );
            return *this;
        }

        /**
        \code
        // / a e i m \   / x 0 0 0 \   / ax ey iz m \
        // | b f j n |   | 0 y 0 0 |   | bx fy jz n |
        // | c g k o | x | 0 0 z 0 | = | cx gy kz o |
        // \ d h l p /   \ 0 0 0 1 /   \ dx hy lz p /
        \endcode
        */
        Matrix4<T>& Scale(const Vector3<T>& vec)
        {
            col[0].x *= vec.x; col[0].y *= vec.x; col[0].z *= vec.x; col[0].w *= vec.x;
            col[1].x *= vec.y; col[1].y *= vec.y; col[1].z *= vec.y; col[1].w *= vec.y;
            col[2].x *= vec.z; col[2].y *= vec.z; col[2].z *= vec.z; col[2].w *= vec.z;
            return *this;
        }

        /**
        Performs an arbitrary rotation.
        \param[in] angle Specifies the rotation angle (in radian).
        \param[in] axis Specifies the rotation axis.
        */
        Matrix4<T>& RotateFree(const T& angle, const Vector3<T>& axis)
        {
            Math::RotateFree(*this, angle, axis);
            return *this;
        }
        
        /**
        \code
        // / 1 0  0 0 \
        // | 0 c -s 0 |
        // | 0 s  c 0 |
        // \ 0 0  0 1 /
        \endcode
        */
        Matrix4<T>& RotateX(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);
            
            /* Temporaries */
            const auto col1 = col[1];
            
            /* Rotation */
            col[1].x = col[1].x*c + col[2].x*s;
            col[1].y = col[1].y*c + col[2].y*s;
            col[1].z = col[1].z*c + col[2].z*s;
            col[1].w = col[1].w*c + col[2].w*s;
            
            col[2].x = col[2].x*c - col1.x*s;
            col[2].y = col[2].y*c - col1.y*s;
            col[2].z = col[2].z*c - col1.z*s;
            col[2].w = col[2].w*c - col1.w*s;
            
            return *this;
        }
        
        /**
        \code
        // /  c 0 s 0 \
        // |  0 1 0 0 |
        // | -s 0 c 0 |
        // \  0 0 0 1 /
        \endcode
        */
        Matrix4<T>& RotateY(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);
            
            /* Temporaries */
            const auto col0 = col[0];
            
            /* Rotation */
            col[0].x = col[0].x*c - col[2].x*s;
            col[0].y = col[0].y*c - col[2].y*s;
            col[0].z = col[0].z*c - col[2].z*s;
            col[0].w = col[0].w*c - col[2].w*s;
            
            col[2].x = col0.x*s + col[2].x*c;
            col[2].y = col0.y*s + col[2].y*c;
            col[2].z = col0.z*s + col[2].z*c;
            col[2].w = col0.w*s + col[2].w*c;
            
            return *this;
        }
        
        /**
        \code
        // / c -s 0 0 \
        // | s  c 0 0 |
        // | 0  0 1 0 |
        // \ 0  0 0 1 /
        \endcode
        */
        Matrix4<T>& RotateZ(const T& angle)
        {
            const T c = std::cos(angle);
            const T s = std::sin(angle);

            /* Temporaries */
            const auto col0 = col[0];

            /* Rotation */
            col[0].x = col[0].x*c + col[1].x*s;
            col[0].y = col[0].y*c + col[1].y*s;
            col[0].z = col[0].z*c + col[1].z*s;
            col[0].w = col[0].w*c + col[1].w*s;

            col[1].x = col[1].x*c - col0.x*s;
            col[1].y = col[1].y*c - col0.y*s;
            col[1].z = col[1].z*c - col0.z*s;
            col[1].w = col[1].w*c - col0.w*s;

            return *this;
        }

        Vector4<T> GetRow(const size_t index) const
        {
            switch (index)
            {
                case 0: return Vector4<T>(col[0].x, col[1].x, col[2].x, col[3].x);
                case 1: return Vector4<T>(col[0].y, col[1].y, col[2].y, col[3].y);
                case 2: return Vector4<T>(col[0].z, col[1].z, col[2].z, col[3].z);
                case 3: return Vector4<T>(col[0].w, col[1].w, col[2].w, col[3].w);
            }
            return Vector4<T>();
        }
        
        void SetRow(const size_t index, const Vector4<T>& vec)
        {
            switch (index)
            {
                case 0: col[0].x = vec.x, col[1].x = vec.y, col[2].x = vec.z, col[3].x = vec.w; break;
                case 1: col[0].y = vec.x, col[1].y = vec.y, col[2].y = vec.z, col[3].y = vec.w; break;
                case 2: col[0].z = vec.x, col[1].z = vec.y, col[2].z = vec.z, col[3].z = vec.w; break;
                case 3: col[0].w = vec.x, col[1].w = vec.y, col[2].w = vec.z, col[3].w = vec.w; break;
            }
            return Vector4<T>();
        }

        inline const Vector4<T>& GetColumn(const size_t index) const
        {
            return col[index];
        }

        inline Vector4<T>& GetColumn(const size_t index)
        {
            return col[index];
        }

        inline void SetColumn(const size_t index, const Vector4<T>& vec)
        {
            col[index] = vec;
        }

        /**
        Returns the matrix position. This consists of the matrix entries
        [12], [13] and [14] or rather (3, 0), (3, 1) and (3, 2).
        */
        inline Point3<T> GetPosition() const
        {
            return col[3].Vec3();
        }

        /**
        Set the position of the matrix, i.e. the x, y and z component of the 4th matrix column.
        \param[in] pos Specifies the position 3D vector.
        \see GetPosition
        */
        inline void SetPosition(const Point3<T>& pos)
        {
            col[3].x = pos.x;
            col[3].y = pos.y;
            col[3].z = pos.z;
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

        /**
        Returns the rotation of thsi left-upper 3x3 sub-matrix.
        \note If this matrix is scaled, the result is undefined!
        \see SetScale
        */
        inline Quaternion<T> GetRotation() const
        {
            return Quaternion<T>(*this);
        }

        /**
        Sets the left-upper 3x3 sub-matrix rotation.
        \param[in] rot Specifies the rotation quaternion.
        \note This will overwrite the matrix scaling!
        */
        void SetRotation(const Quaternion<T>& rot)
        {
            const auto m = rot.Mat3();

            col[0].x = m[0];
            col[0].y = m[1];
            col[0].z = m[2];

            col[1].x = m[3];
            col[1].y = m[4];
            col[1].z = m[5];

            col[2].x = m[6];
            col[2].y = m[7];
            col[2].z = m[8];
        }

        /**
        Returns the euler rotation of the left-upper 3x3 sub-matrix.
        \remarks This is equivalent to: 'GetRotation().GetEulerRotation()'.
        \see Quaternion::GetEulerRotation
        */
        inline Vector3<T> GetEulerRotation() const
        {
            return GetRotation().GetEulerRotation();
        }

        //! Sets the matrix rotation with the sepcified euler angles (in raidans).
        void SetEulerRotation(const Vector3<T>& rot)
        {
            const T cx = std::cos(rot.x);
            const T sx = std::sin(rot.x);
            const T cy = std::cos(rot.y);
            const T sy = std::sin(rot.y);
            const T cz = std::cos(rot.z);
            const T sz = std::sin(rot.z);
            
            const T sxy = sx*sy;
            const T cxy = cx*sy;
            
            m(0, 0) = cy*cz;
            m(0, 1) = cy*sz;
            m(0, 2) = -sy;
            
            m(1, 0) = sxy*cz - cx*sz;
            m(1, 1) = sxy*sz + cx*cz;
            m(1, 2) = sx*cy;
            
            m(2, 0) = cxy*cz + sx*sz;
            m(2, 1) = cxy*sz - sx*cz;
            m(2, 2) = cx*cy;
        }

        /**
        Flips the Y axis of this matrix. This is equivalent to the following matrix multiplication:
        \code
        Math::Matrix4 flipMatrix
        {
            1,  0, 0, 0,
            0, -1, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
        result = flipMatrix * thisMatrix;
        \endcode
        */
        void FlipYAxis()
        {
            /*
            Invert Y-axis in this matrix.
            -> This is equivalent to the following matrix multiplication:
            / 1  0 0 0 \
            | 0 -1 0 0 |
            | 0  0 1 0 | x thisMatrix
            \ 0  0 0 1 /
            */
            col[0].y = -col[0].y;
            col[1].y = -col[1].y;
            col[2].y = -col[2].y;
            col[3].y = -col[3].y;
        }
        /**
        Flips the Z axis of this matrix. This is equivalent to the following matrix multiplication:
        \code
        Math::Matrix4 flipMatrix
        {
            1, 0,  0, 0,
            0, 1,  0, 0,
            0, 0, -1, 0,
            0, 0,  0, 1
        };
        result = flipMatrix * thisMatrix;
        \endcode
        */
        void FlipZAxis()
        {
            /*
            Invert Z-axis in this matrix.
            -> This is equivalent to the following matrix multiplication:
            / 1 0  0 0 \
            | 0 1  0 0 |
            | 0 0 -1 0 | x thisMatrix
            \ 0 0  0 1 /
            */
            col[0].z = -col[0].z;
            col[1].z = -col[1].z;
            col[2].z = -col[2].z;
            col[3].z = -col[3].z;
        }

        /**
        Returns true if this is an affine matrix,
        i.e. the last row is { 0, 0, 0, 1 }.
        */
        inline bool IsAffine() const
        {
            return GetRow(3) == Math::Vector4<T>(0, 0, 0, 1);
        }

        /**
        Ensures that this is an affine matrix
        \throws TransformException If this is not an affine matrix.
        \see IsAffine
        */
        void EnsureAffine() const
        {
            if (!IsAffine())
                throw TransformException(__FUNCTION__, "Matrix must be affine");
        }

        //! Returns the 2x2 matrix out of this 4x4 matrix.
        Matrix2<T> Mat2() const
        {
            return Matrix2<T>(col[0].Vec2(), col[1].Vec2());
        }

        //! Returns the 3x3 matrix out of this 4x4 matrix.
        Matrix3<T> Mat3() const
        {
            return Matrix3<T>(col[0].Vec3(), col[1].Vec3(), col[2].Vec3());
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

        template <typename C> Matrix4<C> Cast() const
        {
            return Matrix4<C>(
                col[0].Cast<C>(),
                col[1].Cast<C>(),
                col[2].Cast<C>(),
                col[3].Cast<C>()
            );
        }

        /* === Members === */

        Vector4<T> col[4];

};


DEFAULT_MATH_BASE_OPERATORS(Matrix4)
DEFAULT_SCALAR_OPERATORS(Matrix4)
DEFAULT_MATH_TYPEDEFS(Matrix4)


} // /namespace Math

} // /namespace Fork


#endif



// ========================