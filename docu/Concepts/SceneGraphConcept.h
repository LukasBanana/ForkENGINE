

namespace Scene
{

namespace SceneNodeSorter
{


enum class SortMethods
{
    NearToFar,              //!< All objects from near to far.
    FarToNear,              //!< All objects from far to near.
    OpaqueToTransparent,    //!< Opaque (near to far) to transparent (far to near).
};

void SortSceneNodes(
    std::vector<SceneNodePtr>& sceneNodes, const Transform3D& compareTransform,
    const SortMethods method, bool isGlobal
);


} // /namespace SceneNodeSorter


class Geometry
{
    public:
        
        enum class Types
        {
            CommonMesh,         //!< Common mesh geometry.
            Simple3DMesh,       //!< Simple 3D mesh geometry.
            Simple2DMesh,       //!< Simple 2D mesh geometry.
            TangentSpaceMesh,   //!< Tangent space mesh geometry.
            CompositionMesh,    //!< Composition mesh geometry.    
            Billboard,          //!< Billboard geometry.
            Terrain,            //!< Terrain geometry.
        };

        virtual ~Geometry();

        virtual Types Type() const = 0;
        virtual void ComputeBoundingBox() = 0;
        virtual void ComputeBoundingSphere() = 0;

        Video::GeometryPrimtives primitiveType;

        inline const Math::AABB3f& GetBoundingBox() const
        {
            return boundingBox_;
        }

    protected:
        
        VertexBufferPtr vertexBuffer_;
        IndexBufferPtr indexBuffer_;

        Math::AABB3f boundingBox_;

};

class CommonMeshGeometry : public Geometry
{
    
    public:
        
        CommonMeshGeometry(const Video::VertexFormat& format);

        //! Returns Geometry::Types::CommonMesh.
        Types Type() const;
        void ComputeBoundingBox();

        void AddVertex(const void* vertex, const size_t vertexSize);
        void AddIndex(const void* index, const size_t indexSize);
        
    private:
        
        Video::VertexFormat format_;

        StrideBuffer vertices_;
        StrideBuffer indices_;

};

//! Base scene node.
class SceneNode
{
    
    public:
        
        virtual ~SceneNode();
        //! Dummy
        virtual void Transform(Math::Matrix4f& matrix);
        //! Dummy
        virtual const Math::Matrix4f& GetMatrix() const;
        
        NodeMetaData metaData;

    private:
        
        static Math::Matrix4f identityMatrix_;

};

//! Dynamic scene nodes can be transformed.
class DynamicSceneNode : public SceneNode
{

    public:
        
        virtual ~DynamicSceneNode();
        /**
        \code
        matrix *= transform.GetMatrix();
        \endcode
        */
        virtual void Transform(Math::Matrix4f& matrix);

        Transform3D transform;

};

//! Static scene nodes can not be transformed.
class StaticSceneNode : public SceneNode
{
    
    public:
        
        virtual ~StaticSceneNode();

};

class GraphNode : public DynamicSceneNode
{

    public:

        /**
        \code
        SceneNodeSorter::SortSceneNodes(sceneNodes, compareTransform, method);
        \endcode
        */
        void SortSceneNodes(const Transform3D& compareTransform, const SortMethods method);
        std::vector<SceneNodePtr> sceneNodes;

};

class GeometryNode : public DynamicSceneNode
{

    public:
    
        GeometryPtr geometry;

};

//! Several mesh geometries 
class GroupMeshGeometryNode : public StaticSceneNode
{

    public:

        GroupMeshGeometryNode(const Texture2DPtr& textureArray);

        size_t AddTransform(const Math::Transform3Df& transform);
        void RemoveTransform(size_t transformIndex);

        size_t AddGeometry(const CommonMeshGeometry* geometry);
        void RemoveGeometry(size_t geometryIndex);

        size_t AddInstance(size_t geometryIndex, size_t transformIndex, size_t textureIndex);
        void RemoveInstance(size_t instanceIndex);
        
        /**
        \code
        auto transformIndex = AddTransform(node->transform);
        if (node->geometry->Type() == Geometry::Types::CompositionMesh)
        {
        for each sub-geometry:
        AddGeometry(dynamic_cast<CommonMeshGeometry>(node->geometry.get()));
        }
        else
        {
        auto geometryIndex = AddGeometry(dynamic_cast<CommonMeshGeometry>(node->geometry.get()));
        AddInstance(geometryIndex, transformIndex, textureIndex);
        }
        \endcode
        */
        void AddGeometryNode(const GeometryNode* node);

        void UpdateTransform(size_t transformIndex, const Math::Transform3Df& transform);
        void UpdateTransformList(const std::vector<Math::Transform3Df>& transforms, size_t firstTransformIndex = 0, size_t numTransforms = ~0);

    private:

        Texture2DPtr textureArray_;

        /**
        \code
        float4x4 transforms[];
        \endcode
        */
        ShaderBufferPtr transformBuffer_;

        /**
        \code
        struct AABB
        {
        float4 min, max;
        };
        struct
        {
        unsigned int firstVertex;
        unsigned int numVertices;
        AABB boundingBox;
        }
        geometries[];
        \endcode
        */
        ShaderBufferPtr geometryBuffer_;

        /**
        \code
        struct
        {
        unsigned int geometryIndex;    // 'geometryBuffer_' index.
        unsigned int transformIndex;    // 'transformBuffer_' index.
        unsigned int textureIndex;    // 'textureArray_' slice.
        }
        instances[];
        \endcode
        */
        ShaderBufferPtr instanceBuffer_;

        CommonMeshGeometry geometry_;

        PartitionManager<size_t> vertexPartitions_;
        PartitionManager<size_t> indexPartitions_;

};

class SceneRenderer
{

    public:

        virtual ~SceneRenderer();

        /**
        \code
        GetRenderSystem()->SetupProjectionMatrix(camera->projection);
        GetRenderSystem()->SetupViewMatrix(camera->transform.GetMatrix().Inverse());
        \endcode
        */
        void SetupView(const CameraNode* camera);

        virtual void RenderScene(const SceneNode* graphRootSceneNode) = 0;

    protected:

        SceneRenderer(const RenderSystemPtr& renderSystem);

        inline RenderSystem* GetRenderSystem() const
        {
            return renderSystem_.get();
        }
    
    private:
        
        RenderSystemPtr renderSystem_;
        
};

class ForwardSceneRenderer : public BaseSceneRenderer
{

    public:

        ForwardSceneRenderer(const RenderSystemPtr& renderSystem);
    
        void RenderScene(const SceneNode* graphRootSceneNode);

        /**
        \code
        sceneGraph->SortSceneNodes(compareTransform, method);
        RenderScene(sceneGraph);
        \endcode
        */
        void SortAndRenderScene(
            SceneNode* graphRootSceneNode, const Transform3D& compareTransform, const SortMethods method
        );

        /**
        \code
        SetupView(camera);
        SortAndRenderScene(sceneGraph, camera->transform, method);
        \endcode
        */
        void SortAndRenderScene(
            SceneNode* graphRootSceneNode, const CameraNode* camera,
            const SortMethods method = SceneNodeSorter::SortMethods::OpaqueToTransparent
        );

};

class SceneManager
{
    
    public:
        
        GeometryPtr LoadModel();

        GeometryNodePtr CreateGeometryNode(const GeometryPtr& geometry = nullptr);

        void ReleaseSceneNode(const SceneNodePtr& sceneNode);

    private:
        
        std::list<SceneNodePtr> sceneNodes_;

};


} // /namespace Scene

