/*
 * OpenGL shader composition header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_SHADER_COMPOSITION_H__
#define __FORK_GL_SHADER_COMPOSITION_H__


#include "Video/RenderSystem/Shader/ShaderComposition.h"

#include "GLVertexShader.h"
#include "GLPixelShader.h"
#include "GLGeometryShader.h"
#include "GLTessControlShader.h"
#include "GLTessEvaluationShader.h"
#include "GLComputeShader.h"

#include "../GLImport.h"


namespace Fork
{

namespace Video
{


//! OpenGL shader composition implementation.
class GLShaderComposition : public ShaderComposition
{
    
    public:
        
        GLShaderComposition();
        ~GLShaderComposition();

        GLShaderComposition(const GLShaderComposition&) = delete;
        GLShaderComposition& operator = (const GLShaderComposition&) = delete;

        /* --- Shader attachments --- */

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        );
        
        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(const ComputeShaderPtr& computeShader);

        /* --- Shader access --- */

        VertexShaderPtr         GetVertexShader         () const;
        PixelShaderPtr          GetPixelShader          () const;
        GeometryShaderPtr       GetGeometryShader       () const;
        TessControlShaderPtr    GetTessControlShader    () const;
        TessEvaluationShaderPtr GetTessEvaluationShader () const;
        ComputeShaderPtr        GetComputeShader        () const;

        /* --- Compilation and input layout --- */

        bool Compile(const Shader::CompilationFlags::DataType flags = 0);

        bool SetupVertexFormat(const VertexFormat& format);

        /* --- Constant buffers --- */

        ConstantBuffer::DescriptionListType QueryConstantBufferDescList() const;

        void Attach(const ConstantBufferPtr& constBuffer, unsigned int slot);
        //! \todo Not yet implemented
        void Detach(const ConstantBufferPtr& constBuffer);
        void DetachAllConstantBuffers();

        /* --- Storage buffers --- */

        StorageBuffer::DescriptionListType QueryStorageBufferDescList() const;

        void Attach(const StorageBufferPtr& storageBuffer, unsigned int slot);
        //! \todo Not yet implemented
        void Detach(const StorageBufferPtr& storageBuffer);
        void DetachAllStorageBuffers();

    protected:
        
        struct GLShaderBufferMetaData
        {
            GLShaderBufferMetaData(GLuint id, GLuint index) :
                bufferID    { id    },
                bindIndex   { index }
            {
            }
            GLuint bufferID = 0;
            GLuint bindIndex = 0;
        };

        friend class GLRenderContext;

        inline GLuint GetProgramID() const
        {
            return programID_;
        }

        inline const std::vector<GLShaderBufferMetaData>& GetConstantBufferMetaData() const
        {
            return constantBufferMetaData_;
        }
        inline const std::vector<GLShaderBufferMetaData>& GetStorageBufferMetaData() const
        {
            return storageBufferMetaData_;
        }

    private:
        
        /* === Functions === */

        void DetachAllShaders();

        bool QueryInfoLog(std::string& infoLog);

        template <class T> bool CompileSingleShader(const T& shader);

        template <class Base, class Derived> void AttachSingleShader(
            std::shared_ptr<Derived>& dest, const std::shared_ptr<Base>& source, const std::string& baseTypeName
        );
        template <class T> void DetachSingleShader(T& shader);

        /* === Members === */

        GLuint                              programID_ = 0;

        GLVertexShaderPtr                   vertexShader_;
        GLPixelShaderPtr                    pixelShader_;
        GLGeometryShaderPtr                 geometryShader_;
        GLTessControlShaderPtr              tessControlShader_;
        GLTessEvaluationShaderPtr           tessEvaluationShader_;
        GLComputeShaderPtr                  computeShader_;

        std::vector<GLShaderBufferMetaData> constantBufferMetaData_;    //!< Packed container for shader binding (constants buffers).
        std::vector<GLShaderBufferMetaData> storageBufferMetaData_;     //!< Packed container for shader binding (storage buffers).

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================