/*
 * Vertex format header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VERTEX_FORMAT_H__
#define __FORK_VERTEX_FORMAT_H__


#include "Video/BufferFormat/RendererDataTypes.h"
#include "Core/DeclPtr.h"
#include "Core/Export.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(VertexFormat);

//! Hardware buffer vertex format class.
class FORK_EXPORT VertexFormat
{
    
    public:
        
        //! Class for a single vertex attribute (e.g. coordinate, texture-coordinate, color, normal etc.).
        class FORK_EXPORT Attribute
        {
            
            public:
                
                /**
                Vertex attribute constructor.
                \param[in] name Specifies the attribute name. This name will be used inside the shader program.
                \param[in] dataType Specifies the data type of the vertex atttributes.
                \param[in] numComponents Specifies the number of components. Must be 1, 2, 3 or 4.
                \throws InvalidVertexFormatException If the expression (1 >= numComponents <= 4) is not true.
                \see RendererDataTypes
                \see Shader
                */
                Attribute(const std::string& name, const RendererDataTypes dataType, unsigned int numComponents);
                
                //! Returns the size (in bytes) of this attribute. This is: <data-type-size> * <number-of-components>.
                size_t Size() const;

                //! Returns the data type of the vertex attribute.
                inline RendererDataTypes GetDataType() const
                {
                    return dataType_;
                }

                //! Returns the number of attribute components. This must be 1, 2, 3 or 4.
                inline unsigned int GetNumComponents() const
                {
                    return numComponents_;
                }

                /**
                Returns the attribute offset inside a single vertex (in bytes).
                This value is computed when all attributes are set up in the vertex format
                (see "VertexFormat::SetupAttributes" for more details).
                \see VertexFormat::SetupAttributes
                */
                inline unsigned int GetOffset() const
                {
                    return offset_;
                }

                /**
                Vertex attribute name (for GLSL) or semantic name (for HLSL).
                \remarks In the following GLSL example 'name' must be "coord":
                \code
                in vec3 coord;
                \endcode
                \remarks In the following HLSL example 'name' must be "POSITION":
                \code
                struct VertexIn
                {
                    float3 coord : POSITION;
                };
                \endcode
                */
                std::string name;

            private:
                
                friend class VertexFormat;

                RendererDataTypes   dataType_       = RendererDataTypes::Float;

                unsigned int        numComponents_  = 0;                        //!< Number of components: 1, 2, 3 or 4.
                unsigned int        offset_         = 0;                        //!< Offset for each vertex (in bytes).

        };

        /**
        Sets up the specified attributes and computes the resulting attribute offsets.
        \param[in] attributes Specifies the new attributes which are to be set up.
        \see Attribute
        \see Attribute::GetOffset
        \note This will replace the previous attributes!
        */
        void SetupAttributes(const std::vector<Attribute>& attributes);

        /**
        Adds a new vertex attribute to the format. Use "SetupAttributes" to setup all attributes at once.
        \see SetupAttributes
        */
        void AddAttribute(const Attribute& attribute);

        //! Clears all attributes.
        void ClearAttributes();

        //! Returns the list of all vertex attributes. A vertex attribute can be the coordinate, color, normal etc.
        inline const std::vector<Attribute>& GetAttributes() const
        {
            return attributes_;
        }

        //! Returns the format size (in bytes). This is also called "stride size" in some renderers.
        inline size_t GetFormatSize() const
        {
            return formatSize_;
        }

    private:

        std::vector<Attribute>  attributes_;        //!< Attribute list.
        size_t                  formatSize_ = 0;    //!< Entire vertex format size (in bytes).

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================