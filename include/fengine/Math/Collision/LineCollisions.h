/*
 * Line collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_LINE_COLLISIONS_H__
#define __FORK_MATH_LINE_COLLISIONS_H__


#include "Math/Geometry/Line.h"
#include "Math/Geometry/LineRelations.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Math
{


//! Computes the point onto the line with the nearest distance between the specified line and point.
template < template <typename> class V, typename T >
V<T> ComputeClosestPointToLine(const Line<V, T>& line, const V<T>& point)
{
    auto pos = point - line.start;
    auto dir = line.Direction();

    auto len = dir.Length();
    dir *= (T(1) / len);

    auto factor = Dot(dir, pos);

    if (factor < T(0))
        return line.start;
    if (factor > len)
        return line.end;

    dir *= factor;

    return line.start + dir;
}

//! Computes the distance between the specified line and point.
template < template <typename> class V, typename T >
inline T ComputeDistanceToLine(const Line<V, T>& line, const V<T>& point)
{
    return Math::Distance(ComputeClosestPointToLine(line, point), point);
}

/**
Computes the closest line segment between the two specified lines (these are also line segments).
The start point of the resulting segment will lie onto the first line 'lineA' and
the end point of the resulting segment will lie onto the second line 'lineB'.
*/
template < template <typename> class V, typename T >
Line<V, T> ComputeClosestSegmentBetweenLines(const Line<V, T>& lineA, const Line<V, T>& lineB)
{
    V<T> dir1   = lineA.Direction();
    V<T> dir2   = lineB.Direction();
    V<T> r      = (lineA.start - lineB.start);
    
    /* Compute squared length of segment A and B */
    T a = dir1.LengthSq();
    T e = dir2.LengthSq();
    T f = Dot(dir2, r);
    
    /* Check if either or both segments degenerate into points */
    if (a <= Epsilon<T>() && e <= Epsilon<T>())
    {
        /* Both segments degenerate into points */
        return Line<V, T>(lineA.start, lineB.start);
    }
    
    T s, t;
    
    if (a <= Epsilon<T>())
    {
        /* First segment degenerates into a point */
        s = T(0);
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = Saturate(t);
    }
    else
    {
        T c = Dot(dir1, r);
        
        if (e <= Epsilon<T>())
        {
            /* Second segment degenerates into a point */
            t = T(0);
            s = -c / a;
            s = Saturate(s); // t = 0 => s = (b*t - c) / a = -c / a
        }
        else
        {
            /* The general nondegenerate case starts here */
            T b = Dot(dir1, dir2);
            T denom = a*e - b*b;
            
            /*
            If segments not parallel, compute closest point on L1 to L2, and
            clamp to segment S1. Else pick arbitrary s (here 0)
            */
            if (denom != T(0))
            {
                s = (b*f - c*e) / denom;
                s = Saturate(s);
            }
            else
                s = T(0);
            
            /*
            Compute point on L2 closest to S1(s) using
            t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
            */
            t = (b*s + f) / e;
            
            /*
            If t in [0,1] done. Else clamp t, recompute s for the new value
            of t using s = Dot((P2 + D2*t) - P1, D1) / Dot(D1, D1) = (t*b - c) / a
            and clamp s to [0, 1]
            */
            if (t < T(0))
            {
                t = T(0);
                s = -c / a;
                s = Saturate(s);
            }
            else if (t > T(1))
            {
                t = T(1);
                s = (b - c) / a;
                s = Saturate(s);
            }
        }
    }
    
    /* Return final line segment */
    return Line<V, T>(
        lineA.start + dir1 * s,
        lineB.start + dir2 * t
    );
}

/**
Computes the relation between line and point.
\param[in] line Specifies the line to which the relation is to be computed.
\param[in] point Specifies the point which is to be tested against the line.
\param[out] closestPoint Specifies the resulting closest point (which is a side effect of the relation computation).
If you only need the closest point, use "ComputeClosestPointToLine".
\see ComputeClosestPointToLine
\see LinePointRelations
*/
template < template <typename> class V, typename T >
LinePointRelations ComputeRleationToLine(const Line<V, T>& line, const V<T>& point, V<T>& closestPoint)
{
    auto pos = point - line.start;
    auto dir = line.Direction();

    auto len = dir.Length();
    dir *= (T(1) / len);

    auto factor = Dot(dir, pos);

    if (factor < T(0))
    {
        closestPoint = line.start;
        return LinePointRelations::Start;
    }
    if (factor > len)
    {
        closestPoint = line.end;
        return LinePointRelations::End;
    }

    dir *= factor;

    closestPoint = line.start + dir;
    return LinePointRelations::Onto;
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================