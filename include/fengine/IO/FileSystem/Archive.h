/*
 * Archive header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_ARCHIVE_H__
#define __FORK_IO_ARCHIVE_H__


#include "Core/DeclPtr.h"
#include "IO/FileSystem/VirtualFile.h"
#include "IO/Crypto/CryptoKey.h"

#include <memory>
#include <string>
#include <map>


namespace Fork
{

namespace IO
{


/**
Virtual archive class. This can be used to structure several virutal files.
This class also provides a simple cryptographic function.
\todo Incomplete!
*/
class FORK_EXPORT Archive
{
    
    public:
        
        //! Archive file type.
        typedef VirtualFile File;
        typedef std::shared_ptr<File> FilePtr;

        DECL_SHR_PTR(Folder);

        //! Archive folder class.
        class Folder
        {
            
            public:
                
                Folder* CreateFolder(const std::string& path);
                void DeleteFolder(Folder* folder);

                Folder* FindFolder(const std::string& path) const;

                File* CreateFile(const std::string& filename);
                void DeleteFile(File* file);

                File* FindFile(const std::string& filename) const;

                inline const std::map<std::string, FolderPtr>& GetFolders() const
                {
                    return folders_;
                }
                inline const std::map<std::string, FilePtr>& GetFiles() const
                {
                    return files_;
                }

            private:
                
                std::map<std::string, FolderPtr> folders_;
                std::map<std::string, FilePtr> files_;

        };

        Folder* CreateFolder(const std::string& path);
        void DeleteFolder(const std::string& path);

        Folder* FindFolder(const std::string& path) const;

        File* CreateFile(const std::string& filename);
        void DeleteFile(const std::string& filename);

        File* FindFile(const std::string& filename) const;

        /**
        Writes the entire archive to a physical file.
        \param[in] filename Specifies the filename of the physical output file.
        \param[in] cryptoKey Optional raw pointer to a cryptographi key to encrypt the output archive file. By default null.
        \return True on success, otherwise false (in this case error messages are printed to the log output).
        \see CryptoKey::Encode
        \see Log
        */
        bool WriteArchiveToFile(const std::string& filename, const CryptoKey* cryptoKey = nullptr);
        /**
        Reads the entire archive from a physical file.
        \param[in] filename Specifies the filename of the physical input file.
        \param[in] cryptoKey Optional raw pointer to a cryptographi key to decrypt the input archive file. By default null.
        \return True on success, otherwise false (in this case error messages are printed to the log output).
        \see CryptoKey::Decode
        \see Log
        */
        bool ReadArchiveFromFile(const std::string& filename, const CryptoKey* cryptoKey = nullptr);

        //! Returns the archive root folder (with path "/").
        inline const Folder& GetRootFolder() const
        {
            return rootFolder_;
        }

    private:

        Folder rootFolder_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================