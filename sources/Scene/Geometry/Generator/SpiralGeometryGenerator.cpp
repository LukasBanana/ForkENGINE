/*
 * Spiral geometry generator header
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


SpiralDescription::SpiralDescription(
    float ringRadius, float innerRadiusU, float innerRadiusV,
    float offset, float rotation, unsigned int segmentsU, unsigned int segmentsV) :
    ringRadius  { ringRadius   },
    innerRadiusU{ innerRadiusU },
    innerRadiusV{ innerRadiusV },
    offset      { offset       },
    rotation    { rotation     },
    segmentsU   { segmentsU    },
    segmentsV   { segmentsV    }
{
}
SpiralDescription::SpiralDescription(
    float ringRadius, float innerRadius, float offset, float rotation, unsigned int segments) :
    SpiralDescription(ringRadius, innerRadius, innerRadius, offset, rotation, segments, segments)
{
}

bool SpiralDescription::CompareSWO(const SpiralDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( ringRadius   )
    RETURN_COMPARE_MEMBER_SWO( innerRadiusU )
    RETURN_COMPARE_MEMBER_SWO( innerRadiusV )
    RETURN_COMPARE_MEMBER_SWO( offset       )
    RETURN_COMPARE_MEMBER_SWO( rotation     )
    RETURN_COMPARE_MEMBER_SWO( segmentsU    )
    return segmentsV < other.segmentsV;
}


FORK_EXPORT void GenerateSpiral(GeometryType& geometry, const SpiralDescription& desc)
{
    //todo ...
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================