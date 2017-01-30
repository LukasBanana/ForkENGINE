/*
 * Geometry texture optimizer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GeometryTextureOptimizer.h"
#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/Node/LODGeometry.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"
#include "Core/STLHelper.h"

#include <vector>
#include <algorithm>


namespace Fork
{

namespace Scene
{


/*
This function works in five phases:
  1: Find all textured geometries in this composition geometry.
  2: Sort textured geometry list (strict weak order)
  3: Group equally textured geometries.
  4: Remove old textured geometries from the composition geometry.
  5: Finally add the grouped geometries to the composition geometry.
*/
void GeometryTextureOptimizer::VisitCompositionGeometry(CompositionGeometry* node)
{
    /*
    (Phase 1)
    Find all textured geometries
    */
    std::vector<TexturedGeometry*> texturedGeometries;

    for (auto& subGeom : node->subGeometries)
    {
        /* First visit sub geometry to propagate optimizations upwards */
        subGeom->Visit(this);

        /* Check if current sub geometry is a textured geometry */
        if (subGeom->Type() == Geometry::Types::Textured)
        {
            auto texGeom = dynamic_cast<TexturedGeometry*>(subGeom.get());
            if (texGeom)
                texturedGeometries.push_back(texGeom);
        }
    }

    /*
    (Phase 2)
    Sort textured geometry list (keep unsorted, ordered list)
    */
    auto sortedTexturedGeometries = texturedGeometries;
    std::sort(
        sortedTexturedGeometries.begin(), sortedTexturedGeometries.end(),
        [](const TexturedGeometry* lhs, const TexturedGeometry* rhs)
        {
            return lhs->CompareTexturesSWO(*rhs);
        }
    );

    /*
    (Phase 3)
    Group all equally textured geometries
    */
    struct GeometryGroup
    {
        std::vector<Video::TexturePtr>  textures;
        std::vector<GeometryPtr>        actualGeometries;
    };

    std::vector<GeometryGroup> geometryGroups;
    TexturedGeometry* prevTexGeom = nullptr;
    GeometryGroup currentGroup;

    auto AddGroupToList = [&currentGroup, &geometryGroups]()
    {
        if (!currentGroup.actualGeometries.empty())
            geometryGroups.push_back(currentGroup);
    };

    for (const auto& geom : sortedTexturedGeometries)
    {
        /* Check if a new texture group has been detected */
        if (!prevTexGeom || !prevTexGeom->CompareTextures(*geom))
        {
            /* Add last group to list if it's not empty */
            AddGroupToList();

            /* Initialize new goemetry group */
            prevTexGeom = geom;
            currentGroup.textures = geom->textures;
            currentGroup.actualGeometries.clear();
        }

        /* Add current actual geometry to current group's geometry list */
        currentGroup.actualGeometries.push_back(geom->actualGeometry);
    }

    /* Add last group to list if it's not empty */
    AddGroupToList();

    /*
    (Phase 4)
    Remove previous textured geometries from this composition geometry
    */
    auto itTexGeom = texturedGeometries.begin();
    auto it = node->subGeometries.begin();
    
    while (it != node->subGeometries.end() && itTexGeom != texturedGeometries.end())
    {
        if (it->get() == *itTexGeom)
        {
            /*
            Remove sub geometry from the composition geometry
            and iterate to the next textured geometry to remove
            */
            it = node->subGeometries.erase(it);
            ++itTexGeom;
        }
        else
            ++it;
    }

    /*
    (Phase 5)
    Add geometry groups to composition geometry
    */
    for (const auto& group : geometryGroups)
    {
        if (!group.actualGeometries.empty())
        {
            /* Create textured geometry root */
            auto texGeom = std::make_shared<TexturedGeometry>();
            texGeom->textures = group.textures;

            if (group.actualGeometries.size() > 1)
            {
                /* Create sub composition geometry */
                auto subCompositionGeom = std::make_shared<CompositionGeometry>();
                subCompositionGeom->subGeometries = group.actualGeometries;
                texGeom->actualGeometry = subCompositionGeom;
            }
            else
            {
                /* Only use the previous actual geometry */
                texGeom->actualGeometry = group.actualGeometries[0];
            }

            node->subGeometries.push_back(texGeom);
        }
    }
}


} // /namespace Scene

} // /namespace Fork



// ========================