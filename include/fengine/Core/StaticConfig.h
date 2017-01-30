/*
 * Static configuration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STATIC_CONFIG_H__
#define __FORK_STATIC_CONFIG_H__


/* --- Static compilation configuration macros --- */

/*
This is the static configuration file.
All compilation options should be inserted here.
*/

/* Enable extended debugging code. */
#ifdef _DEBUG
#   define FORK_DEBUG
#endif

/*
Enables commercial use. Otherwise the "non commercial use"
information is printed out at engine start-up.
*/
#define FORK_COMMERCIAL_USE

/*
Imports the Assimp library (static library) to the engine.
Otherwise all common 3D file formats can not be loaded!
*/
#define FORK_IMPORT_ASSIMP

/* Enable ALL exceptions. Some exceptions can not be disabled! */
#define FORK_ENABLE_ALL_EXCEPTIONS

/* Enable special case exception: "Not Yet Implemented". */
#define FORK_ENABLE_EXCEPTION_NOTYETIMPLEMENTED


/* --- Further macros --- */

#ifdef FORK_DEBUG
#   ifdef _MSC_VER
#       define FORK_DEBUG_BREAK __debugbreak();
#   else
#       define FORK_DEBUG_BREAK
#   endif
#endif


/* --- Disable some warnings --- */

#ifdef _MSC_VER
#   pragma warning(disable : 4250) // Warning occurs when a class inherits from several base-classes and a virtual inheritance is used.
#   pragma warning(disable : 4251) // Warning occurs when a class has a member of another class which is not declared for dll-export.
#   pragma warning(disable : 4290) // Warning occurs when a function is declared to throw a specified exception.
#   pragma warning(disable : 4275) // Warning occurs when a clas inherits from a non dll-interface class (e.g. std::exception)
//#   pragma warning(disable : 4267) // x64: Warning occur when converting size_t to unsigned int.
#endif


#endif



// ========================