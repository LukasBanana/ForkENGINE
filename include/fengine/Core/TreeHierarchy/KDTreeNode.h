/*
 * kd-Tree node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_KD_TREE_NODE_H__
#define __FORK_KD_TREE_NODE_H__


#include <memory>


namespace Fork
{


//! kd-Tree node axis directions.
enum class KDTreeAxes
{
    XAxis,
    YAxis,
    ZAxis,
};


/**
Base kd-Tree node class. To build a kd-Tree use the "Scene::TreeBuilder" namespace.
\see Scene::TreeBuilder
\tparam Container Specifies the class type for the data container, the kd-Tree shall store.
It's meant to be used for your own structures or classes, but it can also
be a standard data type (like int, float etc.).
\tparam T Specifies the axis split data type (should be float or double).
By default float.
*/
template <class Container, typename T = float> class KDTreeNode
{

    public:
        
        //! This tree node type.
        typedef KDTreeNode<Container, T> ThisType;

        //! Unique ownership for image tree nodes.
        typedef std::unique_ptr<ThisType> NodePtr;

        //! Reference pointer to parent node.
        typedef ThisType* ParentPtr;

        /**
        Constructor for a root node.
        \note Pointer to the parent and data container only be initialized in a constructor, but never changed!
        */
        KDTreeNode() = default;
        /**
        Constructor for a non-leaf child node.
        \note Data container only be initialized in a constructor, but never changed!
        */
        KDTreeNode(const ParentPtr& parent) :
            parent_{ parent }
        {
        }
        //! Constructor for a child node.
        KDTreeNode(const ParentPtr& parent, const Container& data) :
            parent_{ parent },
            data_  { data   }
        {
        }

        /**
        Creates the child nodes.
        \param[in] split Specifies the axis split factor (in unit space, i.e. in the range [-1.0 .. 1.0]) for this node.
        \param[in] axis Specifies the axis split direction for this node.
        \note The data fields can only be initialized once when the nodes are created!
        \see KDTreeAxes
        */
        void CreateChildren(const T& split, const KDTreeAxes axis)
        {
            split_ = split;
            axis_ = axis;

            childFront_ = std::make_unique<KDTreeNode<Container, T>>(this);
            childBack_ = std::make_unique<KDTreeNode<Container, T>>(this);
        }
        /**
        Creates the child nodes and initializes the data containers.
        \param[in] split Specifies the axis split factor (in unit space, i.e. in the range [-1.0 .. 1.0]) for this node.
        \param[in] axis Specifies the axis split direction for this node.
        \param[in] dataFront Specifies the data field for the front sided node.
        \param[in] dataBack Specifies the data field for the back sided node.
        \see KDTreeAxes
        */
        void CreateChildren(const T& split, const KDTreeAxes axis, const Container& dataFront, const Container& dataBack)
        {
            split_ = split;
            axis_ = axis;

            childFront_ = std::make_unique<KDTreeNode<D, T>>(this, dataFront);
            childBack_ = std::make_unique<KDTreeNode<D, T>>(this, dataBack);
        }

        /**
        Releases the child nodes. To take over the ownership of the nodes, use the othwer "ReleaseChildren" function.
        \see ReleaseChildren(NodePtr&, NodePtr&)
        */
        void ReleaseChildren()
        {
            childFront_.reset();
            childBack_.reset();
        }
        /**
        Releases the child nodes and transfers the ownership to the specified output parameters.
        \param[out] childFront Specifies the front child node. Its previous managed pointer will be released and overwritten by the tree node's front child.
        \param[out] childBack Specifies the back child node. Its previous managed pointer will be released and overwritten by the tree node's back child.
        */
        void ReleaseChildren(NodePtr& childFront, NodePtr& childBack)
        {
            childFront = std::move(childFront_);
            childBack = std::move(childBack_);
        }

        //! Returns the tree hierarchy level. The root node has level 0.
        size_t Level() const
        {
            return parent_ ? parent_->Level() + 1 : 0;
        }

        //! Returns true if this node has no children and thus is a leaf node.
        inline bool IsLeaf() const
        {
            return childFront_ == nullptr;
        }

        //! Returns the parent node as (non-owning) pointer.
        inline const ParentPtr& GetParent() const
        {
            return parent_;
        }

        //! Returns the axis split factor (in unit space, i.e. in the range [-1.0 .. 1.0]).
        inline const T& GetSplit() const
        {
            return split_;
        }

        //! Returns the front sided child node.
        inline KDTreeNode<Container, T>* GetChildFront() const
        {
            return childFront_;
        }
        //! Returns the back sided child node.
        inline KDTreeNode<Container, T>* GetChildBack() const
        {
            return childBack_;
        }

        //! Specifies the tree node data container.
        Container data;

    private:

        /* === Members === */

        ParentPtr   parent_     = nullptr;              //!< Raw (non-owning) pointer to the parent node.

        NodePtr     childFront_;                        //!< Shared pointer to the front sided child node.
        NodePtr     childBack_;                         //!< Shared pointer to the back sided child node.

        T           split_      = T(0);                 //!< Axis split factor (in unit space, i.e. in the range [-1.0 .. 1.0]).
        KDTreeAxes  axis_       = KDTreeAxes::XAxis;    //!< Direction for the split axis.

};


} // /namespace Fork


#endif



// ========================