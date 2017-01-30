/*
 * Editor UI component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_COMPONENT_H__
#define __FORK_EDITOR_UI_COMPONENT_H__


#include "UIProperty/UIProperty.h"
#include "Core/Container/Observable.h"
#include "Engine/Component/Component.h"
#include "../UIExtensions/PropertyState.h"
#include "IO/FileSystem/File.h"

#include <wx/wx.h>
#include <vector>
#include <map>


namespace Fork
{

namespace Editor
{


class ComponentObserver;
class ComponentContainer;
class UIPropertyFactory;
class UIPropertyGridBuilder;

//! Base class for any customized components in the editor.
class UIComponent
{
    
    public:
        
        void WriteToPropertyGrid();
        void ReadFromProperty(const PropertyState& state);

        //! Shows or hides this component in the associated property grid.
        void Show(bool show = true);

        virtual Engine::Component* EngineComponent() = 0;

        //! Returns the property grid of the entity component view.
        static UIPropertyGrid* PropertyGrid();

    protected:
        
        UIComponent(const std::string& namePrefix);

        virtual void OnReadFromProperty() = 0;

        //! Returns the UI property factory for the specified name prefix.
        static Engine::Component::PropertyFactory* BindUIPropertyFactory(const std::string& namePrefix);

    private:
        
        std::string namePrefix_;

};

#define DECL_UICOMPONENT_INTERFACE(name)                                            \
    private:                                                                        \
        Engine::Component* EngineComponent() override                               \
        {                                                                           \
            return this;                                                            \
        }                                                                           \
        void OnReadFromProperty() override                                          \
        {                                                                           \
            NotifyThisComponentChanged();                                           \
        }                                                                           \
    public:                                                                         \
        name##UIComponent() :                                                       \
            Engine::name##Component                                                 \
            {                                                                       \
                UIComponent::BindUIPropertyFactory(name##Component::GetIDName())    \
            },                                                                      \
            UIComponent                                                             \
            {                                                                       \
                name##Component::GetIDName()                                        \
            }                                                                       \
        {                                                                           \
        }                                                                           \
        static void BuildPropertyGrid(UIPropertyGridBuilder& builder)


} // /namespace Editor

} // /namespace Fork


#endif



// ========================