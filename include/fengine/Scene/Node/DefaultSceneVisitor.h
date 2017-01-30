/*
 * Default scene visitor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DEFAULT_SCENE_VISITOR_H__
#define __FORK_DEFAULT_SCENE_VISITOR_H__


#include "Scene/Node/SceneVisitor.h"
#include "Scene/Geometry/Node/Geometry.h"

#include <vector>


namespace Fork
{

namespace Scene
{


/**
Default scene visitor class.
The visitor does not operate on the nodes but traverses the entire scene graph.
Override specific visitor functions to operate on the respective nodes.
*/
class FORK_EXPORT DefaultSceneVisitor : public SceneVisitor
{
    
    public:
        
        DECL_SCENE_VISITOR_NODE_INTERFACE;

        void VisitLODGeometry           ( LODGeometry*         node ) override;
        void VisitCompositionGeometry   ( CompositionGeometry* node ) override;
        void VisitTexturedGeometry      ( TexturedGeometry*    node ) override;

    protected:
        
        void VisitSceneNodeChildren(SceneNode* node);
        void VisitSubGeometries(const std::vector<GeometryPtr>& subGeometries);

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================