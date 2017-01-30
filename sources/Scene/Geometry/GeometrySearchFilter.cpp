/*
 * Geometry search filter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/GeometrySearchFilter.h"
#include "ImportGeometries.h"


namespace Fork
{

namespace Scene
{


GeometrySearchFilter::GeometrySearchFilter(const FilterCallback& filter) :
    filter_{ filter }
{
}

void GeometrySearchFilter::VisitLODGeometry(LODGeometry* node)
{
    if (filter_(node))
        VisitSubGeometries(node->lodGeometries);
}

void GeometrySearchFilter::VisitBillboardGeometry(BillboardGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitTerrainGeometry(TerrainGeometry* node)
{
    if (filter_(node) && node->templateGeometry)
        node->templateGeometry->Visit(this);
}

void GeometrySearchFilter::VisitMeshGeometry(MeshGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitCompositionGeometry(CompositionGeometry* node)
{
    if (filter_(node))
        VisitSubGeometries(node->subGeometries);
}

void GeometrySearchFilter::VisitTexturedGeometry(TexturedGeometry* node)
{
    if (filter_(node) && node->actualGeometry)
        node->actualGeometry->Visit(this);
}

void GeometrySearchFilter::VisitSimple2DMeshGeometry(Simple2DMeshGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitCommonMeshGeometry(CommonMeshGeometry* node)
{
    filter_(node);
}

void GeometrySearchFilter::VisitTerrainMeshGeometry(TerrainMeshGeometry* node)
{
    filter_(node);
}


} // /namespace Scene

} // /namespace Fork



// ========================