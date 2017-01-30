/*
 * Memento hierarchy header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MEMENTO_HIERARCHY_H__
#define __FORK_MEMENTO_HIERARCHY_H__


#include <memory>
#include <vector>
#include <algorithm>


namespace Fork
{


/**
Memento hierarchy class template. This is used as 'forkable' memento timeline in the editor.
\tparam Data Specifies the memento memory type. For this type a global == operator must be implemented:
\code
bool operator == (const Data&, const Data&);
\endcode
\tparam Limit Specifies an optional hierarchy limit. If this is zero, then there is no limit.
Otherwise the internal tree will have a maximum hierarchy level of 'Limit'.
*/
template <class Data, size_t Limit = 0u> class MementoHierarchy
{
    
    public:
        
        //! Memento tree node class.
        class TreeNode
        {
            
            public:
                
                TreeNode() :
                    nextPath_{ next_.end() }
                {
                }
                
                //! Returns the selected next node.
                TreeNode* Next() const
                {
                    return next_.empty() ? nullptr : nextPath_->get();
                }

                //! Selects and returns the next node.
                TreeNode* Fork(size_t index)
                {
                    if (index < next_.size())
                    {
                        nextPath_ = next_.begin() + index;
                        return nextPath_->get();
                    }
                    return nullptr;
                }
                
                //! Returns the previous node.
                inline TreeNode* GetPrev() const
                {
                    return prev_;
                }
                
                //! Returns true if this node has at least one next node.
                inline bool HasNext() const
                {
                    return !next_.empty();
                }
                
                //! Returns true if this node has a previous node.
                inline bool HasPrev() const
                {
                    return prev_ != nullptr;
                }
                
                //! Returns the hierarchy level of this tree node.
                size_t Level() const
                {
                    return prev_ != nullptr ? prev_->Level() + 1 : 0;
                }
                
                //! Returns the list of all next nodes.
                inline const std::vector<std::unique_ptr<TreeNode>>& GetNext() const
                {
                    return next_;
                }
                
                //! Memento data memory
                Data data;

            private:
                
                friend class MementoHierarchy;
                
                TreeNode(TreeNode* prev) :
                    prev_(prev)
                {
                }
                
                //! Creates a new node with the specified data.
                TreeNode* Commit(const Data& data)
                {
                    // Try to find duplicate
                    auto it = std::find_if(
                        next_.begin(), next_.end(),
                        [&data](const std::unique_ptr<TreeNode>& node) -> bool
                        {
                            return node->data == data;
                        }
                    );
                    
                    if (it != next_.end())
                    {
                        // Store next path
                        nextPath_ = it;
                        return it->get();
                    }
                    
                    // Create new tree node
                    auto node = std::unique_ptr<TreeNode>(new TreeNode(this));
                    auto nodeRef = node.get();
                    nodeRef->data = data;
                    next_.push_back(std::move(node));
                    
                    // Store next path
                    nextPath_ = next_.begin() + (next_.size() - 1);
                    
                    return nodeRef;
                }
                
                //! Returns a reference to the selected next node.
                inline std::unique_ptr<TreeNode>& NextRef()
                {
                    return *nextPath_;
                }
                
                TreeNode* prev_ = nullptr;
                std::vector<std::unique_ptr<TreeNode>> next_;
                typename std::vector<std::unique_ptr<TreeNode>>::iterator nextPath_;
                
        };
        
        MementoHierarchy(const Data& rootData) :
            rootNode_   ( std::make_unique<TreeNode>() ),
            currentNode_( rootNode_.get()              )
        {
            rootNode_->data = rootData;
        }
        
        void Commit(const Data& data)
        {
            currentNode_ = currentNode_->Commit(data);
            
            if (Limit > 0 && currentNode_->Level() > Limit)
            {
                rootNode_ = std::move(rootNode_->NextRef());
                rootNode_->prev_ = nullptr;
            }
        }
        
        //! Returns true if there is a next node.
        inline bool HasNext() const
        {
            return currentNode_->HasNext();
        }
        
        //! Returns true if there is a previous node.
        inline bool HasPrev() const
        {
            return currentNode_->HasPrev();
        }
        
        /**
        Moves forwards to the next selected node in the hierarchy.
        \remarks To select a specific next node use "Fork".
        \see MementoHierarchy::Fork
        \see GetCurrentNode
        */
        void Next()
        {
            auto node = currentNode_->Next();
            if (node)
                currentNode_ = node;
        }
        
        /**
        Moves forwards to the next specified node in the hierarchy.
        \param[in] index Specifies the index to the next node.
        \remarks Use "TreeNode::GetNext" to determine how many next nodes the current node has.
        \see GetCurrentNode
        */
        void Fork(size_t index)
        {
            auto node = currentNode_->Fork(index);
            if (node)
                currentNode_ = node;
        }
        
        /**
        Moves backwards to the previous node in the hierarchy.
        \see GetCurrentNode
        */
        void Prev()
        {
            auto node = currentNode_->GetPrev();
            if (node)
                currentNode_ = node;
        }

        /**
        Traverses the entire hierarchy in pre-order depth-first-search.
        \param[in] func Specifies the transversion functor.
        This function must have the following interface:
        \code
        void Function(TreeNode*);
        \endcode
        */
        template <class Functor> void Traverse(Functor func)
        {
            TraverseNode(func, GetTreeNode());
        }
        
        //! Returns the 
        inline size_t Level() const
        {
            return currentNode_->Level();
        }
        
        //! Returns the tree root node.
        inline TreeNode* GetTreeNode() const
        {
            return rootNode_.get();
        }

        /**
        Returns the current tree node. This may change whenever "Next", "Fork" or "Prev" is called.
        \return Next
        \return Fork
        \return Prev
        */
        inline TreeNode* GetCurrentNode() const
        {
            return currentNode_;
        }
        
    private:
        
        template <class Functor> void TraverseNode(Functor func, TreeNode* node)
        {
            func(node);
            for (const auto& subNode : node->GetNext())
                TraverseNode(func, subNode.get());
        }

        std::unique_ptr<TreeNode> rootNode_;
        TreeNode* currentNode_ = nullptr;
        
};


} // /namespace Fork


#endif



// ========================