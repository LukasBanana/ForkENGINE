/*
 * Bounding box scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BOUNDING_BOX_SCENE_RENDERER_H__
#define __FORK_BOUNDING_BOX_SCENE_RENDERER_H__


#include "Scene/Renderer/SceneRenderer.h"
#include "Scene/Manager/SceneManager.h"
#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/Core/ColorRGBA.h"
#include "Math/Core/Matrix4.h"


namespace Fork
{

namespace Scene
{


/**
Bounding box scene renderer base class.
This scene renderer renders only single colored bounding boxes, for each scene node.
*/
class FORK_EXPORT BoundingBoxSceneRenderer : public SceneRenderer
{
    
    public:
        
        //! \throws NullPointerException If 'sceneManager' is null.
        BoundingBoxSceneRenderer(SceneManager* sceneManager);
        ~BoundingBoxSceneRenderer();

        void RenderScene(SceneNode* sceneGraph) override;

        //! Returns the maximal number of world matrices (i.e. number of scene nodes to visualize with a bounding box).
        unsigned int GetMaxNumWorldMatrices() const
        {
            return maxNumWorldMatrices_;
        }

        //! Color for all rendered bounding boxes. By default white.
        Video::ColorRGBAf boundingBoxColor;

        //! Specifies whether each individual geometry will be rendered with a bounding box, too. By default true.
        bool showGeometries = true;

    private:
        
        /* === Functions === */

        void CreateAtlasTexture(const Math::Size2i& texSize);
        void CreateBoxGeometry(SceneManager* sceneManager);
        void CreateBoxShader();

        void InitWorldMatrixCounter();
        
        void AppendWorldMatrix(const Math::Matrix4f& matrix);
        void AppendBoundingBox(const Math::AABB3f& boundingBox);

        Math::Size2i MinRequiredTextureSize() const;

        /* --- Visitor interface --- */

        DECL_SCENE_VISITOR_INTERFACE;

        /* === Members === */

        GeometryGenerator::GeometryTypePtr  boxGeometry_;

        Video::ShaderCompositionPtr         boxShader_;
        Video::ConstantBufferPtr            constBuffer_;
        Video::Texture2DPtr                 worldMatrixAtlasTexture_;

        Math::Matrix4f                      currentWorldMatrix_;
        std::vector<Math::Matrix4f>         worldMatrices_;

        Math::Size2i                        texSize_;
        unsigned int                        worldMatrixCounter_ = 0;
        unsigned int                        maxNumWorldMatrices_ = 0;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================