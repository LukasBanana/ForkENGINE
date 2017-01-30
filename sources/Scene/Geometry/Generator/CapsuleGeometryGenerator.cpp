/*
 * Capsule geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Core/AuxiliaryMacros.h"


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


CapsuleDescription::CapsuleDescription(
    float radius, float height, unsigned int segmentsU, unsigned int segmentsV) :
    radius      { radius    },
    height      { height    },
    segmentsU   { segmentsU },
    segmentsV   { segmentsV }
{
}
CapsuleDescription::CapsuleDescription(
    float radius, float height, unsigned int segments) :
    CapsuleDescription(radius, height, segments, segments)
{
}

bool CapsuleDescription::CompareSWO(const CapsuleDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( radius    )
    RETURN_COMPARE_MEMBER_SWO( height    )
    RETURN_COMPARE_MEMBER_SWO( segmentsU )
    return segmentsV < other.segmentsV;
}


FORK_EXPORT void GenerateCapsule(GeometryType& geometry, const CapsuleDescription& desc)
{
    //todo ...
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================