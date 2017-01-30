/*
 * Skeleton file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Skeleton.h"
#include "Core/STLHelper.h"
#include "Core/Exception/NullPointerException.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{


/*
 * Joint class
 */

Skeleton::Joint::Joint(Joint* parent) :
    parent_{ parent }
{
}

Skeleton::Joint* Skeleton::Joint::CreateChild()
{
    /*
    This is an exception in memory allocation:
    -> We don't use 'std::make_unique' here because the used "Joint" constructor is private!
    -> We pass the new allocated pointer to the 'unique_ptr' which then owns this pointer.
    */
    children_.emplace_back(std::unique_ptr<Joint>(new Joint(this)));
    return children_.back().get();
}

void Skeleton::Joint::DeleteChild(Joint* joint)
{
    RemoveFromListIf(
        children_,
        [&joint](const std::unique_ptr<Joint>& jnt)
        {
            return jnt.get() == joint;
        }
    );
}

void Skeleton::Joint::GlobalTransform(Math::Matrix4f& matrix) const
{
    if (GetParent())
        GetParent()->GlobalTransform(matrix);
    matrix *= transform.GetMatrix();
}

Math::Matrix4f Skeleton::Joint::GlobalTransform() const
{
    Math::Matrix4f matrix;
    GlobalTransform(matrix);
    return matrix;
}

void Skeleton::Joint::FillMatrixBuffer(Math::Matrix4f* buffer, size_t numMatrices, bool isGlobal, bool isRelative) const
{
    ASSERT_POINTER(buffer);
    if (numMatrices > 0)
    {
        if (isGlobal)
            FillMatrixBufferGlobal(Math::Matrix4f(), buffer, numMatrices, isRelative);
        else
            FillMatrixBufferLocal(buffer, numMatrices, isRelative);
    }
}

size_t Skeleton::Joint::HierarchySize() const
{
    size_t size = 1;

    for (const auto& child : children_)
        size += child->HierarchySize();

    return size;
}

void Skeleton::Joint::NormalizeWeights(bool recursive, size_t maxNumWeights)
{
    if (!weights.empty())
    {
        float sum = 0.0f;

        if (maxNumWeights > 0)
        {
            auto num = std::min(weights.size(), maxNumWeights);

            /* Get sum of first 'maxNumWeights' weights */
            for (size_t i = 0; i < num; ++i)
                sum += weights[i].weight;

            /* Normalize weights */
            float invSum = 1.0f / sum;
            for (size_t i = 0; i < num; ++i)
                weights[i].weight *= invSum;
        }
        else
        {
            /* Get sum of all weights */
            for (const auto& w : weights)
                sum += w.weight;

            /* Normalize weights */
            float invSum = 1.0f / sum;
            for (auto& w : weights)
                w.weight *= invSum;
        }
    }

    if (recursive)
    {
        /* Repeat this function for the entire joint hierarchy */
        for (auto& child : children_)
            child->NormalizeWeights(true, maxNumWeights);
    }
}

void Skeleton::Joint::UpdateOriginMatrix()
{
    if (GetParent())
        UpdateOriginMatrix(GetParent()->GlobalTransform());
    else
        UpdateOriginMatrix(Math::Matrix4f());
}


/*
 * ======= Private: =======
 */

void Skeleton::Joint::FillMatrixBufferGlobal(
    Math::Matrix4f parentMatrix, Math::Matrix4f*& buffer, size_t& numMatrices, bool isRelative) const
{
    /* Store matrix for this joint */
    parentMatrix *= transform.GetMatrix();
    *buffer = parentMatrix;
    if (isRelative)
        *buffer *= GetOriginMatrix();
    ++buffer;

    /* Store matrix for all child nodes */
    for (const auto& child : children_)
    {
        if (numMatrices > 0)
            child->FillMatrixBufferGlobal(parentMatrix, buffer, numMatrices, isRelative);
        else
            break;
    }
}

void Skeleton::Joint::FillMatrixBufferLocal(
    Math::Matrix4f*& buffer, size_t& numMatrices, bool isRelative) const
{
    /* Store matrix for this joint */
    *buffer = transform.GetMatrix();
    if (isRelative)
        *buffer *= GetOriginMatrix();
    ++buffer;

    /* Store matrix for all child nodes */
    for (const auto& child : children_)
    {
        if (numMatrices > 0)
            child->FillMatrixBufferLocal(buffer, numMatrices, isRelative);
        else
            break;
    }
}

void Skeleton::Joint::UpdateOriginMatrix(Math::Matrix4f parentMatrix)
{
    /* Apply the transformation of this joint and store its base inverse matrix */
    parentMatrix *= transform.GetMatrix();
    originMatrix_ = parentMatrix.Inverse();

    /* Repeat the process for each child */
    for (auto& child : children_)
        child->UpdateOriginMatrix(parentMatrix);
}


} // /namespace Scene

} // /namespace Fork



// ========================