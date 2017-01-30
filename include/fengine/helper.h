/*
 * Helper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_HELPER_H__
#define __FORK_HELPER_H__


#include <fengine/core.h>

/* --- Define all helper macros --- */

#define KEY_HIT(k)      ( IO::Keyboard::Instance()->KeyHit     (IO::KeyCodes::Key##k) )
#define KEY_DOWN(k)     ( IO::Keyboard::Instance()->KeyDown    (IO::KeyCodes::Key##k) )
#define KEY_RELEASED(k) ( IO::Keyboard::Instance()->KeyReleased(IO::KeyCodes::Key##k) )

#define MOUSE_HIT(k)        ( IO::Mouse::Instance()->ButtonHit     (IO::MouseKeyCodes::Mouse##k) )
#define MOUSE_DOWN(k)       ( IO::Mouse::Instance()->ButtonDown    (IO::MouseKeyCodes::Mouse##k) )
#define MOUSE_RELEASED(k)   ( IO::Mouse::Instance()->ButtonReleased(IO::MouseKeyCodes::Mouse##k) )

#define LOAD_TEXTURE(f) Video::RenderSystem::Active()->GetTextureManager()->LoadTexture2D(f)


#endif



// ========================