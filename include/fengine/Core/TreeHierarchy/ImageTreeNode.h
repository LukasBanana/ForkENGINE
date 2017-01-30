/*
 * Image tree node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_TREE_NODE_H__
#define __FORK_IMAGE_TREE_NODE_H__


#include "Math/Core/Rect.h"

#include <memory>


namespace Fork
{


/**
The ImageTreeNode class is used to store and pack 2D images into a single rectangle.
\tparam I Specifies the image object type. This is stored as non-owning raw-pointer.
This class must implement to following function:
\code
// Confirms the image insertion for the specified image tree node.
void ConfirmInsertion(ImageTreeNode<I>* node);
\endcode
*/
template <class I> class ImageTreeNode
{

    public:
        
        //! Unique ownership for image tree nodes.
        typedef std::unique_ptr<ImageTreeNode<I>> NodePtr;

        ImageTreeNode() = default;
        ImageTreeNode(const Math::Recti& rect) :
            rect_{ rect }
        {
        }
        ImageTreeNode(const Math::Size2i& size) :
            rect_{ { 0, 0 }, size }
        {
        }

        ImageTreeNode(const ImageTreeNode<I>&) = delete;
        ImageTreeNode& operator = (const ImageTreeNode<I>&) = delete;

        /**
        Tries to insert the specified image object into the tree.
        \param[in] image Pointer to the image object.
        \param[in] imageSize Specifies the image size.
        \return Non-owning raw pointer to the image tree node which contains the inserted image.
        */
        ImageTreeNode<I>* Insert(I* image, const Math::Size2i& imageSize)
        {
            if (!image)
                return 0;

            if (childA_)
            {
                /* Try to find a suitable tree node */
                auto node = childA_->Insert(image, imageSize);

                if (node)
                    return node;

                return childB_->Insert(image, imageSize);
            }

            /* Check if this node already contians an image and check image size */
            if (image_ || imageSize.width > rect_.Width() || imageSize.height > rect_.Height())
                return 0;

            /* Check if image fits exactly into this node */
            if (imageSize == rect_.Size())
            {
                image_ = image;
                image_->ConfirmInsertion(this);
                return this;
            }

            /* Create children and split into two spaces */
            childA_ = std::make_unique<ImageTreeNode<I>>();
            childB_ = std::make_unique<ImageTreeNode<I>>();

            if (rect_.Width() - imageSize.width > rect_.Height() - imageSize.height)
            {
                childA_->rect_ = Math::Recti(rect_.left, rect_.top, rect_.left + imageSize.width, rect_.bottom);
                childB_->rect_ = Math::Recti(rect_.left + imageSize.width, rect_.top, rect_.right, rect_.bottom);
            }
            else
            {
                childA_->rect_ = Math::Recti(rect_.left, rect_.top, rect_.right, rect_.top + imageSize.height);
                childB_->rect_ = Math::Recti(rect_.left, rect_.top + imageSize.height, rect_.right, rect_.bottom);
            }

            /* Try to insert the image into the new first child */
            return childA_->Insert(image, imageSize);
        }

        /**
        Releases the child nodes. To take over the ownership of the nodes, use the othwer "ReleaseChildren" function.
        \see ReleaseChildren(NodePtr&, NodePtr&)
        */
        void ReleaseChildren()
        {
            childA_.reset();
            childB_.reset();
        }
        /**
        Releases the child nodes and transfers the ownership to the specified output parameters.
        \param[out] childA Specifies the first child node. Its previous managed pointer will be released and overwritten by the tree node's first child.
        \param[out] childB Specifies the second child node. Its previous managed pointer will be released and overwritten by the tree node's second child.
        */
        void ReleaseChildren(NodePtr& childA, NodePtr& childB)
        {
            childA = std::move(childA_);
            childB = std::move(childB_);
        }

        //! Returns the first child node.
        inline ImageTreeNode<I>* GetChildA() const
        {
            return childA_.get();
        }
        //! Returns the second child node.
        inline ImageTreeNode<I>* GetChildB() const
        {
            return childB_.get();
        }

        //! Returns the rectangle where this node is embedded.
        inline const Math::Recti& GetRect() const
        {
            return rect_;
        }

        //! Returns the data container.
        inline I* GetImage() const
        {
            return image_;
        }

    private:

        /* === Members === */

        NodePtr     childA_;            //!< First child node.
        NodePtr     childB_;            //!< Second child node.

        Math::Recti rect_;              //!< Rectangle where the image is stored.

        I*          image_ = nullptr;   //!< Pointer reference to the image object.

};


} // /namespace Fork


#endif



// ========================