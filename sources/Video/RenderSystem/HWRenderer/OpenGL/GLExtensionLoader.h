/*
 * OpenGL extension loader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_EXTENSION_LOADER_H__
#define __FORK_GL_EXTENSION_LOADER_H__


#include <string>
#include <map>


namespace Fork
{

namespace Video
{

namespace GLExtensionLoader
{


//! OpenGL extension map type.
typedef std::map<std::string, bool> ExtMapType;

/* --- Common extension loading functions --- */

/**
Returns a hash-map with all supported OpenGL extensions.
The hash-map can be used for faster single-extension queries.
\param[in] useGLCoreProfile Specifies whether the extension are to be loaded via GL core profile or not.
*/
ExtMapType QueryExtensions(bool useGLCoreProfile);

/**
Loads all available extensions and prints errors if an extension is available,
but their respective functions could not be loaded.
\param[in,out] extMap Specifies the extension map. This can be queried by the "QueryExtensions" function.
If an extension is available but some of their respective functions could not be loaded,
the respective entry in the map will be invalidated (set to 'false').
\see QueryExtensions
*/
void LoadAllExtensions(ExtMapType& extMap);

/* --- Common GL extensions --- */

bool LoadSwapIntervalProcs();
bool LoadPixelFormatProcs();
bool LoadCreateContextProcs();

/* --- Hardware buffer extensions --- */

bool LoadVBOProcs();
bool LoadVAOProcs();
bool LoadFBOProcs();
bool LoadUBOProcs();
bool LoadSSBOProcs();

/* --- Drawing extensions --- */

bool LoadDrawBuffersProcs();
bool LoadInstancedProcs();
bool LoadInstancedOffsetProcs();
bool LoadBaseVertexProcs();

/* --- Shader extensions --- */

bool LoadShaderProcs();
bool LoadVertexAttribProcs();
bool LoadTessellationShaderProcs();
bool LoadComputeShaderProcs();
bool LoadProgramBinaryProcs();
bool LoadProgramInterfaceQueryProcs();

/* --- Texture extensions --- */

bool LoadMultiTextureProcs();
bool Load3DTextureProcs();
bool LoadClearTextureProcs();
bool LoadSamplerProcs();

/* --- Other extensions --- */

bool LoadQueryObjectProcs();
bool LoadViewportArrayProcs();
bool LoadDrawBuffersBlendProcs();
bool LoadMultiBindProcs();
bool LoadStencilSeparateProcs();
bool LoadDebugProcs();


} // /namespace GLExtensionLoader

} // /namespace Video

} // /namespace Fork


#endif



// ========================