/*
 * Script interpreter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCRIPT_INTERPRETER_H__
#define __FORK_SCRIPT_INTERPRETER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Platform/Core/DynamicLibrary.h"
#include "Script/ScriptInterpreterException.h"


namespace Fork
{

namespace Script
{


DECL_SHR_PTR(ScriptInterpreter);

//! Script interpreter interface.
class FORK_EXPORT ScriptInterpreter
{
    
    public:
        
        /* === Structures === */

        //! Scripting environment description structure.
        struct EnvironmentDescription
        {
            /**
            Search path for the scripting engine.
            \remarks When using python ('ForkScriptPy') for instance,
            this must be the directory name where Python is installed (e.g. "C:/Python" or "C:/ForkENGINE/plugins/python").
            */
            std::wstring scriptEnginePath;
            /**
            Search path for the scripting modules.
            \remarks When using python ('ForkScriptPy') for instance,
            this must be the directory name where the Python modules are stored (e.g. "C:/ForkENGINE/scripts/python").
            */
            std::string scriptModulePath;
        };

        //! Scripting language feature flags.
        struct FeatureFlags
        {
            typedef unsigned char DataType;
            enum : DataType
            {
                RunFromString   = 0x01, //!< The language allows to run code directly from a string.
                RunFromFile     = 0x02, //!< The language allows to run code directly from a file.
                RunFromByteCode = 0x04, //!< The language allows to run code from a pre-compiled byte-code.
            };
        };

        /**
        Script interpreter descriptor structure.
        \remarks This is not a structure to 'describe' something which is to be created.
        Instead it is a descriptor of an existing scrip interpreter.
        */
        struct Descriptor
        {
            std::string             language;           //!< Scripting language name (e.g. "Python", "C#").
            std::string             fileExt;            //!< Language file extensions (e.g. "py", "cs").
            std::string             version;            //!< Interpreter version (e.g. "3.4").
            std::string             copyright;          //!< Language and interpreter copyright notice.
            FeatureFlags::DataType  featureFlags = 0;   //!< Language feature flags. \see FeatureFlags
        };

        /* === Static functions === */

        /**
        Creates a script interpreter from the specified library.
        \param[in] library Specifies the library from which the script inrerpreter is to be created (e.g. "ForkScriptPy").
        \param[in] environmentDesc Specifies the scrinting environment. This structure contains meta information for the scripting engine.
        \throws ScriptInterpreterException If an error occured during creation.
        \see Platform::DynamicLibrary
        */
        static ScriptInterpreterPtr Create(
            const Platform::DynamicLibraryPtr& library, const EnvironmentDescription& environmentDesc
        );

        /* === Interface === */

        virtual ~ScriptInterpreter();

        /**
        Queries the scripting language descriptor.
        \see Descriptor
        */
        virtual Descriptor QueryDescriptor() const = 0;

        /**
        Runs code directly from the specified string.
        \param[in] code Specifies the code to execute. This may also contain new-line characters '\\n'.
        \return True if the code was successfully executed.
        \note Not every scripting language supports code execution directly from a string.
        See 'Descriptor::featureFlags' for more information.
        \see Descriptor::featureFlags
        \see Descriptor::FeatureFlags::RunFromString
        */
        virtual bool RunFromString(const std::string& code);
        /**
        Runs code directly from the specified file.
        \param[in] filename Specifies the file to execute.
        \return True if the code was successfully executed.
        \note Not every scripting language supports code execution directly from a file.
        See 'Descriptor::featureFlags' for more information.
        \see Descriptor::featureFlags
        \see Descriptor::FeatureFlags::RunFromFile
        */
        virtual bool RunFromFile(const std::string& filename);

    protected:
        
        ScriptInterpreter();

};


} // /namespace Script

} // /namespace Fork


#endif



// ========================