/*
 * Newton physics body file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwBody.h"
#include "../NwMaterial.h"
#include "../NwCore.h"
#include "Core/CastToConcreteObject.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/PointerConversionException.h"


namespace Fork
{

namespace Physics
{


NwBody::NwBody(NewtonBody* body) :
    body_{ body }
{
    ASSERT_POINTER(body);
    NewtonBodySetUserData(body_, ThisBodyPtr());
}
NwBody::~NwBody()
{
    NewtonDestroyBody(body_);
}

void NwBody::SetupTransform(const Scene::Transform& transform)
{
    /* Setup matrix for physics body */
    auto matrix = transform.NonScaledMatrix();
    NewtonBodySetMatrix(body_, matrix.Ptr());

    /* Setup scale for collision */
    auto scale = transform.GetScale();
    NewtonBodySetCollisionScale(body_, scale.x, scale.y, scale.z);
}

Math::Matrix4f NwBody::Transform() const
{
    Math::Matrix4f matrix;
    NewtonBodyGetMatrix(body_, matrix.Ptr());
    return matrix;
}

void NwBody::SetupMaterial(const MaterialPtr& material)
{
    /* Get newton material */
    auto materialNw = CAST_TO_NW_OBJECT(Material, material.get());

    /* Setup body material */
    material_ = material;
    NewtonBodySetMaterialGroupID(GetBody(), materialNw->GetGroupID());
}


} // /namespace Physics

} // /namespace Fork



// ========================