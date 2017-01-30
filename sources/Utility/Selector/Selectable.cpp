/*
 * Selectable node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Selector/Selectable.h"


namespace Fork
{

namespace Utility
{


Selectable::~Selectable()
{
}

Selectable::TransformFlags::DataType Selectable::TransformFlags() const
{
    return TransformFlags::All;
}

void Selectable::Transform(const TransformCallback& callback, bool isUnique)
{
    if (isUnique)
    {
        OnTransformBegin();
        {
            callback(GetTransform());
            OnTransform();
        }
        OnTransformEnd();
    }
    else
    {
        callback(GetTransform());
        OnTransform();
    }
}

void Selectable::OnTransformBegin()
{
    // dummy
}

void Selectable::OnTransform()
{
    // dummy
}

void Selectable::OnTransformEnd()
{
    // dummy
}

void Selectable::StoreTransform()
{
    prevTransform_ = GetTransform();
}

void Selectable::RestoreTransform()
{
    Transform([&](Scene::Transform& transform){ transform = prevTransform_; });
}


} // /namespace Utility

} // /namespace Fork



// ========================