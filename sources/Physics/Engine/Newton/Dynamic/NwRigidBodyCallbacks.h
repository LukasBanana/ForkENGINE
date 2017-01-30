/*
 * Newton physics rigid body callbacks header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_RIGID_BODY_CALLBACKS_H__
#define __FORK_NW_PHYSICS_RIGID_BODY_CALLBACKS_H__


#include <Newton.h>


namespace Fork
{

namespace Physics
{


void NwApplyForceAndTorqueCallback(const NewtonBody* const body, dFloat timeStep, int threadIndex);
void NwSetTransformCallback(const NewtonBody* const body, const dFloat* const matrix, int threadIndex);


} // /namespace Physics

} // /namespace Fork


#endif



// ========================