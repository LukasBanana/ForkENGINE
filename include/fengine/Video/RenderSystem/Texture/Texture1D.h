/*
 * Texture 1D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_1D_H__
#define __FORK_TEXTURE_1D_H__


#include "Video/RenderSystem/Texture/Texture.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Texture1D);

/**
1D texture base interface.
\ingroup texture_classes
*/
class FORK_EXPORT Texture1D : public Texture
{
    
    public:
        
        typedef int PositionType;
        typedef int SizeType;

        virtual ~Texture1D();

        Texture1D(const Texture1D&) = delete;
        Texture1D& operator = (const Texture1D&) = delete;
        
        //! Returns Types::Tex1D.
        Types Type() const;

        /**
        Returns the size of the specified MIP level.
        \see Video::MIPLevelSize
        */
        SizeType MIPLevelSize(unsigned int mipLevel) const;

        //! Returns the 1-dimensional texture size.
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

        Texture1D() = default;

        SizeType        size        = 0;
        unsigned int    arraySize   = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================