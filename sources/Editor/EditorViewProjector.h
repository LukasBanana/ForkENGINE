/*
 * Editor view projector header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_VIEW_PROJECTOR_H__
#define __FORK_EDITOR_VIEW_PROJECTOR_H__


#include "Utility/Projector/Projector.h"
#include "Scene/Manager/FreeViewSceneNodeController.h"


namespace Fork
{

namespace Editor
{


class EditorView;

class EditorViewProjector : public Utility::Projector
{
    
    public:
        
        EditorViewProjector(EditorView& owner, Scene::FreeViewSceneNodeController& cameraCtrl);

    private:
        
        class FORK_EXPORT ProjectorMouseEventHandler : public MouseEventHandler
        {
            
            public:
                
                ProjectorMouseEventHandler(Projector* projector);

                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override;

            private:
                
                //! Returns true if the event, emitted from the specified frame, is dedicated for this editor view projector.
                bool IsDedicatedEvent(const Platform::Frame* frame) const;

        };

        EditorView* owner_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================