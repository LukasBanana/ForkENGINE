/*
 * Using header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_USING_H__
#define __FORK_USING_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"

#include "Math/Core/Matrix2.h"
#include "Math/Core/Matrix3.h"
#include "Math/Core/Matrix4.h"

#include "Math/Core/Quaternion.h"

#include "Math/Core/Rect.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Size3.h"

#include "Math/Core/Transform3D.h"
#include "Math/Core/Transform2D.h"

#include "Math/Core/Angle.h"

#include "Math/Geometry/AABB.h"
#include "Math/Geometry/Capsule.h"
#include "Math/Geometry/Cone.h"
#include "Math/Geometry/Cylinder.h"
#include "Math/Geometry/Line.h"
#include "Math/Geometry/OBB.h"
#include "Math/Geometry/Plane.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Sphere.h"
#include "Math/Geometry/Triangle.h"
#include "Math/Geometry/Frustum.h"


namespace Fork
{


/* --- Using all base types --- */

USE_BASE_MATH_CLASS(Vector2)
USE_BASE_MATH_CLASS(Vector3)
USE_BASE_MATH_CLASS(Vector4)

USE_BASE_MATH_CLASS(Point2)
USE_BASE_MATH_CLASS(Point3)
USE_BASE_MATH_CLASS(Point4)

USE_BASE_MATH_CLASS(Matrix2)
USE_BASE_MATH_CLASS(Matrix3)
USE_BASE_MATH_CLASS(Matrix4)

using Math::Quaternion;
using Math::Angle;
using Math::Degree;
using Math::Radian;

USE_BASE_MATH_CLASS(Rect)
USE_BASE_MATH_CLASS(Size2)
USE_BASE_MATH_CLASS(Size3)

USE_BASE_MATH_CLASS(Transform2D)
USE_BASE_MATH_CLASS(Transform3D)

USE_BASE_MATH_CLASS(AABB2)
USE_BASE_MATH_CLASS(AABB3)

USE_BASE_MATH_CLASS(OBB2)
USE_BASE_MATH_CLASS(OBB3)

USE_BASE_MATH_CLASS(Line2)
USE_BASE_MATH_CLASS(Line3)
USE_BASE_MATH_CLASS(Line4)

USE_BASE_MATH_CLASS(Ray2)
USE_BASE_MATH_CLASS(Ray3)
USE_BASE_MATH_CLASS(Ray4)

using Math::Plane;
using Math::Cone;
using Math::Cylinder;
using Math::Capsule;
using Math::ConvexHull;
using Math::Sphere;
using Math::Frustum;


} // /namespace Fork


#endif



// ========================