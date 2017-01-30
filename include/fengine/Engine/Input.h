/*
 * Engine input header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_INPUT_H__
#define __FORK_ENGINE_INPUT_H__


#include "Core/Export.h"
#include "Core/CiString.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"

#include <map>


namespace Fork
{

namespace Engine
{


/**
Engine device for common user input (keyboard, mouse, joystick etc.).
\remarks This is a high-level abstraction to combine all input devices.
For a more detailed control over these input devices, use the respective class from the "IO" namespace.
\code
// Assign key bindings
Input input;

input.Assign("up", { IO::KeyCodes::KeyUp, Engine::Input::States::Down });
input.Assign("up", { IO::KeyCodes::KeyW, Engine::Input::States::Down });

input.Assign("down", { IO::KeyCodes::KeyDown, Engine::Input::States::Down });
input.Assign("down", { IO::KeyCodes::KeyS, Engine::Input::States::Down });

input.Assign("shoot", { IO::KeyCodes::KeyReturn, Engine::Input::States::Hit });
input.Assign("shoot", { IO::MouseKeyCodes::MouseLeft, Engine::Input::States::Hit });

//...

if (input.IsTriggered("shoot"))
    Shoot();
if (input.IsTriggered("up"))
    MoveObjectUp();
if (input.IsTriggered("down"))
    MoveObjectDown();
\endcode
\see IO::Keyboard
\see IO::Mouse
*/
class FORK_EXPORT Input
{
    
    public:
        
        typedef unsigned int IdType;

        /* --- Enumerations --- */

        //! Input devices enumeration.
        enum class Devices
        {
            Keyboard,   //!< Keyboard. \see IO::Keyboard
            Mouse,      //!< Mouse. \see IO::Mouse
            MouseWheel, //!< Mouse wheel. \see IO::Mouse
            //Joystick,
            //GamePad,
        };

        //! Assignment trigger states.
        enum class States
        {
            Down,           //!< Key/ button pressed down.
            Hit,            //!< Key/ button hit.
            HitRepeat,      //!< Key hit (repeating).
            Released,       //!< Key/ button released.
            DoubleClicked,  //!< Button double clicked.
            Forward,        //!< Mouse wheel forward.
            Backward,       //!< Mouse wheel backward.
        };

        /* --- Structures --- */

        //! Input assignment trigger structure.
        struct FORK_EXPORT Trigger
        {
            typedef unsigned int KeyType;

            Trigger() = default;
            Trigger(const IO::KeyCodes keyCode, const States keyState);
            Trigger(const IO::MouseKeyCodes keyCode, const States keyState);

            Devices device  = Devices::Keyboard;
            States  state   = States::Down;
            KeyType key     = 0;
        };

        //! Input assignment structure.
        struct FORK_EXPORT Assignment
        {
            std::vector<Trigger> triggers;
        };

        /* --- Common input functions --- */

        //! \see IO::Keyboard::ButtonState
        inline IO::Keyboard::KeyStateModel KeyState(const IO::KeyCodes keyCode) const
        {
            return IO::Keyboard::Instance()->KeyState(keyCode);
        }

        //! \see IO::Mouse::ButtonState
        inline IO::Mouse::ButtonStateModel ButtonState(const IO::MouseKeyCodes keyCode) const
        {
            return IO::Mouse::Instance()->ButtonState(keyCode);
        }

        //! \see IO::Mouse::GetWheelMotion
        inline int MouseWheel() const
        {
            return IO::Mouse::Instance()->GetWheelMotion();
        }

        /* --- User settings --- */

        void Assign(const CiString& name, const Trigger& trigger);
        void Assign(const IdType& id, const Trigger& trigger);

        void Clear(const CiString& name);
        void Clear(const IdType& id);
        void ClearAll();

        bool IsTriggered(const CiString& name) const;
        bool IsTriggered(const IdType& id) const;

        Assignment* FindAssignment(const CiString& name);
        Assignment* FindAssignment(const IdType& id);

        const Assignment* FindAssignment(const CiString& name) const;
        const Assignment* FindAssignment(const IdType& id) const;

    private:
        
        bool IsTriggered(const Assignment& assignment) const;
        bool IsTriggered(const Trigger& trigger) const;

        std::map<CiString, Assignment>  nameAssignments_;   //!< Input assignments per name.
        std::map<IdType, Assignment>    idAssignments_;     //!< Input assignments per ID.

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================