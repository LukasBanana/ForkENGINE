/*
 * Doxygen docu extensions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DOCU_H__
#define __FORK_DOCU_H__


/**
\defgroup math_core Core Math Functionality (Vectors, Matrices, Quaternions etc.).
\defgroup lang_forkscript ForkSCript Language.
\defgroup fork_exception All Engine Specific Exception Classes.
\defgroup vertex_struct Default Vertex Structures.
\defgroup std_geometries Standard Geometry Classes.
\defgroup std_materials Standard Material Classes.
\defgroup texture_classes Texture Classes.
\defgroup network Network System.
\defgroup xml_parser XML Parser.
\defgroup animation Animation System.
\defgroup postprocessor Post Processors.
\defgroup gbuffer G-Buffer Classes.
*/

/**
\page page_global_macros Global Macros

Platform dependent macros:
\code
// Is defined on Microsoft Windows platforms.
#define FORK_WINDOWS

// Is defined on Posix platforms (e.g. GNU/Linux) (Not supported yet).
#define FORK_POSIX

// Is defined for Apple OSX and MacOSX platforms (Not supported yet).
#define FORK_OSX

// Is defined for Apple iOS platforms (Not supported yet).
#define FORK_IOS

// Is defined for Microsoft XBox platforms (Not supported yet).
#define FORK_XBOX
\endcode

Compiler dependent macros:
\code
// Is defined for Microsoft VisualC++ compilers.
#define FORK_MSVC

// Is defined for GNU/C++ compilers.
#define FORK_GCC
\endcode
*/

/**
\mainpage ForkENGINE 0.1 Alpha API Documentation

\section section_intro Introduction
Welcome to the API documentation of the ForkENGINE, a high level game engine.

\section section_editor World Editor
Here is a screenshot from the world editor:
\image html images/MainPage.Cover.png
*/

#endif



// ========================