/*
 * Path dictionary header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PATH_DICTIONARY_H__
#define __FORK_PATH_DICTIONARY_H__


#include "Core/Export.h"

#include <vector>
#include <string>


namespace Fork
{

namespace IO
{


/**
Search path dictionary. This can be used to search for files in several paths (or rather directories).
All paths are stored as UTF-8 strings for portability, thus Unicode is NOT supported.
\code
PathDictionary pathDict;

// Add some search paths
pathDict.AddSearchPath("MyProject/CommonShaderFiles/");
pathDict.AddSearchPath("MyProject/SpecificShaderFiles/");

// Search shader filename with dictionary
std::string shaderFilename = "ExampleShader.hlsl";
if (pathDict.FindFile(shaderFilename))
    LoadShader(shaderFilename);
else
    Error();
\endcode
*/
class FORK_EXPORT PathDictionary
{
    
    public:
        
        //! Adds the specified path. If the path may end with with '/' or '\\\\' or without it.
        void AddSearchPath(const std::string& path);
        //! Removes the specified path. This is case sensitive!
        void RemoveSearchPath(const std::string& path);

        /**
        Returns the first found filename.
        \param[in,out] filename Specifies the input and output filename.
        \return True if the specified file has been found in any of the stored search paths.
        */
        bool FindFile(std::string& filename) const;

        //! Returns the search path list.
        inline const std::vector<std::string>& GetSearchPaths() const
        {
            return searchPaths_;
        }

    private:

        std::vector<std::string> searchPaths_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================