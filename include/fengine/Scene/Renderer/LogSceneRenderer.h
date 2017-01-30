/*
 * Log scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LOG_SCENE_RENDERER_H__
#define __FORK_LOG_SCENE_RENDERER_H__


#include "Scene/Renderer/SceneRenderer.h"

#include <functional>


namespace Fork
{

namespace Scene
{


/**
The Log scene renderer can be used for debugging purposes
to print (or rather 'render') the scene into a debug log.
\see IO::Log
*/
class FORK_EXPORT LogSceneRenderer : public SceneRenderer
{
    
    public:
        
        DECL_SCENE_VISITOR_INTERFACE;

        struct RenderFlags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                ShowDetails     = (1 << 0),                     //!< Prints some details (like number of vertices etc.).
                ShowMetaData    = (1 << 1),                     //!< Prints further meta data (scene node name and flags).
                Default         = (ShowDetails | ShowMetaData), //!< Default render flags.
            };
        };

        void RenderScene(SceneNode* sceneGraph) override;

        /**
        Render flags. This can be a combination of the "RenderFlags" enumeration entries.
        By default RenderFlags::Default.
        \see RenderFlags
        */
        RenderFlags::DataType renderFlags = RenderFlags::Default;

    private:
        
        void PrintBaseMeshGeometry(
            const NodeMetaData& metaData, std::string text, size_t numVertices, size_t numIndices
        );

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================