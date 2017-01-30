/*
 * Texture 2D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_2D_H__
#define __FORK_TEXTURE_2D_H__


#include "Video/RenderSystem/Texture/Texture.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Texture2D);

/**
2D texture base interface.
\ingroup texture_classes
*/
class FORK_EXPORT Texture2D : public Texture
{
    
    public:
        
        typedef Math::Vector2i PositionType;
        typedef Math::Size2i SizeType;

        virtual ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator = (const Texture2D&) = delete;
        
        //! Returns Types::Tex2D.
        Types Type() const;

        /**
        Returns the size of the specified MIP level.
        \see Video::MIPLevelSize
        */
        SizeType MIPLevelSize(unsigned int mipLevel) const;

        //! Returns the 2-dimensional texture size.
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

        Texture2D() = default;

        SizeType        size;
        unsigned int    arraySize = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================