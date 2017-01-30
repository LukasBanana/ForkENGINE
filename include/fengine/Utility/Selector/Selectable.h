/*
 * Selectable node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTABLE_H__
#define __FORK_UTILITY_SELECTABLE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/Node/CameraNode.h"

#include <functional>


namespace Fork
{

namespace Utility
{


/**
Selectable node interface.
Inherit from this class to write your own selectable scene node objects.
\see Selector
*/
class FORK_EXPORT Selectable
{

    public:
        
        //! Transform callback signature.
        typedef std::function<void (Scene::Transform& transform)> TransformCallback;

        //! Flags to enable/disable specific transformation types.
        struct TransformFlags
        {
            typedef unsigned char DataType;
            enum : DataType
            {
                Translate       = 0x01,                         //!< Translation is allowed.
                Rotate          = 0x02,                         //!< Rotation is allowed.
                UniformScale    = 0x04,                         //!< Uniform scale is allowed.
                FreeScale       = 0x08,                         //!< Free scale is allowed.
                Scale           = (UniformScale | FreeScale),   //!< Both uniform and free scale is allowed.
                All             = 0xff,                         //!< All kinds of transformations are allowed.
            };
        };

        virtual ~Selectable();

        /**
        Allows the transformation of this scene node and calls the "OnTransform" callback.
        \param[in] callback Specifies the transformation callback.
        \param[in] isUnique Specifies whether this is a unique transformation or a series of transformations.
        If 'isUnique' is true, this function calls "OnTransformBegin", "OnTransform" and "OnTransformEnd".
        Otherwise only "OnTransform" is called. By default true.
        \see OnTransformBegin
        \see OnTransform
        \see OnTransformEnd
        */
        void Transform(const TransformCallback& callback, bool isUnique = true);

        //! Returns the local transformation of this scene node.
        virtual const Scene::Transform& LocalTransform() const = 0;
        //! Returns the global transformation matrix of this scene node.
        virtual Math::Matrix4f GlobalTransform() const = 0;

        //! Returns the flags which transformation types are enabled. By default all.
        virtual TransformFlags::DataType TransformFlags() const;

        //! Callback function when the selectable node begins to be transformed.
        virtual void OnTransformBegin();
        //! Callback function when the selectable node is currently being transformed.
        virtual void OnTransform();
        //! Callback function when the selectable node ends to be transformed.
        virtual void OnTransformEnd();

        /**
        Stores the current transformation.
        \remarks This can be used when the selection transformation is canceled.
        \see RestoreTransform
        */
        void StoreTransform();
        /**
        Restores the previously stored transformation.
        \remarks This can be used when the selection transformation is canceled.
        \remarks This will also call 'OnTransformBegin' and 'OnTransformEnd'.
        \see StoreTransform
        \see OnTransformBegin
        \see OnTransformEnd
        */
        void RestoreTransform();

    protected:

        Selectable() = default;

        //! Returns a reference to the scene node's transformation.
        virtual Scene::Transform& GetTransform() = 0;

    private:
        
        /**
        Previous transformation. This can be used to restore the
        transformation when the selection-transform is canceled.
        */
        Scene::Transform prevTransform_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================