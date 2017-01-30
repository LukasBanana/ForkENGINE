
class VertexBufferCompound : public VertexBuffer
{
	
	public:
		
		void SetupSubBuffers(const std::vector<VertexBufferPtr>& subVertexBuffers);
		void SetupFormat();
		
};
