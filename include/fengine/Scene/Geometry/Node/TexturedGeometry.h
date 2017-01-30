/*
 * Textured geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURED_GEOMETRY_H__
#define __FORK_TEXTURED_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/Material/Material.h"

#include <vector>


namespace Fork
{

namespace Scene
{


/**
Textured geometry class. This class provides a list of textures
and the actual geometry object.
\ingroup std_geometries
\todo Rename to "MaterialGeometry".
*/
class FORK_EXPORT TexturedGeometry : public Geometry
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Textured;

        TexturedGeometry() = default;
        virtual ~TexturedGeometry();

        TexturedGeometry(const TexturedGeometry&) = delete;
        TexturedGeometry& operator = (const TexturedGeometry&) = delete;

        /* === Functions === */

        virtual void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Textured.
        Types Type() const override;

        //! Copies the bounding volume of the actual geometry.
        void ComputeBoundingVolume() override;

        /**
        Returns true if the textures of this geometry are considered to go before 'other' in a strict weak ordering (SWO).
        This function is used to sort objects of this class, e.g. in a hash-map.
        \remarks Use "CompareTextures" to compare the textures on equality.
        \see CompareTextures
        */
        bool CompareTexturesSWO(const TexturedGeometry& other) const;
        /**
        Compares the textures of this and the other textured geometry on equality.
        \see CompareTexturesSWO
        */
        bool CompareTextures(const TexturedGeometry& other) const;

        /* === Members === */

        /**
        Surface material.
        \see Video::Material
        */
        Video::MaterialPtr material;

        /**
        Custom shader composition. If this is null, the shader composition of the
        upper geometry or scene node in the scene graph hierarchy will be used.
        \see Video::ShaderComposition
        */
        Video::ShaderCompositionPtr shaderComposition;

        /**
        Texture list.
        \see Video::Texture
        */
        std::vector<Video::TexturePtr> textures;

        //! This is the actual geometry object.
        GeometryPtr actualGeometry;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================