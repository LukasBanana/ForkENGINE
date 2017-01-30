/*
 * Texture Cube header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_CUBE_H__
#define __FORK_TEXTURE_CUBE_H__


#include "Video/RenderSystem/Texture/Texture.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"
#include "Math/Common/AxisDirections.h"


namespace Fork
{

namespace Video
{


class RenderTarget;

DECL_SHR_PTR(TextureCube);

/**
Cube texture base interface.
\ingroup texture_classes
*/
class FORK_EXPORT TextureCube : public Texture
{
    
    public:
        
        /* === Types === */

        typedef Math::Vector2i PositionType;
        typedef Math::Size2i SizeType;

        //! Type alias for the cube texture faces.
        typedef Math::AxisDirections Faces;

        /* === Structures === */

        /**
        Abstract model structure for the cube faces.
        \tparam T Specifies the data type. This must be default
        \see DefaultValue
        */
        template <typename T> struct FaceModel
        {
            FaceModel() = default;
            FaceModel(
                const T& xPositive, const T& xNegative,
                const T& yPositive, const T& yNegative,
                const T& zPositive, const T& zNegative) :
                    xPositive{ xPositive },
                    xNegative{ xNegative },
                    yPositive{ yPositive },
                    yNegative{ yNegative },
                    zPositive{ zPositive },
                    zNegative{ zNegative }
            {
            }

            inline T& operator [] (size_t index)
            {
                return *((&xPositive) + index);
            }
            inline const T& operator [] (size_t index) const
            {
                return *((&xPositive) + index);
            }

            T xPositive = T(0);
            T xNegative = T(0);
            T yPositive = T(0);
            T yNegative = T(0);
            T zPositive = T(0);
            T zNegative = T(0);
        };

        //! Type alias to store render targets to all six cube faces.
        typedef FaceModel<std::shared_ptr<RenderTarget>> FaceRenderTargets;
        //! Type alias to store render target references to all six cube faces.
        typedef FaceModel<RenderTarget*> FaceRenderTargetRefs;

        /* === Functions === */

        virtual ~TextureCube();

        TextureCube(const TextureCube&) = delete;
        TextureCube& operator = (const TextureCube&) = delete;
        
        //! Returns Types::TexCube.
        Types Type() const;

        /**
        Returns the size of the specified MIP level.
        \see Video::MIPLevelSize
        */
        SizeType MIPLevelSize(unsigned int mipLevel) const;

        /**
        Maps the cube texture array layer.
        \param[in] arrayIndex Specifies the index of the cube texture array.
        \param[in] face Specifies one of the six cube texture faces.
        \remarks This is simply: (arrayIndex*6 + face).
        \see Faces
        */
        static unsigned int MapArrayLayer(unsigned int arrayIndex, const Faces face);

        //! Returns the 2-dimensional texture faces size.
        inline const SizeType& GetSize() const
        {
            return size;
        }

        //! Returns the array size. By default 0.
        inline unsigned int GetArraySize() const
        {
            return arraySize;
        }
        //! Returns true if this texture has an array, i.e. the array size is greater than 0.
        inline bool HasArray() const
        {
            return arraySize > 0;
        }

    protected:
        
        friend class RenderSystem;

        TextureCube() = default;

        SizeType        size;
        unsigned int    arraySize = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================