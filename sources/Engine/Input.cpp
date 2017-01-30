/*
 * Engine input file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Input.h"
#include "IO/Core/Log.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Engine
{


/*
 * Trigger structure
 */

Input::Trigger::Trigger(const IO::KeyCodes keyCode, const States keyState) :
    device  { Devices::Keyboard             },
    state   { keyState                      },
    key     { static_cast<KeyType>(keyCode) }
{
}
Input::Trigger::Trigger(const IO::MouseKeyCodes keyCode, const States keyState) :
    device  { Devices::Mouse                },
    state   { keyState                      },
    key     { static_cast<KeyType>(keyCode) }
{
}


/*
 * Input class
 */

void Input::Assign(const CiString& name, const Trigger& trigger)
{
    nameAssignments_[name].triggers.push_back(trigger);
}

void Input::Assign(const IdType& id, const Trigger& trigger)
{
    idAssignments_[id].triggers.push_back(trigger);
}

void Input::Clear(const CiString& name)
{
    auto it = nameAssignments_.find(name);
    if (it != nameAssignments_.end())
        nameAssignments_.erase(it);
}

void Input::Clear(const IdType& id)
{
    auto it = idAssignments_.find(id);
    if (it != idAssignments_.end())
        idAssignments_.erase(it);
}

void Input::ClearAll()
{
    nameAssignments_.clear();
    idAssignments_.clear();
}

bool Input::IsTriggered(const CiString& name) const
{
    auto assignment = FindAssignment(name);
    return assignment ? IsTriggered(*assignment) : false;
}

bool Input::IsTriggered(const IdType& id) const
{
    auto assignment = FindAssignment(id);
    return assignment ? IsTriggered(*assignment) : false;
}

Input::Assignment* Input::FindAssignment(const CiString& name)
{
    auto it = nameAssignments_.find(name);
    return it != nameAssignments_.end() ? &(it->second) : nullptr;
}

Input::Assignment* Input::FindAssignment(const IdType& id)
{
    auto it = idAssignments_.find(id);
    return it != idAssignments_.end() ? &(it->second) : nullptr;
}

const Input::Assignment* Input::FindAssignment(const CiString& name) const
{
    auto it = nameAssignments_.find(name);
    return it != nameAssignments_.end() ? &(it->second) : nullptr;
}

const Input::Assignment* Input::FindAssignment(const IdType& id) const
{
    auto it = idAssignments_.find(id);
    return it != idAssignments_.end() ? &(it->second) : nullptr;
}


/*
 * ======= Private: =======
 */

static bool WrongKeyState()
{
    #ifdef FORK_DEBUG
    IO::Log::Debug("Wrong key state for Engine::Input::Trigger");
    #endif
    return false;
}

bool Input::IsTriggered(const Assignment& assignment) const
{
    for (const auto& trigger : assignment.triggers)
    {
        if (IsTriggered(trigger))
            return true;
    }
    return false;
}

bool Input::IsTriggered(const Trigger& trigger) const
{
    switch (trigger.device)
    {
        case Devices::Keyboard:
        {
            const auto keyState = KeyState(static_cast<IO::KeyCodes>(trigger.key));

            switch (trigger.state)
            {
                case States::Down:      return keyState.down;
                case States::Hit:       return keyState.hit;
                case States::HitRepeat: return keyState.hitRepeat;
                case States::Released:  return keyState.released;
                default:                return WrongKeyState();
            }
        }
        break;

        case Devices::Mouse:
        {
            const auto buttonState = ButtonState(static_cast<IO::MouseKeyCodes>(trigger.key));
                
            switch (trigger.state)
            {
                case States::Down:          return buttonState.down;
                case States::Hit:           return buttonState.hit;
                case States::Released:      return buttonState.released;
                case States::DoubleClicked: return buttonState.doubleClicked;
                default:                    return WrongKeyState();
            }
        }
        break;

        case Devices::MouseWheel:
        {
            switch (trigger.state)
            {
                case States::Forward:   return MouseWheel() > 0;
                case States::Backward:  return MouseWheel() < 0;
                default:                return WrongKeyState();
            }
        }
        break;
    }

    return false;
}


} // /namespace Engine

} // /namespace Fork



// ========================