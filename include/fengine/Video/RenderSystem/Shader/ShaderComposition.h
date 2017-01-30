/*
 * Shader composition header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_COMPOSITION_H__
#define __FORK_SHADER_COMPOSITION_H__


#include "Video/RenderSystem/Shader/VertexShader.h"
#include "Video/RenderSystem/Shader/TessControlShader.h"
#include "Video/RenderSystem/Shader/TessEvaluationShader.h"
#include "Video/RenderSystem/Shader/GeometryShader.h"
#include "Video/RenderSystem/Shader/PixelShader.h"
#include "Video/RenderSystem/Shader/ComputeShader.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Video/BufferFormat/HardwareBufferUsage.h"
#include "Video/RenderSystem/HardwareBuffer/StorageBuffer.h"
#include "IO/Core/CommandLine.h"
#include "Core/Export.h"

#include <vector>
#include <functional>


namespace Fork
{

namespace Video
{


class RenderContext;

DECL_SHR_PTR(ShaderComposition);

//! Shader composition interface.
class FORK_EXPORT ShaderComposition
{
    
    public:
        
        /* === Classes === */

        //! Shader composition event handler interface.
        class FORK_EXPORT EventHandler
        {
            
            public:
                
                virtual ~EventHandler()
                {
                }

                /**
                Receives the 'update constant buffers' event.
                This is called just before a geometry is rendered while a shader composition is bound.
                \param[in] shaderComposition Constant raw pointer to the shader composition whose constant buffers are to be updated.
                \param[in] renderContext Raw pointer to the active render context which is used for rendering.
                \remarks Update all constant buffers of the respective shader composition which must be updated every frame (e.g. world matrix).
                */
                virtual void OnUpdateConstantBuffers(const ShaderComposition* shaderComposition, RenderContext* renderContext)
                {
                }

                /**
                Receives the 'disassembly output' event.
                \see Shader::CompilationFlags::DisassemblyOutput
                \note This is only supported for D3D11 render system.
                */
                virtual void OnDisassemblyOutput(const ShaderComposition* shaderComposition, const Shader* shader, const std::string& disassemblyOutput)
                {
                }

            protected:
                
                EventHandler() = default;
                
        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /**
        Shader container template class. This can be used to store
        any data for each shader of a shader composition.
        \code
        // Examples:
        ShaderContainer<std::string> shaderFilenames;
        ShaderContainer<Video::ShaderCodePtr> shaderSourceCodes;
        \endcode
        */
        template <class T> struct Container
        {
            T vertex;           //!< Data field for the vertex shader.
            T pixel;            //!< Data field for the pixel shader.
            T geometry;         //!< Data field for the geometry shader.
            T tessControl;      //!< Data field for the tessellation control shader.
            T tessEvaluation;   //!< Data field for the tessellation evaluation shader.
            T compute;          //!< Data field for the compute shader.
        };

        /* === Functions === */

        virtual ~ShaderComposition();

        ShaderComposition(const ShaderComposition&) = delete;
        ShaderComposition& operator = (const ShaderComposition&) = delete;
        
        /* --- Shader attachments --- */

        /**
        Attaches the specified shaders to the shader composition.
        This should be done before the shader source has been specified and/or the shader has been compiled.
        \throws PointerConversionException If a pointer can not be converted to its respective render system object.
        */
        virtual void AttachShader(
            const VertexShaderPtr& vertexShader,
            const PixelShaderPtr& pixelShader
        ) = 0;

        virtual void AttachShader(
            const VertexShaderPtr& vertexShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        ) = 0;
        
        virtual void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const PixelShaderPtr& pixelShader
        ) = 0;

        virtual void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        ) = 0;

        virtual void AttachShader(const ComputeShaderPtr& computeShader) = 0;

        /* --- Shader access --- */

        //! Returns a shared pointer to the vertex shader or null if this shader composition does not has a vertex shader.
        virtual VertexShaderPtr         GetVertexShader         () const = 0;
        //! Returns a shared pointer to the pixel shader or null if this shader composition does not has a pixel shader.
        virtual PixelShaderPtr          GetPixelShader          () const = 0;
        //! Returns a shared pointer to the geometry shader or null if this shader composition does not has a geometry shader.
        virtual GeometryShaderPtr       GetGeometryShader       () const = 0;
        //! Returns a shared pointer to the tessellation-control shader or null if this shader composition does not has a tessellation-control shader.
        virtual TessControlShaderPtr    GetTessControlShader    () const = 0;
        //! Returns a shared pointer to the tessellation-evaluation shader or null if this shader composition does not has a tessellation-evaluation shader.
        virtual TessEvaluationShaderPtr GetTessEvaluationShader () const = 0;
        //! Returns a shared pointer to the compute shader or null if this shader composition does not has a compute shader.
        virtual ComputeShaderPtr        GetComputeShader        () const = 0;

        /* --- Compilation and input layout --- */

        /**
        Compiles the shader composition and returns true on success.
        Otherwise errors during the compilation occured and will be printed into the log output.
        \param[in] flags Optional compilation flags. By default 0.
        \see Shader::CompilationFlags
        */
        virtual bool Compile(const Shader::CompilationFlags::DataType flags = 0) = 0;
        /**
        Compiles the shader composition with compilation flags specified by the command line.
        \param[in] commandLine Specifies the command line. See 'Shader::CompilationFlags' for information about all compiler options.
        \remarks Example command line: "-O3 -debug -D_MY_MACRO_=3". This command line will run the
        shader compiler with optimization level 3, inserts debug information and defines a macro via "#define _MY_MACRO_ 3".
        \see Compile(const Shader::CompilationFlags::DataType)
        \see Shader::CompilationFlags
        */
        bool Compile(const IO::CommandLine& commandLine);

        /**
        Sets the vertex format the for input assembly.
        This must be called, before the shader composition is compiled!
        \see VertexFormat
        */
        virtual bool SetupVertexFormat(const VertexFormat& format) = 0;

        /* --- Constant buffers --- */

        /**
        Queries a list of all constant buffer descriptions used in this shader composition.
        \note The shader composition must already be compiled successful.
        \see ConstantBuffer::DescriptionListType
        \see HasCompiledSuccessful
        */
        virtual ConstantBuffer::DescriptionListType QueryConstantBufferDescList() const = 0;

        /**
        Trys to find the specified constant buffer description.
        If the specified constant buffer could not be found, the return value is null.
        */
        virtual ConstantBuffer::DescriptionPtr FindConstantBufferDesc(const std::string& name) const;

        /**
        Binds the specified constant buffer to this shader composition
        at the specified slot (or rather 'binding index').
        */
        virtual void Attach(const ConstantBufferPtr& constBuffer, unsigned int slot);
        /**
        Binds the specified constant buffer to this shader composition.
        The slot is choosen from the buffer name which must exist in the shader.
        \note If the name does not identify an active constant buffer, this function call has no effect.
        */
        virtual void Attach(const ConstantBufferPtr& constBuffer, const std::string& name);
        /**
        Binds the specified constant buffer to this shader composition.
        The slot is choosen from the constant buffer description.
        */
        virtual void Attach(const ConstantBufferPtr& constBuffer);

        //! Unbinds the specified constant buffer from this shader composition.
        virtual void Detach(const ConstantBufferPtr& constBuffer);
        //! Unbinds all previously bound constant buffers from this shader composition.
        virtual void DetachAllConstantBuffers();

        /**
        Creates the specified constant buffer, binds it to this shader composition and
        allocates the storage (i.e. calls "RenderSystem::WriteBuffer" with empty data).
        \param[in] name Specifies the constnat buffer name in the shader.
        \param[in] usage Specifies the buffer usage. By default HardwareBufferUsage::Static.
        \param[in] isOptional Specifies whether this constant buffer is optional or not.
        If this is true, the function will return null if the constant buffer could not be found.
        If this is false, the function will throw a 'ConstantBufferNotFoundException' exception
        if the constant buffer could not be found. By default false.
        \return Shared pointer to the new constant buffer or null if the 'isOptional' is true
        and the specified constant buffer could not be found.
        \throws ConstantBufferNotFoundException If the specified constant buffer could not be found.
        \note This is a comfort function and uses the active render system.
        \see FindConstantBufferDesc
        \see RenderSystem::CreateConstantBuffer
        \see RenderSystem::WriteBuffer(ConstantBuffer*, const void*, size_t, const HardwareBufferUsage)
        \see HardwareBufferUsage
        */
        ConstantBufferPtr CreateAndAttachConstantBuffer(
            const std::string& name, const HardwareBufferUsage usage = HardwareBufferUsage::Static, bool isOptional = false
        );
        /**
        Creates the specified constant buffer, binds it to this shader composition and
        allocates the storage (i.e. calls "RenderSystem::WriteBuffer" with empty data).
        This function also appends the specified shader name to the expection message,
        if a "ConstantBufferNotFoundException" expection will be thrown.
        \see CreateAndAttachConstantBuffer(const std::string&, const HardwareBufferUsage, bool)
        */
        ConstantBufferPtr CreateAndAttachConstantBuffer(
            const std::string& name, const std::string& shaderName,
            const HardwareBufferUsage usage = HardwareBufferUsage::Static, bool isOptional = false
        );

        /* --- Storage buffers --- */

        /**
        Queries a list of all storage buffer descriptions used in this shader composition.
        \note The shader composition must already be compiled successful.
        \see StorageBuffer::DescriptionListType
        \see HasCompiledSuccessful
        */
        virtual StorageBuffer::DescriptionListType QueryStorageBufferDescList() const = 0;

        /**
        Trys to find the specified storage buffer description.
        If the specified storage buffer could not be found, the return value is null.
        */
        virtual StorageBuffer::DescriptionPtr FindStorageBufferDesc(const std::string& name) const;

        /**
        Binds the specified storage buffer to this shader composition
        at the specified slot (or rather 'binding index').
        */
        virtual void Attach(const StorageBufferPtr& storageBuffer, unsigned int slot);
        /**
        Binds the specified storage buffer to this shader composition.
        The slot is choosen from the buffer name which must exist in the shader.
        \note If the name does not identify an active storage buffer, this function call has no effect.
        */
        virtual void Attach(const StorageBufferPtr& storageBuffer, const std::string& name);
        /**
        Binds the specified storage buffer to this shader composition.
        The slot is choosen from the constant buffer description.
        */
        virtual void Attach(const StorageBufferPtr& storageBuffer);

        //! Unbinds the specified storage buffer from this shader composition.
        virtual void Detach(const StorageBufferPtr& storageBuffer);
        //! Unbinds all previously bound storage buffers from this shader composition.
        virtual void DetachAllStorageBuffers();

        /**
        Creates the specified storage buffer, binds it to this shader composition and
        allocates the storage (i.e. calls "RenderSystem::WriteBuffer" with empty data).
        \param[in] name Specifies the storage buffer name in the shader.
        \param[in] usage Specifies the buffer usage. By default HardwareBufferUsage::Static.
        \param[in] isOptional Specifies whether this storage buffer is optional or not.
        If this is true, the function will return null if the storage buffer could not be found.
        If this is false, the function will throw a 'StorageBufferNotFoundException' exception
        if the storage buffer could not be found. By default false.
        \return Shared pointer to the new storage buffer or null if the 'isOptional' is true
        and the specified storage buffer could not be found.
        \throws StorageBufferNotFoundException If the specified storage buffer could not be found.
        \note This is a comfort function and uses the active render system.
        \see FindStorageBufferDesc
        \see RenderSystem::CreateStorageBuffer
        \see RenderSystem::WriteBuffer(StorageBuffer*, const void*, size_t, const HardwareBufferUsage)
        \see HardwareBufferUsage
        */
        StorageBufferPtr CreateAndAttachStorageBuffer(
            const std::string& name, const HardwareBufferUsage usage = HardwareBufferUsage::Static, bool isOptional = false
        );
        /**
        Creates the specified storage buffer, binds it to this shader composition and
        allocates the storage (i.e. calls "RenderSystem::WriteBuffer" with empty data).
        This function also appends the specified shader name to the expection message,
        if a "StorageBufferNotFoundException" expection will be thrown.
        \see CreateAndAttachStorageBuffer(const std::string&, const HardwareBufferUsage, bool)
        */
        StorageBufferPtr CreateAndAttachStorageBuffer(
            const std::string& name, const std::string& shaderName,
            const HardwareBufferUsage usage = HardwareBufferUsage::Static, bool isOptional = false
        );

        /* --- Event handler handling --- */

        /**
        Posts the 'update constant buffer' event.
        \param[in] renderContext Raw pointer to the render context to which this shader composition is bound.
        If 'renderContext' is null, this function call has no effect.
        \see SetEventHandler
        \see EventHandler
        */
        void PostUpdateConstantBuffer(RenderContext* renderContext) const;

        /**
        Posts the 'disassembly output' event.
        \param[in] shader Constant raw pointer to the shader whose code has been disassembled.
        \param[in] disassemblyOutput Specifies the disassembled shader assembly in text form.
        \see SetEventHandler
        \see EventHandler
        */
        void PostDisassemblyOutput(const Shader* shader, const std::string& disassemblyOutput) const;

        /* === Inline functions === */

        //! Returns the list of all bound constant buffers.
        inline const std::vector<ConstantBufferPtr>& GetConstantBuffers() const
        {
            return constantBuffers;
        }

        //! Returns true if this shader composition has already been compiled successful.
        inline bool HasCompiledSuccessful() const
        {
            return hasCompiledSuccessful;
        }

        /**
        Sets the shader composition event handler.
        \see EventHandler
        */
        inline void SetEventHandler(const EventHandlerPtr& eventHandler)
        {
            eventHandler_ = eventHandler;
        }

    protected:
        
        ShaderComposition();

        void PrintInfoLog(const std::string& infoLog, bool isError);

        bool hasCompiledSuccessful = false;

        std::vector<ConstantBufferPtr> constantBuffers;
        std::vector<StorageBufferPtr> storageBuffers;

    private:

        EventHandlerPtr eventHandler_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================