/*
 * Index format header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INDEX_FORMAT_H__
#define __FORK_INDEX_FORMAT_H__


#include "Video/BufferFormat/RendererDataTypes.h"
#include "Core/DeclPtr.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(IndexFormat);

//! Hardware buffer index format class.
class FORK_EXPORT IndexFormat
{
    
    public:
        
        IndexFormat() = default;
        //! \see ChangeDataType
        IndexFormat(const RendererDataTypes dataType);

        /**
        Changes the data type of the index format.
        \param[in] dataType Specifies the new data type for the index format.
        Only UByte, UShort and UInt are allowed.
        \throws InvalidRendererDataTypeException If the data type is invalid.
        \see RendererDataTypes
        */
        void ChangeDataType(const RendererDataTypes dataType);

        inline size_t GetFormatSize() const
        {
            return GetDataTypeSize(dataType_);
        }

        inline RendererDataTypes GetDataType() const
        {
            return dataType_;
        }

    private:

        RendererDataTypes dataType_ = RendererDataTypes::UInt;
        
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================