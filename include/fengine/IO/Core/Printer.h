/*
 * Printer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_PRINTER_H__
#define __FORK_IO_PRINTER_H__


#include "Core/Export.h"
#include "Core/StringModifier.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix2.h"
#include "Math/Core/Matrix3.h"
#include "Math/Core/Matrix4.h"
#include "Math/Core/Quaternion.h"
#include "Math/Core/Zero.h"
#include "Math/Common/Polynomial.h"
#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"

#include <string>


namespace Fork
{

namespace IO
{

/**
The printer namespace provides global functions to print core classes (like vector and matrix classes)
into a string for debug output.
\note This has nothing to do with a physical printer device!
*/
namespace Printer
{


/**
Prints an NxN-matrix into a string.
\tparam M Specifies the matrix type. This class type must have a public static constant member
named "num" from type 'size_t', which specifies the number of rows and columns (e.g. num = 4 for a 4x4 matrix).
This type must also implement the following operator:
\code
// Returns the elements at the specified column and row.
T operator () (size_t column, size_t row) const;
\endcode
This can be Math::Matrix2, Math::Matrix3 or Math::Matrix4.
\tparam T Specifies the matrix element data type. This is typically float or double.
\param[in] mat Specifies the matrix which is to be printed.
\param[in] precision Specifies the floating-point number precision for printing. By default 'defaultPrecision'.
\return The final 'printed' string.
\remarks Here is a small example:
\code
// Example output of a 3x3 matrix:
IO::Printer::PrintMatrix(Math::Matrix3f()) --> "/ 1 0 0 \\\n| 0 1 0 |\n\\ 0 0 1 /\n";
// Results in:
// / 1 0 0 \
// | 0 1 0 |
// \ 0 0 1 /
\endcode
\see defaultPrecision
*/
template < template <typename> class M, typename T > std::string PrintMatrix(const M<T>& mat, int precision = defaultPrecision)
{
    static const size_t num = M<T>::num;
    
    /* Store all elements as string and find the largest string entry */
    std::string elem[num][num];
    size_t maxLen = 0;

    for (size_t col = 0; col < num; ++col)
    {
        for (size_t row = 0; row < num; ++row)
        {
            elem[col][row] = ToStr(mat(col, row), precision);
            auto len = elem[col][row].size();
            if (maxLen < len)
                maxLen = len;
        }
    }

    /* Print matrix */
    std::string str;

    for (size_t row = 0; row < num; ++row)
    {
        if (row == 0)
            str += "/ ";
        else if (row + 1 == num)
            str += "\\ ";
        else
            str += "| ";

        /* Print current row */
        for (size_t col = 0; col < num; ++col)
        {
            auto len = elem[col][row].size();
            if (len < maxLen)
                str += std::string(maxLen - len, ' ');

            str += elem[col][row];

            if (col + 1 < num)
                str += " ";
        }

        if (row == 0)
            str += " \\";
        else if (row + 1 == num)
            str += " /";
        else
            str += " |";

        if (row + 1 < num)
            str += "\n";
    }

    return str;
}

//! Prints the specified 2D vector into a string, e.g. "( 42 | -3 )".
template <typename T> std::string Print(const Math::Vector2<T>& vec, int precision = defaultPrecision)
{
    return
        "( " + ToStr(vec.x, precision) +
        " | " + ToStr(vec.y, precision) +
        " )";
}

//! Prints the specified 3D vector into a string, e.g. "( 1 | 2 | 3 )".
template <typename T> std::string Print(const Math::Vector3<T>& vec, int precision = defaultPrecision)
{
    return
        "( " + ToStr(vec.x, precision) +
        " | " + ToStr(vec.y, precision) +
        " | " + ToStr(vec.z, precision) +
        " )";
}

//! Prints the specified 4D vector into a string, e.g. "( 0 | 0 | 0 | 1 )".
template <typename T> std::string Print(const Math::Vector4<T>& vec, int precision = defaultPrecision)
{
    return
        "( " + ToStr(vec.x, precision) +
        " | " + ToStr(vec.y, precision) +
        " | " + ToStr(vec.z, precision) +
        " | " + ToStr(vec.w, precision) +
        " )";
}

//! Prints the specified RGB color into a string, e.g. "( 255 | 255 | 0 )".
template <typename T> std::string Print(const Video::ColorRGB<T>& color, int precision = defaultPrecision)
{
    return
        "( " + ToStr(color.r, precision) +
        " | " + ToStr(color.g, precision) +
        " | " + ToStr(color.b, precision) +
        " )";
}

//! Prints the specified RGBA color into a string, e.g. "( 128 | 200 | 60 | 255 )".
template <typename T> std::string Print(const Video::ColorRGBA<T>& color, int precision = defaultPrecision)
{
    return
        "( " + ToStr(color.r, precision) +
        " | " + ToStr(color.g, precision) +
        " | " + ToStr(color.b, precision) +
        " | " + ToStr(color.a, precision) +
        " )";
}

/**
Prints the specified 2x2 matrix into a string.
\see PrintMatrix
*/
template <typename T> inline std::string Print(const Math::Matrix2<T>& mat, int precision = defaultPrecision)
{
    return PrintMatrix(mat, precision);
}

/**
Prints the specified 3x3 matrix into a string.
\see PrintMatrix
*/
template <typename T> inline std::string Print(const Math::Matrix3<T>& mat, int precision = defaultPrecision)
{
    return PrintMatrix(mat, precision);
}

/**
Prints the specified 4x4 matrix into a string.
\see PrintMatrix
*/
template <typename T> inline std::string Print(const Math::Matrix4<T>& mat, int precision = defaultPrecision)
{
    return PrintMatrix(mat, precision);
}

//! Prints the specified quaternion into a string, e.g. "( 0 | 0 | 0 | 1 )".
template <typename T> std::string Print(const Math::Quaternion<T>& quat, int precision = defaultPrecision)
{
    return
        "( " + ToStr(quat.x, precision) +
        " | " + ToStr(quat.y, precision) +
        " | " + ToStr(quat.z, precision) +
        " | " + ToStr(quat.w, precision) +
        " )";
}

/**
Prints the specified polynomial into a string.
\tparam T Specifies the data type. This should be float or double.
\tparam degree Specifies the polynomial degree.
\param[in] polynomial Specifies the polynomial which is to be printed.
\param[in] precision Specifies the floating-point number precision for printing. By default 'defaultPrecision'.
\param[in] param Specifies the parameter name for the polynomial. This is typically "x", "y" or "t". By default "t".
\return The final 'printed' string.
\remarks Here is a small example:
\code
// Example output:
IO::Printer::Print(Math::Polynomial<float, 3>({ 3, -4, 0, 1 })) --> "3t^3 - 4t^2 + 1";
\endcode
\see defaultPrecision
*/
template <typename T, size_t degree> std::string Print(
    const Math::Polynomial<T, degree>& polynomial, int precision = defaultPrecision, const std::string& param = "t")
{
    std::string str;

    for (size_t i = 0; i <= degree; ++i)
    {
        /* Get next coefficient */
        auto deg = degree - i;
        auto coeff = polynomial[deg];

        if (Math::Zero(coeff))
            continue;

        /* Append sign */
        if (coeff < T(0))
        {
            if (i > 0)
                str += " - ";
            else
                str += '-';
        }
        else if (i > 0)
            str += " + ";

        /* Append coefficient (if it's not 1) */
        coeff = std::abs(coeff);
        if (coeff != T(1) || deg == 0)
            str += ToStr(coeff, precision);

        if (deg > 0)
        {
            /* Append parameter (if current degree is > 0) */
            str += param;

            if (deg >= 2)
            {
                /* Append exponent (if degree is >= 2) */
                str += '^';
                str += ToStr(deg);
            }
        }
    }

    return str;
}


} // /namespace Printer

} // /namespace IO

} // /namespace Fork


#endif



// ========================