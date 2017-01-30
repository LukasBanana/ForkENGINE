
# === CMake lists for the ForkENGINE - (23/02/2014) ===

cmake_minimum_required(VERSION 2.8)
project(ForkENGINE)


# === Build path ===

set(dir ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)


# === Preprocessor definitions ===

macro(add_define IDENT)
	if(MSVC)
		add_definitions(/D${IDENT})
	else(MSVC)
		add_definitions(-D${IDENT})
	endif(MSVC)
endmacro()

if(WIN32)
	add_define(_CRT_SECURE_NO_WARNINGS)
	add_define(_SCL_SECURE_NO_WARNINGS)
	add_define(UNICODE)
	add_define(NOMINMAX)
	if(MSVC)
		# Disable some warnings for MSVC compiler
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250 /wd4251 /wd4290 /wd4103")
	endif(MSVC)
endif(WIN32)


# === Global files ===

set(IncludePath include/fengine)
set(SourcesPath sources)
set(ShadersPath shaders)

file(GLOB_RECURSE IncludeCoreALL						${IncludePath}/Core/*.*)
file(GLOB_RECURSE SourcesCoreALL						${SourcesPath}/Core/*.*)

file(GLOB_RECURSE IncludeMathALL						${IncludePath}/Math/*.*)
file(GLOB_RECURSE SourcesMathALL						${SourcesPath}/Math/*.*)

file(GLOB_RECURSE IncludeIOALL							${IncludePath}/IO/*.*)
file(GLOB_RECURSE SourcesIOALL							${SourcesPath}/IO/*.*)

file(GLOB_RECURSE IncludeLangALL						${IncludePath}/Lang/*.*)
file(GLOB_RECURSE SourcesLangALL						${SourcesPath}/Lang/*.*)

file(GLOB_RECURSE IncludeSceneALL						${IncludePath}/Scene/*.*)
file(GLOB_RECURSE SourcesSceneALL						${SourcesPath}/Scene/*.*)

file(GLOB_RECURSE IncludeUtilityALL						${IncludePath}/Utility/*.*)
file(GLOB_RECURSE SourcesUtilityALL						${SourcesPath}/Utility/*.*)

file(GLOB_RECURSE IncludeAnimationALL					${IncludePath}/Animation/*.*)
file(GLOB_RECURSE SourcesAnimationALL					${SourcesPath}/Animation/*.*)

file(GLOB_RECURSE IncludeAudioALL						${IncludePath}/Audio/*.*)
file(GLOB_RECURSE SourcesAudioSoundSystemXAudio2ALL		${SourcesPath}/Audio/SoundSystem/XAudio2/*.*)

file(GLOB_RECURSE IncludeNetworkALL						${IncludePath}/Network/*.*)
file(GLOB_RECURSE SourcesNetworkALL						${SourcesPath}/Network/*.*)

file(GLOB_RECURSE IncludePhysicsALL						${IncludePath}/Physics/*.*)
file(GLOB_RECURSE SourcesPhysicsEngineNewtonALL			${SourcesPath}/Physics/Engine/Newton/*.*)

file(GLOB_RECURSE IncludeScriptALL						${IncludePath}/Script/*.*)
file(GLOB_RECURSE SourcesScriptLanguagePythonALL		${SourcesPath}/Script/Language/Python/*.*)

file(GLOB_RECURSE IncludeGUIALL							${IncludePath}/GUI/*.*)
file(GLOB_RECURSE SourcesGUIALL							${SourcesPath}/GUI/*.*)


file(GLOB IncludeCore									${IncludePath}/Core/*.*)
file(GLOB SourcesCore									${SourcesPath}/Core/*.*)
file(GLOB IncludeCoreContainer							${IncludePath}/Core/Container/*.*)
file(GLOB SourcesCoreContainer							${SourcesPath}/Core/Container/*.*)
file(GLOB IncludeCoreException							${IncludePath}/Core/Exception/*.*)
file(GLOB IncludeCoreTreeHierarchy						${IncludePath}/Core/TreeHierarchy/*.*)

file(GLOB SourcesMath									${SourcesPath}/Math/*.*)
file(GLOB IncludeMathCore								${IncludePath}/Math/Core/*.*)
file(GLOB IncludeMathCoreArithmetic						${IncludePath}/Math/Core/Arithmetic/*.*)
file(GLOB IncludeMathGeometry							${IncludePath}/Math/Geometry/*.*)
file(GLOB IncludeMathCommon								${IncludePath}/Math/Common/*.*)
file(GLOB IncludeMathCollision							${IncludePath}/Math/Collision/*.*)

file(GLOB IncludeIOCore									${IncludePath}/IO/Core/*.*)
file(GLOB SourcesIOCore									${SourcesPath}/IO/Core/*.*)
file(GLOB IncludeIOCrypto								${IncludePath}/IO/Crypto/*.*)
file(GLOB SourcesIOCrypto								${SourcesPath}/IO/Crypto/*.*)
file(GLOB IncludeIOFileSystem							${IncludePath}/IO/FileSystem/*.*)
file(GLOB SourcesIOFileSystem							${SourcesPath}/IO/FileSystem/*.*)
file(GLOB IncludeIOInputDevice							${IncludePath}/IO/InputDevice/*.*)
file(GLOB SourcesIOInputDevice							${SourcesPath}/IO/InputDevice/*.*)

file(GLOB IncludeLang									${IncludePath}/Lang/*.*)
file(GLOB IncludeLangSyntaxAnalyzer						${IncludePath}/Lang/SyntaxAnalyzer/*.*)
file(GLOB SourcesLangSyntaxAnalyzer						${SourcesPath}/Lang/SyntaxAnalyzer/*.*)
file(GLOB IncludeLangAST								${IncludePath}/Lang/AbstractSyntaxTrees/*.*)
file(GLOB SourcesLangAST								${SourcesPath}/Lang/AbstractSyntaxTrees/*.*)
file(GLOB IncludeLangASTFSC								${IncludePath}/Lang/AbstractSyntaxTrees/FSC/*.*)
file(GLOB IncludeLangFSCInterpreter						${IncludePath}/Lang/FSCInterpreter/*.*)
file(GLOB SourcesLangFSCInterpreter						${SourcesPath}/Lang/FSCInterpreter/*.*)
file(GLOB IncludeLangFSCInterpreterIMR					${IncludePath}/Lang/FSCInterpreter/IMR/*.*)
file(GLOB IncludeLangXMLParser							${IncludePath}/Lang/XMLParser/*.*)
file(GLOB SourcesLangXMLParser							${SourcesPath}/Lang/XMLParser/*.*)

file(GLOB IncludeVideoCore								${IncludePath}/Video/Core/*.*)
file(GLOB SourcesVideoCore								${SourcesPath}/Video/Core/*.*)
file(GLOB IncludeVideoImage								${IncludePath}/Video/Image/*.*)
file(GLOB SourcesVideoImage								${SourcesPath}/Video/Image/*.*)
file(GLOB IncludeVideoFont								${IncludePath}/Video/Font/*.*)
file(GLOB SourcesVideoFont								${SourcesPath}/Video/Font/*.*)
file(GLOB IncludeVideoFileHandler						${IncludePath}/Video/FileHandler/*.*)
file(GLOB SourcesVideoFileHandler						${SourcesPath}/Video/FileHandler/*.*)
file(GLOB IncludeVideoAdvancedRenderer					${IncludePath}/Video/AdvancedRenderer/*.*)
file(GLOB SourcesVideoAdvancedRenderer					${SourcesPath}/Video/AdvancedRenderer/*.*)
file(GLOB IncludeVideoPostProcessor						${IncludePath}/Video/AdvancedRenderer/PostProcessor/*.*)
file(GLOB SourcesVideoPostProcessor						${SourcesPath}/Video/AdvancedRenderer/PostProcessor/*.*)
file(GLOB IncludeVideoRenderSystem						${IncludePath}/Video/RenderSystem/*.*)
file(GLOB SourcesVideoRenderSystem						${SourcesPath}/Video/RenderSystem/*.*)
file(GLOB IncludeVideoRenderSystemHardwareBuffer		${IncludePath}/Video/RenderSystem/HardwareBuffer/*.*)
file(GLOB SourcesVideoRenderSystemHardwareBuffer		${SourcesPath}/Video/RenderSystem/HardwareBuffer/*.*)
file(GLOB IncludeVideoRenderSystemShader				${IncludePath}/Video/RenderSystem/Shader/*.*)
file(GLOB SourcesVideoRenderSystemShader				${SourcesPath}/Video/RenderSystem/Shader/*.*)
file(GLOB IncludeVideoRenderSystemShaderStandardShader	${IncludePath}/Video/RenderSystem/Shader/StandardShader/*.*)
file(GLOB IncludeVideoRenderSystemTexture				${IncludePath}/Video/RenderSystem/Texture/*.*)
file(GLOB SourcesVideoRenderSystemTexture				${SourcesPath}/Video/RenderSystem/Texture/*.*)
file(GLOB IncludeVideoRenderSystemRenderState			${IncludePath}/Video/RenderSystem/RenderState/*.*)
file(GLOB SourcesVideoRenderSystemRenderState			${SourcesPath}/Video/RenderSystem/RenderState/*.*)
file(GLOB IncludeVideoBufferFormat						${IncludePath}/Video/BufferFormat/*.*)
file(GLOB SourcesVideoBufferFormat						${SourcesPath}/Video/BufferFormat/*.*)
file(GLOB IncludeVideoMonitor							${IncludePath}/Video/Monitor/*.*)
file(GLOB SourcesVideoMonitor							${SourcesPath}/Video/Monitor/*.*)
file(GLOB IncludeVideoMaterial							${IncludePath}/Video/Material/*.*)
file(GLOB SourcesVideoMaterial							${SourcesPath}/Video/Material/*.*)

file(GLOB IncludePlatformCore							${IncludePath}/Platform/Core/*.*)
file(GLOB IncludePlatformVideo							${IncludePath}/Platform/Video/*.*)
file(GLOB IncludePlatformNetwork						${IncludePath}/Platform/Network/*.*)

file(GLOB SourcesVideoRenderSystemGL					${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/*.*)
file(GLOB SourcesVideoRenderSystemGLHardwareBuffer		${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/HardwareBuffer/*.*)
file(GLOB SourcesVideoRenderSystemGLShader				${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/Shader/*.*)
file(GLOB SourcesVideoRenderSystemGLTexture				${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/Texture/*.*)
file(GLOB SourcesVideoRenderSystemGLRenderState			${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/RenderState/*.*)

file(GLOB SourcesVideoRenderSystemD3D11					${SourcesPath}/Video/RenderSystem/HWRenderer/Direct3D11/*.*)
file(GLOB SourcesVideoRenderSystemD3D11HardwareBuffer	${SourcesPath}/Video/RenderSystem/HWRenderer/Direct3D11/HardwareBuffer/*.*)
file(GLOB SourcesVideoRenderSystemD3D11Shader			${SourcesPath}/Video/RenderSystem/HWRenderer/Direct3D11/Shader/*.*)
file(GLOB SourcesVideoRenderSystemD3D11Texture			${SourcesPath}/Video/RenderSystem/HWRenderer/Direct3D11/Texture/*.*)
file(GLOB SourcesVideoRenderSystemD3D11RenderState		${SourcesPath}/Video/RenderSystem/HWRenderer/Direct3D11/RenderState/*.*)

file(GLOB IncludeEngine									${IncludePath}/Engine/*.*)
file(GLOB SourcesEngine									${SourcesPath}/Engine/*.*)
file(GLOB IncludeEngineComponent						${IncludePath}/Engine/Component/*.*)
file(GLOB SourcesEngineComponent						${SourcesPath}/Engine/Component/*.*)
file(GLOB SourcesEngineComponentProperty				${SourcesPath}/Engine/Component/Property/*.*)
file(GLOB IncludeEngineGame								${IncludePath}/Engine/Game/*.*)
file(GLOB SourcesEngineGame								${SourcesPath}/Engine/Game/*.*)
file(GLOB IncludeEngineAsset							${IncludePath}/Engine/Asset/*.*)
file(GLOB SourcesEngineAsset							${SourcesPath}/Engine/Asset/*.*)
file(GLOB IncludeEngineFileHandler						${IncludePath}/Engine/FileHandler/*.*)
file(GLOB SourcesEngineFileHandler						${SourcesPath}/Engine/FileHandler/*.*)

file(GLOB IncludeSceneFileHandler						${IncludePath}/Scene/FileHandler/*.*)
file(GLOB SourcesSceneFileHandler						${SourcesPath}/Scene/FileHandler/*.*)
file(GLOB IncludeScene									${IncludePath}/Scene/*.*)
file(GLOB SourcesScene									${SourcesPath}/Scene/*.*)
file(GLOB IncludeSceneNode								${IncludePath}/Scene/Node/*.*)
file(GLOB SourcesSceneNode								${SourcesPath}/Scene/Node/*.*)
file(GLOB IncludeSceneManager							${IncludePath}/Scene/Manager/*.*)
file(GLOB SourcesSceneManager							${SourcesPath}/Scene/Manager/*.*)
file(GLOB IncludeSceneRenderer							${IncludePath}/Scene/Renderer/*.*)
file(GLOB SourcesSceneRenderer							${SourcesPath}/Scene/Renderer/*.*)
file(GLOB IncludeSceneGeometry							${IncludePath}/Scene/Geometry/*.*)
file(GLOB SourcesSceneGeometry							${SourcesPath}/Scene/Geometry/*.*)
file(GLOB IncludeSceneGeometryNode						${IncludePath}/Scene/Geometry/Node/*.*)
file(GLOB SourcesSceneGeometryNode						${SourcesPath}/Scene/Geometry/Node/*.*)
file(GLOB IncludeSceneGeometryGenerator					${IncludePath}/Scene/Geometry/Generator/*.*)
file(GLOB SourcesSceneGeometryGenerator					${SourcesPath}/Scene/Geometry/Generator/*.*)
file(GLOB IncludeSceneGeometryModifier					${IncludePath}/Scene/Geometry/Modifier/*.*)
file(GLOB SourcesSceneGeometryModifier					${SourcesPath}/Scene/Geometry/Modifier/*.*)
file(GLOB SourcesSceneGeometryOptimizer					${SourcesPath}/Scene/Geometry/Optimizer/*.*)
file(GLOB IncludeSceneLightSource						${IncludePath}/Scene/LightSource/*.*)
file(GLOB SourcesSceneLightSource						${SourcesPath}/Scene/LightSource/*.*)

file(GLOB IncludeUtility								${IncludePath}/Utility/*.*)
file(GLOB SourcesUtility								${SourcesPath}/Utility/*.*)
file(GLOB IncludeUtilityTerminal						${IncludePath}/Utility/Terminal/*.*)
file(GLOB SourcesUtilityTerminal						${SourcesPath}/Utility/Terminal/*.*)
file(GLOB SourcesUtilityTerminalStandardCommand			${SourcesPath}/Utility/Terminal/StandardCommand/*.*)
file(GLOB IncludeUtilitySelector						${IncludePath}/Utility/Selector/*.*)
file(GLOB SourcesUtilitySelector						${SourcesPath}/Utility/Selector/*.*)
file(GLOB IncludeUtilityProjector						${IncludePath}/Utility/Projector/*.*)
file(GLOB SourcesUtilityProjector						${SourcesPath}/Utility/Projector/*.*)
file(GLOB IncludeUtilityHTMLRenderer					${IncludePath}/Utility/HTMLRenderer/*.*)
file(GLOB SourcesUtilityHTMLRenderer					${SourcesPath}/Utility/HTMLRenderer/*.*)

file(GLOB IncludeGUI									${IncludePath}/GUI/*.*)
file(GLOB SourcesGUI									${SourcesPath}/GUI/*.*)
file(GLOB IncludeGUICore								${IncludePath}/GUI/Core/*.*)
file(GLOB SourcesGUICore								${SourcesPath}/GUI/Core/*.*)
file(GLOB IncludeGUIEventSystem							${IncludePath}/GUI/EventSystem/*.*)
file(GLOB SourcesGUIEventSystem							${SourcesPath}/GUI/EventSystem/*.*)
file(GLOB IncludeGUIViewController						${IncludePath}/GUI/ViewController/*.*)
file(GLOB SourcesGUIViewController						${SourcesPath}/GUI/ViewController/*.*)

file(GLOB IncludeAnimationCore							${IncludePath}/Animation/Core/*.*)
file(GLOB SourcesAnimationCore							${SourcesPath}/Animation/Core/*.*)
file(GLOB IncludeAnimationSystem						${IncludePath}/Animation/AnimationSystem/*.*)
file(GLOB SourcesAnimationSystem						${SourcesPath}/Animation/AnimationSystem/*.*)

file(GLOB IncludeNetworkCore							${IncludePath}/Network/Core/*.*)
file(GLOB SourcesNetworkCore							${SourcesPath}/Network/Core/*.*)
file(GLOB IncludeNetworkProtocol						${IncludePath}/Network/Protocol/*.*)
file(GLOB SourcesNetworkProtocol						${SourcesPath}/Network/Protocol/*.*)
file(GLOB IncludeNetworkSystemDaemon					${IncludePath}/Network/System/Daemon/*.*)
file(GLOB SourcesNetworkSystemDaemon					${SourcesPath}/Network/System/Daemon/*.*)
file(GLOB IncludeNetworkSystemSession					${IncludePath}/Network/System/Session/*.*)
file(GLOB SourcesNetworkSystemSession					${SourcesPath}/Network/System/Session/*.*)

file(GLOB IncludePhysics								${IncludePath}/Physics/*.*)
file(GLOB SourcesPhysics								${SourcesPath}/Physics/*.*)
file(GLOB IncludePhysicsDynamic							${IncludePath}/Physics/Dynamic/*.*)
file(GLOB SourcesPhysicsDynamic							${SourcesPath}/Physics/Dynamic/*.*)
file(GLOB IncludePhysicsKinematic						${IncludePath}/Physics/Kinematic/*.*)
file(GLOB SourcesPhysicsKinematic						${SourcesPath}/Physics/Kinematic/*.*)
file(GLOB SourcesPhysicsNewton							${SourcesPath}/Physics/Engine/Newton/*.*)
file(GLOB SourcesPhysicsNewtonDynamic					${SourcesPath}/Physics/Engine/Newton/Dynamic/*.*)
file(GLOB SourcesPhysicsNewtonKinematic					${SourcesPath}/Physics/Engine/Newton/Kinematic/*.*)

file(GLOB IncludeAudioCore								${IncludePath}/Audio/Core/*.*)
file(GLOB SourcesAudioCore								${SourcesPath}/Audio/Core/*.*)
file(GLOB IncludeAudioFileHandler						${IncludePath}/Audio/FileHandler/*.*)
file(GLOB SourcesAudioFileHandler						${SourcesPath}/Audio/FileHandler/*.*)
file(GLOB IncludeAudioSoundSystem						${IncludePath}/Audio/SoundSystem/*.*)
file(GLOB SourcesAudioSoundSystem						${SourcesPath}/Audio/SoundSystem/*.*)
file(GLOB SourcesAudioSoundSystemXAudio2				${SourcesPath}/Audio/SoundSystem/XAudio2/*.*)

file(GLOB IncludeScript									${IncludePath}/Script/*.*)
file(GLOB SourcesScript									${SourcesPath}/Script/*.*)
file(GLOB SourcesScriptPython							${SourcesPath}/Script/Language/Python/*.*)
file(GLOB SourcesScriptPythonModule						${SourcesPath}/Script/Language/Python/Module/*.*)

file(GLOB SourcesEditor									${SourcesPath}/Editor/*.*)
file(GLOB SourcesEditorCore								${SourcesPath}/Editor/Core/*.*)
file(GLOB SourcesEditorCoreComponent					${SourcesPath}/Editor/Core/Component/*.*)
file(GLOB SourcesEditorCoreComponentUIProperty			${SourcesPath}/Editor/Core/Component/UIProperty/*.*)
file(GLOB SourcesEditorCoreTimeline						${SourcesPath}/Editor/Core/Timeline/*.*)
file(GLOB SourcesEditorCoreUIExtensions					${SourcesPath}/Editor/Core/UIExtensions/*.*)
file(GLOB SourcesEditorEntity							${SourcesPath}/Editor/Entity/*.*)
file(GLOB SourcesEditorFrame							${SourcesPath}/Editor/Frame/*.*)
file(GLOB SourcesEditorFrameConfig						${SourcesPath}/Editor/Frame/Config/*.*)
file(GLOB SourcesEditorFrameConsole						${SourcesPath}/Editor/Frame/Console/*.*)
file(GLOB SourcesEditorFrameHierarchyViewer				${SourcesPath}/Editor/Frame/HierarchyViewer/*.*)
file(GLOB SourcesEditorFrameMain						${SourcesPath}/Editor/Frame/Main/*.*)
file(GLOB SourcesEditorLanguage							${SourcesPath}/Editor/Language/*.*)
file(GLOB SourcesEditorProject							${SourcesPath}/Editor/Project/*.*)
file(GLOB SourcesEditorTools							${SourcesPath}/Editor/Tools/*.*)
file(GLOB SourcesEditorToolsAssetBrowser				${SourcesPath}/Editor/Tools/AssetBrowser/*.*)
file(GLOB SourcesEditorToolsAssetBrowserAsset			${SourcesPath}/Editor/Tools/AssetBrowser/Asset/*.*)

file(GLOB SourcesDaemon									${SourcesPath}/Daemon/*.*)
file(GLOB SourcesDaemonService							${SourcesPath}/Daemon/Service/*.*)

file(GLOB ShadersHLSL									${ShadersPath}/*.hlsl)
file(GLOB ShadersGLSL									${ShadersPath}/*.glsl)

file(GLOB SourcesPlatformWindowsCore					${SourcesPath}/Platform/Windows/Core/*.*)
file(GLOB SourcesPlatformWindowsVideo					${SourcesPath}/Platform/Windows/Video/*.*)
file(GLOB SourcesPlatformWindowsNetwork					${SourcesPath}/Platform/Windows/Network/*.*)
file(GLOB SourcesGLPlatformWindows						${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/Platform/Windows/*.*)

file(GLOB SourcesPlatformPosixCore						${SourcesPath}/Platform/Posix/Core/*.*)
file(GLOB SourcesPlatformPosixVideo						${SourcesPath}/Platform/Posix/Video/*.*)
file(GLOB SourcesPlatformPosixNetwork					${SourcesPath}/Platform/Posix/Network/*.*)
file(GLOB SourcesGLPlatformPosix						${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/Platform/Posix/*.*)

file(GLOB SourcesPlatformOSXCore						${SourcesPath}/Platform/OSX/Core/*.*)
file(GLOB SourcesPlatformOSXVideo						${SourcesPath}/Platform/OSX/Video/*.*)
file(GLOB SourcesPlatformOSXNetwork						${SourcesPath}/Platform/OSX/Network/*.*)
file(GLOB SourcesGLPlatformOSX							${SourcesPath}/Video/RenderSystem/HWRenderer/OpenGL/Platform/OSX/*.*)

if(WIN32)
	set(SourcesPlatformCore ${SourcesPlatformWindowsCore})
	set(SourcesPlatformVideo ${SourcesPlatformWindowsVideo})
	set(SourcesPlatformNetwork ${SourcesPlatformWindowsNetwork})
	set(SourcesGLPlatform ${SourcesGLPlatformWindows})
	set(EditorResourceFile ${SourcesPath}/Editor/Resources/Resources.rc)
elseif(UNIX)
	set(SourcesPlatformCore ${SourcesPlatformPosixCore})
	set(SourcesPlatformVideo ${SourcesPlatformPosixVideo})
	set(SourcesPlatformNetwork ${SourcesPlatformPosixNetwork})
	set(SourcesGLPlatform ${SourcesGLPlatformPosix})
elseif(APPLE)
	set(SourcesPlatformCore ${SourcesPlatformOSXCore})
	set(SourcesPlatformVideo ${SourcesPlatformOSXVideo})
	set(SourcesPlatformNetwork ${SourcesPlatformOSXNetwork})
	set(SourcesGLPlatform ${SourcesGLPlatformOSX})
#else()
#	error !
endif()

file(GLOB FilesPluginsJPEG ${SourcesPath}/Plugins/jpeglib/*.*)


# === Source group folders ===

# ------- "Core" -------

source_group(
	"Core" FILES
	${IncludeCore}
	${SourcesCore}
)

source_group(
	"Core\\Container" FILES
	${IncludeCoreContainer}
	${SourcesCoreContainer}
)

source_group(
	"Core\\Exception" FILES
	${IncludeCoreException}
)

source_group(
	"Core\\TreeHierarchy" FILES
	${IncludeCoreTreeHierarchy}
)

# ------- "Math" -------

source_group(
	"Math\\Core" FILES
	${IncludeMathCore}
)

source_group(
	"Math\\Core\\Arithmetic" FILES
	${IncludeMathCoreArithmetic}
	${SourcesPath}/Math/BaseArithmetic.cpp
	${SourcesPath}/Math/BitArithmetic.cpp
)

source_group(
	"Math\\Geometry" FILES
	${IncludeMathGeometry}
)

source_group(
	"Math\\Common" FILES
	${IncludeMathCommon}
	${SourcesPath}/Math/AprxMathFunctions.cpp
)

source_group(
	"Math\\Collision" FILES
	${IncludeMathCollision}
)

# ------- "IO" -------

source_group(
	"IO\\Core" FILES
	${IncludeIOCore}
	${SourcesIOCore}
)

source_group(
	"IO\\Crypto" FILES
	${IncludeIOCrypto}
	${SourcesIOCrypto}
)

source_group(
	"IO\\FileSystem" FILES
	${IncludeIOFileSystem}
	${SourcesIOFileSystem}
)

source_group(
	"IO\\InputDevice" FILES
	${IncludeIOInputDevice}
	${SourcesIOInputDevice}
)

# ------- "Platform" -------

source_group(
	"Platform\\Core" FILES
	${IncludePlatformCore}
	${SourcesPlatformCore}
)

source_group(
	"Platform\\Video" FILES
	${IncludePlatformVideo}
	${SourcesPlatformVideo}
)

source_group(
	"Platform\\Network" FILES
	${IncludePlatformNetwork}
	${SourcesPlatformNetwork}
)

# ------- "Lang" -------

source_group(
	"Lang" FILES
	${IncludeLang}
)

source_group(
	"Lang\\SyntaxAnalyzer" FILES
	${IncludeLangSyntaxAnalyzer}
	${SourcesLangSyntaxAnalyzer}
)

source_group(
	"Lang\\AbstractSyntaxTrees" FILES
	${IncludeLangAST}
	${SourcesLangAST}
)

source_group(
	"Lang\\AbstractSyntaxTrees\\FSC" FILES
	${IncludeLangASTFSC}
)

source_group(
	"Lang\\FSCInterpreter" FILES
	${IncludeLangFSCInterpreter}
	${SourcesLangFSCInterpreter}
)

source_group(
	"Lang\\FSCInterpreter\\IMR" FILES
	${IncludeLangFSCInterpreterIMR}
)

source_group(
	"Lang\\XMLParser" FILES
	${IncludeLangXMLParser}
	${SourcesLangXMLParser}
)

# ------- "Video" -------

source_group(
	"Video\\Core" FILES
	${IncludeVideoCore}
	${SourcesVideoCore}
)

source_group(
	"Video\\Image" FILES
	${IncludeVideoImage}
	${SourcesVideoImage}
)

source_group(
	"Video\\Font" FILES
	${IncludeVideoFont}
	${SourcesVideoFont}
)

source_group(
	"Video\\FileHandler" FILES
	${IncludeVideoFileHandler}
	${SourcesVideoFileHandler}
)

source_group(
	"Shaders\\HLSL" FILES
	${ShadersHLSL}
)

source_group(
	"Shaders\\GLSL" FILES
	${ShadersGLSL}
)

source_group(
	"Video\\BufferFormat" FILES
	${IncludeVideoBufferFormat}
	${SourcesVideoBufferFormat}
)

source_group(
	"Video\\AdvancedRenderer" FILES
	${IncludeVideoAdvancedRenderer}
	${SourcesVideoAdvancedRenderer}
)

source_group(
	"Video\\AdvancedRenderer\\PostProcessor" FILES
	${IncludeVideoPostProcessor}
	${SourcesVideoPostProcessor}
)

source_group(
	"Video\\RenderSystem" FILES
	${IncludeVideoRenderSystem}
	${SourcesVideoRenderSystem}
)

source_group(
	"Video\\RenderSystem\\HardwareBuffer" FILES
	${IncludeVideoRenderSystemHardwareBuffer}
	${SourcesVideoRenderSystemHardwareBuffer}
)

source_group(
	"Video\\RenderSystem\\Shader" FILES
	${IncludeVideoRenderSystemShader}
	${SourcesVideoRenderSystemShader}
)

source_group(
	"Video\\RenderSystem\\Shader\\StandardShader" FILES
	${IncludeVideoRenderSystemShaderStandardShader}
)

source_group(
	"Video\\RenderSystem\\Texture" FILES
	${IncludeVideoRenderSystemTexture}
	${SourcesVideoRenderSystemTexture}
)

source_group(
	"Video\\RenderSystem\\RenderState" FILES
	${IncludeVideoRenderSystemRenderState}
	${SourcesVideoRenderSystemRenderState}
)

source_group(
	"Video\\Monitor" FILES
	${IncludeVideoMonitor}
	${SourcesVideoMonitor}
)

source_group(
	"Video\\Material" FILES
	${IncludeVideoMaterial}
	${SourcesVideoMaterial}
)

source_group(
	"OpenGL" FILES
	${SourcesVideoRenderSystemGL}
)

source_group(
	"OpenGL\\HardwareBuffer" FILES
	${SourcesVideoRenderSystemGLHardwareBuffer}
)

source_group(
	"OpenGL\\Shader" FILES
	${SourcesVideoRenderSystemGLShader}
)

source_group(
	"OpenGL\\Texture" FILES
	${SourcesVideoRenderSystemGLTexture}
)

source_group(
	"OpenGL\\RenderState" FILES
	${SourcesVideoRenderSystemGLRenderState}
)

source_group(
	"OpenGL\\Platform" FILES
	${SourcesGLPlatform}
)

source_group(
	"Direct3D11" FILES
	${SourcesVideoRenderSystemD3D11}
)

source_group(
	"Direct3D11\\HardwareBuffer" FILES
	${SourcesVideoRenderSystemD3D11HardwareBuffer}
)

source_group(
	"Direct3D11\\Shader" FILES
	${SourcesVideoRenderSystemD3D11Shader}
)

source_group(
	"Direct3D11\\Texture" FILES
	${SourcesVideoRenderSystemD3D11Texture}
)

source_group(
	"Direct3D11\\RenderState" FILES
	${SourcesVideoRenderSystemD3D11RenderState}
)

# ------- "Scene" -------

source_group(
	"Scene" FILES
	${IncludeScene}
	${SourcesScene}
)

source_group(
	"Scene\\FileHandler" FILES
	${IncludeSceneFileHandler}
	${SourcesSceneFileHandler}
)

source_group(
	"Scene\\Node" FILES
	${IncludeSceneNode}
	${SourcesSceneNode}
)

source_group(
	"Scene\\Geometry" FILES
	${IncludeSceneGeometry}
	${SourcesSceneGeometry}
)

source_group(
	"Scene\\Geometry\\Node" FILES
	${IncludeSceneGeometryNode}
	${SourcesSceneGeometryNode}
)

source_group(
	"Scene\\Geometry\\Generator" FILES
	${IncludeSceneGeometryGenerator}
	${SourcesSceneGeometryGenerator}
)

source_group(
	"Scene\\Geometry\\Modifier" FILES
	${IncludeSceneGeometryModifier}
	${SourcesSceneGeometryModifier}
)

source_group(
	"Scene\\Geometry\\Optimizer" FILES
	${SourcesSceneGeometryOptimizer}
)

source_group(
	"Scene\\LightSource" FILES
	${IncludeSceneLightSource}
	${SourcesSceneLightSource}
)

source_group(
	"Scene\\Manager" FILES
	${IncludeSceneManager}
	${SourcesSceneManager}
)

source_group(
	"Scene\\Renderer" FILES
	${IncludeSceneRenderer}
	${SourcesSceneRenderer}
)

# ------- "Utility" -------

source_group(
	"Utility" FILES
	${IncludeUtility}
	${SourcesUtility}
)

source_group(
	"Utility\\Terminal" FILES
	${IncludeUtilityTerminal}
	${SourcesUtilityTerminal}
)

source_group(
	"Utility\\Terminal\\StandardCommand" FILES
	${SourcesUtilityTerminalStandardCommand}
)

source_group(
	"Utility\\Selector" FILES
	${IncludeUtilitySelector}
	${SourcesUtilitySelector}
)

source_group(
	"Utility\\Projector" FILES
	${IncludeUtilityProjector}
	${SourcesUtilityProjector}
)

source_group(
	"Utility\\HTMLRenderer" FILES
	${IncludeUtilityHTMLRenderer}
	${SourcesUtilityHTMLRenderer}
)

# ------- "GUI" -------

source_group(
	"GUI" FILES
	${IncludeGUI}
	${SourcesGUI}
)

source_group(
	"GUI\\Core" FILES
	${IncludeGUICore}
	${SourcesGUICore}
)

source_group(
	"GUI\\EventSystem" FILES
	${IncludeGUIEventSystem}
	${SourcesGUIEventSystem}
)

source_group(
	"GUI\\ViewController" FILES
	${IncludeGUIViewController}
	${SourcesGUIViewController}
)

# ------- "Animation" -------

source_group(
	"Animation\\Core" FILES
	${IncludeAnimationCore}
	${SourcesAnimationCore}
)

source_group(
	"Animation\\AnimationSystem" FILES
	${IncludeAnimationSystem}
	${SourcesAnimationSystem}
)

# ------- "Network" -------

source_group(
	"Network\\Core" FILES
	${IncludeNetworkCore}
	${SourcesNetworkCore}
)

source_group(
	"Network\\System\\Session" FILES
	${IncludeNetworkSystemSession}
	${SourcesNetworkSystemSession}
)

source_group(
	"Network\\System\\Daemon" FILES
	${IncludeNetworkSystemDaemon}
	${SourcesNetworkSystemDaemon}
)

source_group(
	"Network\\Protocol" FILES
	${IncludeNetworkProtocol}
	${SourcesNetworkProtocol}
)

# ------- "Physics" -------

source_group(
	"Physics" FILES
	${IncludePhysics}
	${SourcesPhysics}
)

source_group(
	"Physics\\Dynamic" FILES
	${IncludePhysicsDynamic}
	${SourcesPhysicsDynamic}
)

source_group(
	"Physics\\Kinematic" FILES
	${IncludePhysicsKinematic}
	${SourcesPhysicsKinematic}
)

source_group(
	"Physics\\Engine\\Newton" FILES
	${SourcesPhysicsNewton}
)

source_group(
	"Physics\\Engine\\Newton\\Dynamic" FILES
	${SourcesPhysicsNewtonDynamic}
)

source_group(
	"Physics\\Engine\\Newton\\Kinematic" FILES
	${SourcesPhysicsNewtonKinematic}
)

# ------- "Script" -------

source_group(
	"Script" FILES
	${IncludeScript}
	${SourcesScript}
)

source_group(
	"Script\\Language\\Python" FILES
	${SourcesScriptPython}
)

source_group(
	"Script\\Language\\Python\\Module" FILES
	${SourcesScriptPythonModule}
)

# ------- "Audio" -------

source_group(
	"Audio\\Core" FILES
	${IncludeAudioCore}
	${SourcesAudioCore}
)

source_group(
	"Audio\\FileHandler" FILES
	${IncludeAudioFileHandler}
	${SourcesAudioFileHandler}
)

source_group(
	"Audio\\SoundSystem" FILES
	${IncludeAudioSoundSystem}
	${SourcesAudioSoundSystem}
)

source_group(
	"Audio\\SoundSystem\\XAudio2" FILES
	${SourcesAudioSoundSystemXAudio2}
)

# ------- "Engine" -------

source_group(
	"Engine" FILES
	${IncludeEngine}
	${SourcesEngine}
)

source_group(
	"Engine\\Component" FILES
	${IncludeEngineComponent}
	${SourcesEngineComponent}
)

source_group(
	"Engine\\Component\\Property" FILES
	${SourcesEngineComponentProperty}
)

source_group(
	"Engine\\Game" FILES
	${IncludeEngineGame}
	${SourcesEngineGame}
)

source_group(
	"Engine\\Asset" FILES
	${IncludeEngineAsset}
	${SourcesEngineAsset}
)

source_group(
	"Engine\\FileHandler" FILES
	${IncludeEngineFileHandler}
	${SourcesEngineFileHandler}
)

# ------- "Editor" -------

source_group("Editor" FILES ${SourcesEditor})
source_group("Editor\\Core" FILES ${SourcesEditorCore})
source_group("Editor\\Core\\Component" FILES ${SourcesEditorCoreComponent})
source_group("Editor\\Core\\Component\\UIProperty" FILES ${SourcesEditorCoreComponentUIProperty})
source_group("Editor\\Core\\Timeline" FILES ${SourcesEditorCoreTimeline})
source_group("Editor\\Core\\UIExtensions" FILES ${SourcesEditorCoreUIExtensions})
source_group("Editor\\Entity" FILES ${SourcesEditorEntity})
source_group("Editor\\Frame" FILES ${SourcesEditorFrame})
source_group("Editor\\Frame\\Config" FILES ${SourcesEditorFrameConfig})
source_group("Editor\\Frame\\Console" FILES ${SourcesEditorFrameConsole})
source_group("Editor\\Frame\\HierarchyViewer" FILES ${SourcesEditorFrameHierarchyViewer})
source_group("Editor\\Frame\\Main" FILES ${SourcesEditorFrameMain})
source_group("Editor\\Language" FILES ${SourcesEditorLanguage})
source_group("Editor\\Project" FILES ${SourcesEditorProject})
source_group("Editor\\Tools" FILES ${SourcesEditorTools})
source_group("Editor\\Tools\\AssetBrowser" FILES ${SourcesEditorToolsAssetBrowser})
source_group("Editor\\Tools\\AssetBrowser\\Asset" FILES ${SourcesEditorToolsAssetBrowserAsset})

if(WIN32)
	source_group("Editor\\Core\\Resources" FILES ${EditorResourceFile})
endif(WIN32)

# ------- "Daemon" -------

source_group(
	"Daemon" FILES
	${SourcesDaemon}
)

source_group(
	"Daemon\\Service" FILES
	${SourcesDaemonService}
)


# === Include directories ===

include_directories(
	"${PROJECT_SOURCE_DIR}/${IncludePath}"
	"${PROJECT_SOURCE_DIR}/include"
)


# === Executable ===

set(
	FilesCore
	
	${IncludeCoreALL}
	${SourcesCoreALL}
	
	${IncludeMathALL}
	${SourcesMathALL}
	
	${IncludeIOALL}
	${SourcesIOALL}
	
	${IncludeLangALL}
	${SourcesLangALL}
	
	${IncludePlatformCore}
	${SourcesPlatformCore}
	
	${IncludeVideoCore}
	${SourcesVideoCore}
	
	${IncludeVideoImage}
	${SourcesVideoImage}
)

set(
	FilesRenderer
	
	${IncludeVideoFont}
	${SourcesVideoFont}
	
	${IncludeVideoFileHandler}
	${SourcesVideoFileHandler}
	
	${IncludePlatformVideo}
	${SourcesPlatformVideo}
	
	${IncludeVideoAdvancedRenderer}
	${SourcesVideoAdvancedRenderer}
	
	${IncludeVideoPostProcessor}
	${SourcesVideoPostProcessor}
	
	${IncludeVideoRenderSystem}
	${SourcesVideoRenderSystem}
	
	${IncludeVideoRenderSystemHardwareBuffer}
	${SourcesVideoRenderSystemHardwareBuffer}
	
	${IncludeVideoRenderSystemShader}
	${SourcesVideoRenderSystemShader}
	
	${IncludeVideoRenderSystemShaderStandardShader}
	
	${IncludeVideoRenderSystemTexture}
	${SourcesVideoRenderSystemTexture}
	
	${IncludeVideoRenderSystemRenderState}
	${SourcesVideoRenderSystemRenderState}
	
	${IncludeVideoBufferFormat}
	${SourcesVideoBufferFormat}
	
	${IncludeVideoMonitor}
	${SourcesVideoMonitor}
	
	${IncludeVideoMaterial}
	${SourcesVideoMaterial}
	
#	${ShadersHLSL}
	${ShadersGLSL}
)

set(
	FilesRendererGL
	
	${SourcesVideoRenderSystemGL}
	${SourcesVideoRenderSystemGLHardwareBuffer}
	${SourcesVideoRenderSystemGLShader}
	${SourcesVideoRenderSystemGLStandardShader}
	${SourcesVideoRenderSystemGLTexture}
	${SourcesVideoRenderSystemGLRenderState}
	${SourcesGLPlatform}
)

set(
	FilesRendererD3D11
	
	${SourcesVideoRenderSystemD3D11}
	${SourcesVideoRenderSystemD3D11HardwareBuffer}
	${SourcesVideoRenderSystemD3D11Shader}
	${SourcesVideoRenderSystemD3D11StandardShader}
	${SourcesVideoRenderSystemD3D11Texture}
	${SourcesVideoRenderSystemD3D11RenderState}
)

set(
	FilesAudio
	
	${IncludeAudioALL}
	${SourcesAudioCore}
	${SourcesAudioFileHandler}
	${SourcesAudioSoundSystem}
)

set(
	FilesScene
	
	${IncludeSceneALL}
	${SourcesSceneALL}
)

set(
	FilesEngine
	
	${IncludeEngine}
	${SourcesEngine}
	${IncludeEngineComponent}
	${SourcesEngineComponent}
	${SourcesEngineComponentProperty}
	${IncludeEngineGame}
	${SourcesEngineGame}
	${IncludeEngineAsset}
	${SourcesEngineAsset}
	${IncludeEngineFileHandler}
	${SourcesEngineFileHandler}
)

set(
	FilesUtility
	
	${IncludeUtilityALL}
	${SourcesUtilityALL}
)

set(
	FilesAnimation
	
	${IncludeAnimationALL}
	${SourcesAnimationALL}
)

set(
	FilesNetwork
	
	${IncludeNetworkALL}
	${SourcesNetworkALL}
	
	${IncludePlatformNetwork}
	${SourcesPlatformNetwork}
)

set(
	FilesGUI
	
	${IncludeGUIALL}
	${SourcesGUIALL}
)

set(
	FilesPhysics
	
	${IncludePhysicsALL}
	${SourcesPhysics}
	${SourcesPhysicsDynamic}
	${SourcesPhysicsKinematic}
)

set(
	FilesScript
	
	${IncludeScriptALL}
	${SourcesScript}
)

set(
	FilesEditor
	
	${SourcesEditor}
	${SourcesEditorCore}
	${SourcesEditorCoreComponent}
	${SourcesEditorCoreComponentUIProperty}
	${SourcesEditorCoreTimeline}
	${SourcesEditorCoreUIExtensions}
	${SourcesEditorEntity}
	${SourcesEditorFrame}
	${SourcesEditorFrameConfig}
	${SourcesEditorFrameConsole}
	${SourcesEditorFrameHierarchyViewer}
	${SourcesEditorFrameMain}
	${SourcesEditorProject}
	${SourcesEditorTools}
	${SourcesEditorToolsAssetBrowser}
	${SourcesEditorToolsAssetBrowserAsset}
	${SourcesPath}/Editor/Language/Interface.h
	${SourcesPath}/Editor/Language/LanguageStrings.h
	${SourcesPath}/Editor/Language/LanguageLoader.h
	${SourcesPath}/Editor/Language/LanguageLoader.cpp
)

set(
	FilesDaemonServer
	
	${SourcesDaemon}
	${SourcesDaemonService}
)


# === Libraries ===

add_library(ForkCore SHARED ${FilesCore})
add_library(ForkRenderer SHARED ${FilesRenderer})
add_library(ForkRendererGL SHARED ${FilesRendererGL})
add_library(ForkScene SHARED ${FilesScene})
add_library(ForkUtility SHARED ${FilesUtility})
add_library(ForkGUI SHARED ${FilesGUI})
add_library(ForkAnimation SHARED ${FilesAnimation})
add_library(ForkAudio SHARED ${FilesAudio})
add_library(ForkNetwork SHARED ${FilesNetwork})
add_library(ForkPhysics SHARED ${FilesPhysics})
add_library(ForkPhysicsNw SHARED ${SourcesPhysicsEngineNewtonALL})
add_library(ForkScript SHARED ${FilesScript})
add_library(ForkScriptPy SHARED ${SourcesScriptLanguagePythonALL})
add_library(ForkENGINE SHARED ${FilesEngine})

set_target_properties(ForkCore PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkRenderer PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkRendererGL PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkScene PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkUtility PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkGUI PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkAnimation PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkAudio PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkNetwork PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkPhysics PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkPhysicsNw PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkScript PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkScriptPy PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkENGINE PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(ForkCore PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkRenderer PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkRendererGL PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkScene PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkUtility PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkGUI PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkAnimation PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkAudio PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkNetwork PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkPhysics PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkPhysicsNw PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkScript PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkScriptPy PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkENGINE PROPERTIES DEBUG_POSTFIX "D")

if(WIN32)
	add_library(ForkRendererD3D11 SHARED ${FilesRendererD3D11})
	set_target_properties(ForkRendererD3D11 PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(ForkRendererD3D11 PROPERTIES DEBUG_POSTFIX "D")
	
	add_library(ForkAudioXA2 SHARED ${SourcesAudioSoundSystemXAudio2ALL})
	set_target_properties(ForkAudioXA2 PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(ForkAudioXA2 PROPERTIES DEBUG_POSTFIX "D")
endif(WIN32)


# === Langauge libraries ===

add_library(ForkEdLanguageEN SHARED ${SourcesPath}/Editor/Language/LanguageEN.c)
add_library(ForkEdLanguageDE SHARED ${SourcesPath}/Editor/Language/LanguageDE.c)
add_library(ForkEdLanguageFR SHARED ${SourcesPath}/Editor/Language/LanguageFR.c)
add_library(ForkEdLanguageRU SHARED ${SourcesPath}/Editor/Language/LanguageRU.c)

set_target_properties(ForkEdLanguageEN PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ForkEdLanguageDE PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ForkEdLanguageFR PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ForkEdLanguageRU PROPERTIES LINKER_LANGUAGE C)

set_target_properties(ForkEdLanguageEN PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkEdLanguageDE PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkEdLanguageFR PROPERTIES DEBUG_POSTFIX "D")
set_target_properties(ForkEdLanguageRU PROPERTIES DEBUG_POSTFIX "D")


# === Executables ===

if(WIN32)
	add_executable(
		ForkEd WIN32
		${FilesEditor}
		${EditorResourceFile}
	)
else(WIN32)
	add_executable(
		ForkEd
		${FilesEditor}
	)
endif(WIN32)

set_target_properties(ForkEd PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkEd PROPERTIES DEBUG_POSTFIX "D")

add_executable(ForkDaemonServer ${FilesDaemonServer})

set_target_properties(ForkDaemonServer PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ForkDaemonServer PROPERTIES DEBUG_POSTFIX "D")


# === Plugins ===

# JPEG lib
add_library(jpeglib STATIC ${FilesPluginsJPEG})
set_target_properties(jpeglib PROPERTIES LINKER_LANGUAGE C)


# === Internal dependencies ===

target_link_libraries(ForkRenderer ForkCore jpeglib)
target_link_libraries(ForkRendererGL ForkCore ForkRenderer)
target_link_libraries(ForkScene ForkCore ForkRenderer)
target_link_libraries(ForkAnimation ForkCore ForkScene)
target_link_libraries(ForkAudio ForkCore)
target_link_libraries(ForkPhysics ForkCore ForkScene)
target_link_libraries(ForkPhysicsNw ForkCore ForkScene ForkPhysics)
target_link_libraries(ForkScript ForkCore)
target_link_libraries(ForkScriptPy ForkScript ForkENGINE)
target_link_libraries(ForkNetwork ForkCore)
target_link_libraries(ForkUtility ForkCore ForkRenderer ForkScene ForkAudio)
target_link_libraries(ForkGUI ForkCore ForkRenderer ForkUtility)
target_link_libraries(ForkENGINE ForkCore ForkRenderer ForkScene ForkUtility ForkAnimation ForkNetwork ForkPhysics ForkScript ForkGUI ForkAudio)
target_link_libraries(ForkEd ForkENGINE)
target_link_libraries(ForkDaemonServer ForkCore ForkNetwork)

if(WIN32)
	target_link_libraries(ForkRendererD3D11 ForkCore ForkRenderer)
	target_link_libraries(ForkAudioXA2 ForkCore ForkAudio)
endif(WIN32)


# === External libraries ===

# OpenGL
find_package(OpenGL)
if(OPENGL_FOUND)
	message("Found OpenGL")
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(ForkRendererGL ${OPENGL_LIBRARIES})
else(OPENGL_FOUND)
	message("Missing OpenGL -> OpenGL render system is excluded")
endif(OPENGL_FOUND)

# Assimp
include(FindAssimp.cmake)
if(Assimp_FOUND)
	message("Found Assimp")
	include_directories(${Assimp_INCLUDE_DIR})
	target_link_libraries(ForkScene ${Assimp_LIBRARIES} ${zlib_LIBRARIES})
else(Assimp_FOUND)
	message("Missing Assimp -> Common model reader is excluded")
endif(Assimp_FOUND)

# Newton
include(FindNewton.cmake)
if (Newton_FOUND)
	message("Found Newton Game Dynamics")
	include_directories(${Newton_INCLUDE_DIR})
	target_link_libraries(ForkPhysicsNw ${Newton_LIBRARIES})
	add_define(_NEWTON_STATIC_LIB)
else(Newton_FOUND)
	message("Missing Newton -> Newton physics system is excluded")
endif(Newton_FOUND)

# Sockets
if(WIN32)
	target_link_libraries(ForkNetwork WS2_32 NetAPI32 Vfw32 Iphlpapi)
endif(WIN32)

# wxWidgets
find_package(wxWidgets REQUIRED adv aui base core html propgrid richtext stc xml jpeg tiff png zlib regex expat scintilla ribbon)
if(wxWidgets_FOUND)
	message("Found wxWidgets")
	include_directories(${wxWidgets_INCLUDE_DIRS})
	target_link_libraries(ForkEd ${wxWidgets_LIBRARIES})
else(wxWidgets_FOUND)
	message("Missing wxWidgets -> Editor is excluded")
endif(wxWidgets_FOUND)

# DirectX
if(WIN32)
	# Direct3D 11
	target_link_libraries(
		ForkRendererD3D11
		d3d11 d2d1 dwrite dxguid d3dcompiler dxgi
	)
	
	# XAudio2
#	if(${CMAKE_SYSTEM_VERSION} EQUAL 6.2) # Windows 8
		target_link_libraries(ForkAudioXA2 XAudio2)
		message("Found XAudio2")
#	else()
#		include(FindXAudio2.cmake)
#		if(DirectX_FOUND)
#			target_link_libraries(ForkAudioXA2 ${XAUDIO2_LIBRARIES})
#			message("Found XAudio2 (legacy)")
#		else(DirectX_FOUND)
#			message("Missing XAudio2 -> XA2 audio system is excluded")
#		endif(DirectX_FOUND)
#	endif()
	
#	option(
#		USE_XAUDIO2_INCLUDE_FROM_DIRECTX_SDK
#		"Use include directory for XAudio2 from the old DirectX SDK (required for Windows 7 and older)"
#		ON
#	)
#	if(USE_XAUDIO2_INCLUDE_FROM_DIRECTX_SDK)
#		find_path(DXSDK_INCLUDE_DIR XAudio2.h)
#		include_directories(${DXSDK_INCLUDE_DIR})
#	endif(USE_XAUDIO2_INCLUDE_FROM_DIRECTX_SDK)
endif(WIN32)

# Python
find_package(PythonLibs)
if(PYTHONLIBS_FOUND)
	message("Found Python")
	include_directories(${PYTHON_INCLUDE_DIRS})
	target_link_libraries(ForkScriptPy ${PYTHON_LIBRARIES})
else(PYTHONLIBS_FOUND)
	message("Missing Python -> Python scripting is excluded")
endif(PYTHONLIBS_FOUND)

# OggVorbis
include(FindOggVorbis.cmake)
if(OggVorbis_FOUND)
	message("Found OggVorbis")
	include_directories(${OGG_INCLUDE_DIR})
	include_directories(${VORBIS_INCLUDE_DIR})
	target_link_libraries(ForkAudio ${Ogg_LIBRARIES} ${Vorbis_LIBRARIES} ${VorbisFile_LIBRARIES})
else(OggVorbis_FOUND)
	message("Missing OggVorbis -> Audio system is excluded")
endif(OggVorbis_FOUND)

# HLSLTranslator
include(FindHLSLTranslator.cmake)
if(HTLib_FOUND)
	message("Found HLSLTranslator")
	include_directories(${HTLib_INCLUDE_DIR})
	target_link_libraries(ForkRenderer ${HTLib_LIBRARIES})
else(HTLib_FOUND)
	message("Missing HLSLTranslator -> Render system is incomplete")
endif(HTLib_FOUND)


# === Tests ===

include(tests/Console/CMakeLists.txt)
include(tests/ForkSCript/CMakeLists.txt)
include(tests/Video/CMakeLists.txt)
include(tests/Scene/CMakeLists.txt)
include(tests/SSCT/CMakeLists.txt)
include(tests/Script/CMakeLists.txt)
include(tests/Network/CMakeLists.txt)
include(tests/Physics/CMakeLists.txt)
include(tests/RenderTargets/CMakeLists.txt)
include(tests/HiZMap/CMakeLists.txt)
include(tests/D3D11/CMakeLists.txt)
include(tests/CubeMap/CMakeLists.txt)
include(tests/DaemonClient/CMakeLists.txt)
include(tests/GUI/CMakeLists.txt)
include(tests/Audio/CMakeLists.txt)
include(tests/RayTracing/CMakeLists.txt)


# === Tutorials ===

include(tutorials/01-GettingStarted/CMakeLists.txt)


# === Tools ===

include(tools/ShaderFileConverter/CMakeLists.txt)

