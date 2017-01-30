/*
 * Constant buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONSTANT_BUFFER_H__
#define __FORK_CONSTANT_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/ConstantBufferException.h"
#include "Core/DeclPtr.h"
#include "Core/Export.h"

#include <vector>
#include <string>
#include <map>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ConstantBuffer);

/**
Shader constant buffer interface.
Constant buffers can be used for read only operations in a shader stage.
The size of constant buffers are limited to 4096 vectors with four components each, or rather 65536 bytes.
*/
class FORK_EXPORT ConstantBuffer
{
    
    public:
        
        //! Constant buffer description structure.
        struct Description
        {
            std::string     name;           //!< Constant buffer name.
            unsigned int    index   = 0;    //!< Constant buffer index.
            size_t          size    = 0;    //!< Constant buffer size (in bytes).
        };

        typedef std::map<std::string, Description> DescriptionListType;
        typedef std::shared_ptr<Description> DescriptionPtr;

        virtual ~ConstantBuffer();

        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer& operator = (const ConstantBuffer&) = delete;

        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        ConstantBuffer(const Description& desc);

    private:
        
        Description desc_; //!< Constant buffer description.

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================