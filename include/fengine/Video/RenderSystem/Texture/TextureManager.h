/*
 * Texture manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_MANAGER_H__
#define __FORK_TEXTURE_MANAGER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Core/Container/SharedHashMap.h"
#include "Math/Core/Cuboid.h"
//#include "Core/Container/EventEmitter.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(TextureManager);

/**
Texture manager class.
This class should be used to create textures from image files.
*/
class FORK_EXPORT TextureManager// : public EventEmitter<TextureManager::EventHandler>
{
    
    public:
        
        //! Texture manager event handler interface.
        class FORK_EXPORT EventHandler
        {
            
            public:
                
                virtual ~EventHandler()
                {
                }

                /**
                Callback function to optionally modify a texture filename before the texture is loaded.
                Event will be received when "CreateTexture2DFromFile" is called.
                \param[in] filename Specifies the texture filename which can be modified by returning a different filename.
                \return Final texture filename to load. By default the input filename will be returned unmodified.
                */
                virtual std::string OnTextureFilename(const std::string& filename)
                {
                    return filename;
                }

                /**
                Callback function when "CreateTexture2DFromFile" is called. This can be used to discard a texture load.
                \param[in] filename Specifies the image which is to be loaded to create the texture data.
                \param[in] hasHDR Specifies whether the image has HDR (high-dynamic-range) information or not. By default false.
                \return True if the specified texture file is to be loaded. Otherwise the texture load will be discarded.
                \remarks This event is posted after "OnTextureFilename" was posted.
                \remarks There are possibly more than one event handler, but only a single event handler suffice to discard a texture load.
                In this case, all the other event handler will receive this event as well but they can not cancel this discard message.
                \see OnTextureFilename
                */
                virtual bool OnCreateTexture2DFromFile(const std::string& filename, bool hasHDR)
                {
                    return true;
                }

                //! \todo Document me.
                virtual bool OnCreateTexture2DArrayFromFiles(const std::vector<std::string>& filenames, bool hasHDR)
                {
                    return true;
                }

                //! \todo Document me.
                virtual bool OnLoadTexture2D(const std::string& filename, bool hasHDR)
                {
                    return true;
                }

                //! Receives the 'texture 2D has been created' event. This can be used to register, that a new texture was created.
                virtual void OnTexture2DCreated(const Texture2DPtr& texture)
                {
                }

        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /**
        Creates a 2D texture and initializes it with the image, loaded from the specified file.
        This is a 'helper function', if you need access to that image object, create the texture
        with "CreateTexture2D", load the image with "Video::ImageFileHandler::ReadImage" and
        initialize the texture with "RenderSystem::WriteTexture".
        \param[in] filename Specifies the image which is to be loaded to create the texture data.
        \param[in] hasHDR Specifies whether the image has HDR (high-dynamic-range) information or not. By default false.
        \return Shared pointer to the new texture or null if the texture loading failed.
        \throws NullPointerException If no render system or no render context is active.
        \see CreateTexture2D
        \see Video::ImageFileHandler::ReadImage
        \see WriteTexture
        */
        Texture2DPtr CreateTexture2DFromFile(const std::string& filename, bool hasHDR = false);

        /**
        Creates a 2D texture array and initializes it with the images, loaded from the specified files.
        \param[in] filenames Specifies the filename list.
        \see CreateTexture2DFromFile
        */
        Texture2DPtr CreateTexture2DArrayFromFiles(const std::vector<std::string>& filenames, bool hasHDR = false);

        /**
        Creates a cube texture and initializes it with the images, loaded from the specified files.
        \param[in] filenames Specifies the filename cuboid for all 6 cube map faces.
        \see CreateTexture2DFromFile
        */
        TextureCubePtr CreateTextureCubeFromFiles(const Math::Cuboid<std::string>& filenames, bool hasHDR = false);

        /**
        Loads the specified 2D image texture. If the texture was already loaded, this texture will be returned.
        \see CreateTexture2DFromFile
        */
        Texture2DPtr LoadTexture2D(const std::string& filename, bool hasHDR = false);

        //! Releases the specified texture.
        void ReleaseTexture(const Texture* texture);
        //! Releases all textures in this texture manager.
        void ReleaseAllTextures();

        /**
        Adds an event handler. This can be used to discard and/or register texture loading events.
        \see EventHandler
        */
        void AddEventHandler(const EventHandlerPtr& eventHandler);
        //! Removes the specified event handler.
        void RemoveEventHandler(const EventHandlerPtr& eventHandler);

    private:
        
        template <class T> T AddTexture(const T& texture)
        {
            textures_.push_back(texture);
            return texture;
        }

        std::vector<TexturePtr>                 textures_;          //!< Main texture container.
        std::vector<EventHandlerPtr>            eventHandlers_;     //!< Event handler container.

        SharedHashMap<std::string, Texture2D>   loaded2DTextures_;  //!< Loaded texture shared map.

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================