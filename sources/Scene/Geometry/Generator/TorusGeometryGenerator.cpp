/*
 * Torus geometry generator header
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


TorusDescription::TorusDescription(
    float ringRadius, float innerRadiusU, float innerRadiusV,
    unsigned int segmentsU, unsigned int segmentsV) :
    ringRadius  { ringRadius   },
    innerRadiusU{ innerRadiusU },
    innerRadiusV{ innerRadiusV },
    segmentsU   { segmentsU    },
    segmentsV   { segmentsV    }
{
}
TorusDescription::TorusDescription(
    float ringRadius, float innerRadius, unsigned int segments) :
    TorusDescription(ringRadius, innerRadius, innerRadius, segments, segments)
{
}

bool TorusDescription::CompareSWO(const TorusDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( ringRadius   )
    RETURN_COMPARE_MEMBER_SWO( innerRadiusU )
    RETURN_COMPARE_MEMBER_SWO( innerRadiusV )
    RETURN_COMPARE_MEMBER_SWO( segmentsU    )
    return segmentsV < other.segmentsV;
}


FORK_EXPORT void GenerateTorus(GeometryType& geometry, const TorusDescription& desc)
{
    //todo ...
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================