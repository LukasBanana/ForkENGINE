/*
 * Texture 3D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_3D_H__
#define __FORK_TEXTURE_3D_H__


#include "Video/RenderSystem/Texture/Texture.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Size3.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Texture3D);

/**
3D texture base interface.
\ingroup texture_classes
*/
class FORK_EXPORT Texture3D : public Texture
{
    
    public:
        
        typedef Math::Vector3i PositionType;
        typedef Math::Size3i SizeType;

        virtual ~Texture3D();

        Texture3D(const Texture3D&) = delete;
        Texture3D& operator = (const Texture3D&) = delete;
        
        //! Returns Types::Tex3D.
        Types Type() const;

        /**
        Returns the size of the specified MIP level.
        \see Video::MIPLevelSize
        */
        SizeType MIPLevelSize(unsigned int mipLevel) const;

        //! Returns the 3-dimensional texture size.
        inline const SizeType& GetSize() const
        {
            return size;
        }

    protected:
        
        friend class RenderSystem;

        Texture3D() = default;

        SizeType size;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================