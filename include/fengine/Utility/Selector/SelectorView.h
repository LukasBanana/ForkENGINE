/*
 * Selector view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTOR_VIEW_H__
#define __FORK_UTILITY_SELECTOR_VIEW_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/Selector/SelectorModel.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Manager/SceneManager.h"


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(SelectorView);

//! Selector view class.
class FORK_EXPORT SelectorView
{

    public:

        //! Selector view configuration structure.
        struct FORK_EXPORT Configuration
        {
            struct FORK_EXPORT Axis
            {
                Axis(const Video::ColorRGBAf& normalColor);
                Axis(int);

                Video::ColorRGBAf normalColor; //!< Normal 3D axis color.
                Video::ColorRGBAf selectColor; //!< 3D axis color on selection (and mouse over). By default (1, 1, 0, 1).
            };

            Configuration();

            Math::Vector3<Axis> axes;                   //!< Configuration for each axis.
            Video::ColorRGBAf   inactiveAxisColor;      //!< Color for each inactive axis. By default (0.75, 0.75, 0.75, 1).
            Video::ColorRGBAf   selectPlaneColor;       //!< Color for the selection plane quad. By default (1, 1, 0, 0.5).

            float               normalAxisWidth = 2.0f; //!< Normal width of the axis line. By default 2.0.
            float               selectAxisWidth = 3.0f; //!< Width of the axis line on selection (and mouse over). By default 3.0.

            float               arrowSize       = 0.2f; //!< Size of the arrow models. By default 0.2.
        };

        struct DrawFlags
        {
            typedef unsigned char DataType;
            enum : DataType
            {
                DrawAxesLabels          = (1 << 0),                 //!< Draw axes labels ("X", "Y", "Z").
                DrawSelectedAxesOnly    = (1 << 1),                 //!< Draw only selected axes when selection is active.
                DrawPrevTransform       = (1 << 2),                 //!< Draw a second selector view for the previous transformation.
                Default                 = ( DrawAxesLabels       |
                                            DrawSelectedAxesOnly |
                                            DrawPrevTransform ),    //!< Default drawing flags.
            };
        };
        
        //! \throws NullPointerException
        SelectorView(
            Scene::SceneManager* sceneManager,
            const SelectorModel* model, const Video::TextureFont* labelFont
        );
        virtual ~SelectorView();

        /**
        Draws the selector model with additional options.
        To draw the selector model use the other 'Draw' function.
        \see Draw(Video::PrimitiveRenderer*, const DrawFlags::DataType)
        */
        void Draw(
            Video::PrimitiveRenderer* renderer,
            const Math::Transform3Df& transform,
            const Math::Vector3<SelectorModel::AxisState>& axesState,
            const DrawFlags::DataType flags
        );

        /**
        Draws the selector model.
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

        virtual Video::ColorRGBAf AxisColor(
            bool isSelected, const Configuration::Axis& axisConfig, float alpha = 1.0f
        ) const;

        virtual Video::ColorRGBAf AxisColor(
            const SelectorModel::AxisState& axisState,
            const Configuration::Axis& axisConfig
        ) const;

        /**
        Draws the axis along the positive Z axis of the specified matrix.
        \param[in] matrix Specifies the axis matrix. Rotate this to use another axis direction.
        \param[in] axisState Specifies the axis state.
        \param[in] axisConfig Specifies the visual axis configuration.
        \note The pointer must never be null!
        */
        virtual void DrawAxis(
            Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
            const SelectorModel::AxisState& axisState, const Configuration::Axis& axisConfig
        );
        /**
        Draws the axes plane along the positive X and positive Z axes.
        This consists of a small quad, which visualizes the plane for the specified zwo axes.
        \param[in] matrix Specifies the axis matrix. Rotate this to use another axis direction.
        \param[in] axisStateA Specifies the first axis state (if untransformed this is the Z axis).
        \param[in] axisConfigA Specifies the first visual axis configuration (if untransformed this is the Z axis).
        \param[in] axisStateB Specifies the second axis state (if untransformed this is the Y axis).
        \param[in] axisConfigB Specifies the second visual axis configuration (if untransformed this is the Y axis).
        \note The pointers must never be null!
        */
        virtual void DrawAxesPlane(
            Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
            const SelectorModel::AxisState& axisStateA, const Configuration::Axis& axisConfigA,
            const SelectorModel::AxisState& axisStateB, const Configuration::Axis& axisConfigB
        );

        /**
        Draws the axes plane selection as a quadrangle between the plane lines.
        \param[in] matrix Specifies the axis matrix. Rotate this to use another axis direction.
        \param[in] axisStateA Specifies the first axis state (if untransformed this is the Z axis).
        \param[in] axisStateB Specifies the second axis state (if untransformed this is the Y axis).
        \note The pointers must never be null!
        */
        virtual void DrawAxesPlaneSelection(
            Video::PrimitiveRenderer* renderer, const Math::Matrix4f& matrix,
            const SelectorModel::AxisState& axisStateA, const SelectorModel::AxisState& axisStateB
        );

        /**
        Returns a constant raw-pointer to the current arrow model
        This depends on the current transform mode.
        \see SelectorModel::GetTransformMode
        */
        virtual const Scene::Simple3DMeshGeometry* CurrentArrowModel() const;

        inline const SelectorModel* GetModel() const
        {
            return model_;
        }
        inline const Video::TextureFont* GetFont() const
        {
            return labelFont_;
        }

    private:
        
        /* === Functions === */

        void CreateModels(Scene::SceneManager* sceneManager);
        void CreateShader();

        /* === Members === */

        const SelectorModel* model_ = nullptr;
        const Video::TextureFont* labelFont_ = nullptr;

        Video::ShaderCompositionPtr arrowShader_;
        Video::ConstantBufferPtr constBuffer_;

        Scene::Simple3DMeshGeometryPtr arrowModels_[3];

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================