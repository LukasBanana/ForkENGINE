/*
 * Editor free-look mouse event handler file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FreeLookMouseEventHandler.h"
#include "Platform/Core/Cursor.h"
#include "Platform/Core/Desktop.h"


namespace Fork
{

namespace Editor
{


/*
 * ======= Protected: =======
 */

void FreeLookMouseEventHandler::LockFreeLook()
{
    if (!isFreeLook_)
    {
        LockFocus();
        isFreeLook_ = true;

        prevMousePos_ = Platform::Cursor::Position();
        Platform::Cursor::Show(false);
    }
}

void FreeLookMouseEventHandler::UnlockFreeLook()
{
    if (isFreeLook_)
    {
        Platform::Cursor::SetupPosition(prevMousePos_);
        Platform::Cursor::Show(true);

        isFreeLook_ = false;
        UnlockFocus();
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================