/*
 * Notification component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NOTIFICATION_COMPONENT_H__
#define __FORK_NOTIFICATION_COMPONENT_H__


#include "Engine/Component/Component.h"


namespace Fork
{

namespace Engine
{


class GameObject;

/*
Notification component.
\remarks This is an exceptional component. It does not allow any further inheritance.
*/
class FORK_EXPORT NotificationComponent final : public Component
{
    
    public:
        
        NotificationComponent(GameObject* owner);

        Types Type() const override;
        std::string IDName() const override;

        static std::string GetIDName();

    private:
        
        bool IsObservableOfInterest(const Types type) const override;
        void OnObservableChanged(const Component* observable) override;

        GameObject* owner_ = nullptr;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================