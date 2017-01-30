/*
 * Newton physics collider file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwCollider.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Physics
{


/* --- Internal functions --- */

static inline ColliderPtr MakeGeom(NewtonCollision* collision)
{
    return std::make_shared<NwCollider>(collision);
}

static inline const float* MatrixPtr(const Math::Matrix4f* matrix)
{
    return matrix != nullptr ? matrix->Ptr() : nullptr;
}

template <typename T> T ExtractIndex(const char*& indexDataByteAligned)
{
    T index = *reinterpret_cast<const T*>(indexDataByteAligned);
    indexDataByteAligned += sizeof(T);
    return index;
}


/*
 * NwGeometry class
 */

NwCollider::NwCollider(NewtonCollision* collision) :
    collision_{ collision }
{
    ASSERT_POINTER(collision);
    NewtonCollisionSetUserData(collision_, ThisColliderPtr());
}
NwCollider::~NwCollider()
{
    NewtonDestroyCollision(collision_);
}

ColliderPtr NwCollider::CreateNull(const NewtonWorld* world)
{
    return MakeGeom(NewtonCreateNull(world));
}

ColliderPtr NwCollider::CreateSphere(const NewtonWorld* world, float radius, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateSphere(world, radius, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateBox(const NewtonWorld* world, const Math::Size3f& size, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateBox(world, size.width, size.height, size.depth, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateCone(const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateCone(world, radius, height, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateCapsule(const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateCapsule(world, radius, height, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateCylinder(const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateCylinder(world, radius, height, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateChamferCylinder(const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return MakeGeom(NewtonCreateChamferCylinder(world, radius, height, 0, MatrixPtr(offsetMatrix)));
}

ColliderPtr NwCollider::CreateConvexHull(
    const NewtonWorld* world,
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const Math::Matrix4f* offsetMatrix, float tolerance)
{
    auto collision = NewtonCreateConvexHull(
        world,
        static_cast<int>(numVertices),
        reinterpret_cast<const float*>(vertexData),
        vertexStride,
        tolerance,
        0,
        MatrixPtr(offsetMatrix)
    );
    return MakeGeom(collision);
}

ColliderPtr NwCollider::CreateMesh(
    const NewtonWorld* world,
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const Math::Matrix4f* offsetMatrix, bool optimize)
{
    /* Create tree collision geometry */
    auto collision = NewtonCreateTreeCollision(world, 0);
    
    NewtonTreeCollisionBeginBuild(collision);
    {
        auto vertexDataByteAligned = reinterpret_cast<const char*>(vertexData);

        if (offsetMatrix)
        {
            /* Add transformed face for each triangle */
            Math::Vector3f vertices[3];

            for (unsigned int i = 0; i < numVertices; i += 3)
            {
                for (unsigned int j = 0; j < 3; ++j)
                {
                    /* Extract vertex from array */
                    vertices[j] = *reinterpret_cast<const Math::Vector3f*>(vertexDataByteAligned);
                    vertexDataByteAligned += vertexStride;

                    /* Transform vertex */
                    vertices[j] = (*offsetMatrix) * vertices[j];
                }

                /* Add polygon (3 vertices) */
                NewtonTreeCollisionAddFace(
                    collision, 3, reinterpret_cast<const float*>(&vertices[0].x), sizeof(Math::Vector3f), 0
                );
            }
        }
        else
        {
            /* Add face for each triangle */
            for (unsigned int i = 0; i < numVertices; i += 3)
            {
                /* Add polygon (3 vertices) */
                NewtonTreeCollisionAddFace(
                    collision, 3, reinterpret_cast<const float*>(vertexDataByteAligned), vertexStride, 0
                );
                vertexDataByteAligned += 3*vertexStride;
            }
        }
    }
    NewtonTreeCollisionEndBuild(collision, optimize ? 1 : 0);

    return MakeGeom(collision);
}

ColliderPtr NwCollider::CreateMesh(
    const NewtonWorld* world,
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const void* indexData, unsigned int numIndices, unsigned int indexStride,
    const Math::Matrix4f* offsetMatrix, bool optimize)
{
    if (indexStride != 1 && indexStride != 2 && indexStride != 4)
        throw InvalidArgumentException(__FUNCTION__, "indexStride", "Index stride must be 1, 2 or 4");

    /* Create tree collision geometry */
    auto collision = NewtonCreateTreeCollision(world, 0);
    
    NewtonTreeCollisionBeginBuild(collision);
    {
        auto vertexDataByteAligned = reinterpret_cast<const char*>(vertexData);
        auto indexDataByteAligned = reinterpret_cast<const char*>(indexData);

        /* Add transformed face for each triangle */
        Math::Vector3f vertices[3];

        for (unsigned int i = 0; i < numIndices; i += 3)
        {
            for (unsigned int j = 0; j < 3; ++j)
            {
                /* Extract index from array */
                unsigned int vertexIndex = 0;

                switch (indexStride)
                {
                    case 1:
                        vertexIndex = ExtractIndex<unsigned char>(indexDataByteAligned);
                        break;
                    case 2:
                        vertexIndex = ExtractIndex<unsigned short>(indexDataByteAligned);
                        break;
                    case 4:
                        vertexIndex = ExtractIndex<unsigned int>(indexDataByteAligned);
                        break;
                }

                /* Extract vertex from array */
                vertices[j] = *reinterpret_cast<const Math::Vector3f*>(vertexDataByteAligned + vertexStride*vertexIndex);

                if (offsetMatrix)
                    vertices[j] = (*offsetMatrix) * vertices[j];
            }

            NewtonTreeCollisionAddFace(
                collision, 3, reinterpret_cast<const float*>(&vertices[0].x), sizeof(Math::Vector3f), 0
            );
        }
    }
    NewtonTreeCollisionEndBuild(collision, optimize ? 1 : 0);

    return MakeGeom(collision);
}

bool NwCollider::RayCast(const Math::Line3f& line, Math::Intersection3f& intersection) const
{
    dLong attrib = 0;

    const auto lerp = NewtonCollisionRayCast(
        collision_, line.start.Ptr(), line.end.Ptr(), intersection.normal.Ptr(), &attrib
    );

    if (lerp > 1.0f)
        return false;

    intersection.point = line.Lerp(lerp);

    return true;
}

void NwCollider::SetupScale(const Math::Vector3f& scale)
{
    NewtonCollisionSetScale(collision_, scale.x, scale.y, scale.z);
}

Math::Vector3f NwCollider::Scale() const
{
    Math::Vector3f scale;
    NewtonCollisionGetScale(collision_, &scale.x, &scale.y, &scale.z);
    return scale;
}


} // /namespace Physics

} // /namespace Fork



// ========================