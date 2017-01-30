/*
 * Geometry search filter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_SEARCH_FILTER_H__
#define __FORK_GEOMETRY_SEARCH_FILTER_H__


#include "Scene/Geometry/Node/Geometry.h"
#include "Scene/Node/DefaultSceneVisitor.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/StaticConfig.h"

#include <functional>


namespace Fork
{

namespace Scene
{


/**
Geometry search filter visitor.
\see FilterGeometry
*/
class FORK_EXPORT GeometrySearchFilter : public DefaultSceneVisitor
{
    
    public:
        
        /**
        Search filter callback signature.
        \param[in] node Pointer to the current geometry node.
        \return True to filter further child nodes of this geometry.
        */
        typedef std::function<bool (Geometry* node)> FilterCallback;

        GeometrySearchFilter(const FilterCallback& filter);

        DECL_SCENE_VISITOR_GEOMETRY_INTERFACE;

    private:
        
        FilterCallback filter_;

};

/**
Search function to filter geometry nodes.
\tparam Geom Specifies the geometry class type to filter.
This type must have the following interface:
\code
class Geom
{
    public:
        static const Scene::Geometry::Types classType;
        Scene::Geometry::Types Type() const;
};
\endcode
And it must be part of the "SceneVisitor" interface.
\remarks Usage example:
\code
Scene::FilterGeometry<Scene::TexturedGeometry>(
    sceneGraph,
    [](Scene::TexturedGeometry* texGeom)
    {
        // Do something with 'texGeom' ...
    }
);
\endcode
\throws PointerConversionException (Only in debug mode) If a geometry node could not be
converted into its sub class, or rather if 'Geometry::Type' returns the wrong class type.
\see SceneVisitor
\see GeometrySearchFilter
\see Geometry::Type
*/
template <typename Geom, typename Node>
void FilterGeometry(Node* rootNode, const std::function<void (Geom*)>& callback)
{
    GeometrySearchFilter visitor(
        [&callback](Geometry* node) -> bool
        {
            if (node->Type() == Geom::classType)
            {
                #ifdef FORK_DEBUG
                auto filteredNode = dynamic_cast<Geom*>(node);
                if (!filteredNode)
                    throw PointerConversionException(__FUNCTION__, "Geometry", "<sub-geometry-class>");
                #else
                auto filteredNode = static_cast<Geom*>(node);
                #endif
                callback(filteredNode);
            }
            return true;
        }
    );
    rootNode->Visit(&visitor);
}


} // /namespace Scene

} // /namespace Fork


#endif



// ========================