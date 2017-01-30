/*
 * Projector view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_PROJECTOR_VIEW_H__
#define __FORK_UTILITY_PROJECTOR_VIEW_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/Projector/ProjectorModel.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Video/RenderSystem/Shader/StandardShader/UtilCommonModelConstantBuffers.h"
#include "Math/Core/Cuboid.h"


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(ProjectorView);

//! Projector view class.
class FORK_EXPORT ProjectorView
{

    public:

        struct FORK_EXPORT Configuration
        {
            struct FORK_EXPORT Trigger
            {
                Trigger(const Video::ColorRGBAf& normalColor);
                Trigger(int);

                const Video::ColorRGBAf& GetColorByState(bool isSelected) const;

                Video::ColorRGBAf normalColor; //!< Normal 3D axis color.
                Video::ColorRGBAf selectColor; //!< 3D axis color on selection (and mouse over). By default (1, 1, 0, 1).
            };

            Configuration();

            Trigger mainTrigger;
            Math::Cuboid<Trigger> triggers; //!< Configuration for each trigger.
        };

        struct DrawFlags
        {
            typedef unsigned char DataType;
            enum : DataType
            {
                DrawAxesLabels  = (1 << 0),         //!< Draw axes labels ("X", "Y", "Z").
                Default         = (DrawAxesLabels), //!< Default drawing flags.
            };
        };
        
        //! \throws NullPointerException
        ProjectorView(const ProjectorModel* model, const Video::TextureFont* labelFont);
        virtual ~ProjectorView();

        /**
        Draws the projector model.
        \param[in] renderer Raw-pointer to the primitive renderer which is to be used for drawing operations.
        \param[in] flags Specifies the drawing flags. This can be a combination of the "DrawFlags" enumeration entries.
        \see DrawFlags
        \throws NullPointerException If renderer is a null pointer.
        */
        void Draw(
            Video::PrimitiveRenderer* renderer,
            const DrawFlags::DataType flags = DrawFlags::Default
        );

        //! Common view configuration.
        Configuration config;

    protected:
        
        /* === Functions === */

        inline const ProjectorModel* GetModel() const
        {
            return model_;
        }
        inline const Video::TextureFont* GetFont() const
        {
            return labelFont_;
        }

    private:
        
        /* === Functions === */

        void CreateModels();
        void CreateShader();

        void InitWorldMatrices();

        /* === Members === */

        const ProjectorModel* model_ = nullptr;
        const Video::TextureFont* labelFont_ = nullptr;

        Video::ShaderCompositionPtr modelShader_;
        Video::ConstantBufferPtr constBuffer_;

        Video::StandardShader::UtilCommonModelShader::ConstBuffer constBufferData_;

        Scene::GeometryGenerator::GeometryType cubeModel_, coneModel_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================