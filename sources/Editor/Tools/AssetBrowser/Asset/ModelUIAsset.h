/*
 * Editor model UI asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MODEL_UI_ASSET_H__
#define __FORK_EDITOR_MODEL_UI_ASSET_H__


#include "UIAsset.h"
#include "Scene/Node/SceneNodeDecl.h"
#include "Scene/Geometry/Node/MeshGeometry.h"


namespace Fork
{

namespace Editor
{


//! Editor model asset class.
class ModelUIAsset : public UIAsset
{
    
    public:
        
        //! \throws NullPointerException If 'geometryResource' is null.
        ModelUIAsset(const PreviewPartition& previewPartition, const Scene::GeometryPtr& geometryResource, const std::string& assetLabel = "");

        //! Returns Types::MeshGeometry.
        Types Type() const override;
        Flags::DataType Flags() const override;

        void Instantiate() override;

        //! Shows the geometry hierarchy in the main-frame's geometry hierarchy viwer.
        void ShowGeometryHierarchy();

        //! Returns the initial model asset preview rotation.
        static Math::Quaternionf InitialPreviewRotation();
        
        inline const Scene::GeometryPtr& GetSharedGeometryResource() const
        {
            return geometryResource_;
        }
        inline Scene::Geometry* GetGeometryResource() const
        {
            return geometryResource_.get();
        }

        static void BuildPropertyGrid(UIPropertyGrid& propGrid, const wxString& namePrefix);

        /* === Members === */

        //! Initial model size.
        Math::Vector3f initialSize = Math::Vector3f(1);

        /**
        Model preview rotation. By default pitch = 45 and yaw = 45 (in degrees).
        \see InitialPreviewRotation
        */
        Math::Quaternionf previewRotation;

    private:
        
        void DrawPreviewContent(const Math::Size2i& previewSize) override;

        void OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix) override;

        Scene::GeometryPtr geometryResource_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================