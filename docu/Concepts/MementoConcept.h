
// \tparam Data Must implement the == operator.
template <class Data, size_t Limit = 0> class MementoHierarchy
{
	
	public:
		
		class TreeNode
		{
			
			public:
				
				TreeNode() :
					nextPath_(next_.end())
				{
				}
				
				// Memento data memory
				Data data;
				
			protected:
				
				friend class MementoHierarchy;
				
				// Creates a new node with the specified data
				TreeNode* Commit(const Data& data)
				{
					// Try to find duplicate
					auto it = std::find(next_.begin(), next_.end(), data);
					
					if (it != next_.end())
					{
						// Store next path
						nextPath_ = it;
						return it->get();
					}
					
					// Create new tree node
					auto node = std::make_unique<TreeNode<Data>>(this);
					auto nodeRef = node.get();
					nodeRef->data = data;
					next_.push_back(std::move(node));
					
					// Store next path
					nextPath_ = next_.begin() + (next_.size() - 1);
					
					return nodeRef;
				}
				
				// Returns the next node
				TreeNode* Next() const
				{
					return next_.empty() ? nullptr : nextPath_->get();
				}
				// Selects and returns the next node
				TreeNode* Fork(size_t index)
				{
					if (index < next_.size())
					{
						nextPath_ = next_.begin() + index;
						return nextPath_->get();
					}
					return nullptr;
				}
				
				// Returns the previous node
				TreeNode* GetPrev() const
				{
					return prev_;
				}
				
				bool HasNext() const
				{
					return !next_.empty();
				}
				
				bool HasPrev() const
				{
					return prev_ != nullptr;
				}
				
				size_t Level() const
				{
					return prev_ != nullptr ? prev_->Level() + 1 : 0;
				}
				
				std::unique_ptr<TreeNode>& NextRef()
				{
					return *nextPath_;
				}
				
			private:
				
				TreeNode(TreeNode* prev) :
					prev_(prev)
				{
				}
				
				TreeNode* prev_ = nullptr;
				std::vector<std::unique_ptr<TreeNode>> next_;
				std::vector<std::unique_ptr<TreeNode>>::iterator nextPath_;
				
		};
		
		MementoHierarchy() :
			rootNode_(std::make_unique<TreeNode>()),
			currentNode_(rootNode_.get())
		{
		}
		
		void Commit(const Data& data)
		{
			currentNode_ = currentNode_->Commit(data);
			
			if (Limit > 0 && currentNode_->Level() > Limit)
				rootNode_ = std::move(rootNode_->NextRef());
		}
		
		bool HasNext() const
		{
			return currentNode_->HasNext();
		}
		
		bool HasPrev() const
		{
			return currentNode_->HasPrev();
		}
		
		void Next()
		{
			auto node = currentNode_->Next();
			if (node)
				currentNode_ = node;
		}
		
		void Fork(size_t index)
		{
			auto node = currentNode_->Next(index);
			if (node)
				currentNode_ = node;
		}
		
		void Prev()
		{
			auto node = currentNode_->GetPrev();
			if (node)
				currentNode_ = node;
		}
		
		size_t Level() const
		{
			return currentNode_->Level();
		}
		
		TreeNode* GetCurrentNode() const
		{
			return currentNode_;
		}
		
	private:
		
		std::unique_ptr<TreeNode> rootNode_;
		TreeNode* currentNode_ = nullptr;
		
};



