/*
 * Core header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CORE_H__
#define __FORK_CORE_H__


/* --- Static config header file --- */

#include "Core/StaticConfig.h"


/* --- Core header files --- */

#include "Core/Version.h"
#include "Core/StringModifier.h"
#include "Core/SDKGuard.h"
#include "Core/Container/StrideBuffer.h"
#include "Core/Container/MementoHierarchy.h"
#include "Core/TreeHierarchy/KDTreeNode.h"
#include "Core/CiString.h"
#include "Core/DefaultValue.h"


/* --- Base data type classes --- */

#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"

#include "Math/Core/Matrix2.h"
#include "Math/Core/Matrix3.h"
#include "Math/Core/Matrix4.h"

#include "Math/Core/Quaternion.h"

#include "Math/Core/Range.h"
#include "Math/Core/Rect.h"
#include "Math/Core/Cuboid.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Size3.h"

#include "Math/Core/Transform3D.h"
#include "Math/Core/Transform2D.h"

#include "Math/Core/Angle.h"
#include "Math/Core/Equal.h"
#include "Math/Core/Zero.h"

#include "Math/Core/Arithmetic/BaseArithmetic.h"


/* --- Geometry classes --- */

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


/* --- Common math --- */

#include "Math/Common/ExtMathFunctions.h"
#include "Math/Common/AprxMathFunctions.h"
#include "Math/Common/Spline.h"
#include "Math/Common/StaticBezier.h"
#include "Math/Common/DynamicBezier.h"
#include "Math/Common/Transform.h"
#include "Math/Common/Convert.h"
#include "Math/Common/RasterNumber.h"
#include "Math/Common/CoordinateSpace.h"


/* --- Collision math --- */

#include "Math/Collision/RayCollisions.h"
#include "Math/Collision/LineCollisions.h"
#include "Math/Collision/PlaneCollisions.h"
#include "Math/Collision/TriangleCollisions.h"
#include "Math/Collision/AABBCollisions.h"
#include "Math/Collision/CylinderCollisions.h"


/* --- IO --- */

#include "IO/Core/Log.h"
#include "IO/Core/Console.h"
#include "IO/Core/Printer.h"
#include "IO/Core/Variant.h"
#include "IO/Core/Filename.h"
#include "IO/Core/SystemClock.h"
#include "IO/Core/ScopedTimer.h"
#include "IO/Core/ScopedLogTimer.h"
#include "IO/Core/ScopedStringTimer.h"

#include "IO/Crypto/CryptoBitKey.h"
#include "IO/Crypto/DecryptionException.h"
#include "IO/Crypto/EncryptionException.h"

#include "IO/FileSystem/PhysicalFile.h"
#include "IO/FileSystem/VirtualFile.h"
#include "IO/FileSystem/LogFile.h"

#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"


/* --- Lang --- */

#include "Lang/AbstractSyntaxTrees/Visitor.h"
#include "Lang/FSCInterpreter/FSCInterpreter.h"
#include "Lang/XMLParser/XMLParser.h"
#include "Lang/XMLParser/XMLWriter.h"


/* --- Platform --- */

#include "Platform/Core/SystemInfo.h"
#include "Platform/Core/Desktop.h"
#include "Platform/Core/Cursor.h"
#include "Platform/Core/Frame.h"
#include "Platform/Core/VideoModeEnumerator.h"
#include "Platform/Core/Clipboard.h"


/* --- Video --- */

#include "Video/Core/Projection.h"
#include "Video/Core/Scissor.h"
#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Core/BitwiseColor.h"
#include "Video/Core/Spaces.h"

#include "Video/Image/ImageConverter.h"



#endif



// ========================