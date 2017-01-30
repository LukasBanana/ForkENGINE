/*
 * Editor UI property grid builder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_GRID_BUILDER_H__
#define __FORK_EDITOR_UI_PROPERTY_GRID_BUILDER_H__


#include "../../Language/LanguageStrings.h"
#include "Engine/Component/Component.h"

#include <string>
#include <wx/propgrid/propgrid.h>


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

//! Helper class to build the component view's property grid.
class UIPropertyGridBuilder
{
    
    private:

        typedef LangStrings::DataType StringID;

    public:
        
        UIPropertyGridBuilder(UIPropertyGrid& propGrid);

        /**
        Initializes the build setup for a new category.
        This will update the name prefix for the property name IDs and reset the property key counter.
        */
        void InitCategory(const StringID labelStringID, const std::string& namePrefix);

        /* --- Properties --- */
        
        void Bool       (const StringID labelStringID);
        void Float      (const StringID labelStringID);
        
        void Int        (const StringID labelStringID, bool hasSpin = false);
        void Int        (const StringID labelStringID, long minRange, long maxRange, bool hasSpin = false);
        void UInt       (const StringID labelStringID);
        
        void ColorRGB   (const StringID labelStringID);
        void ColorRGBA  (const StringID labelStringID);
        void SystemColor(const StringID labelStringID);
        
        void Enum       (const StringID labelStringID, wxPGChoices& choices);
        void Enum       (const StringID labelStringID);
        
        void String     (const StringID labelStringID);
        void LongString (const StringID labelStringID);
        void ArrayString(const StringID labelStringID);
        
        void File       (const StringID labelStringID);
        
        void Dir        (const StringID labelStringID);

        void Vector3    (const StringID labelStringID);
        void Vector4    (const StringID labelStringID);

        void Asset      (const StringID labelStringID);

    private:
        
        wxString NextPropID();

        UIPropertyGrid* const           propGrid_;
        std::string                     namePrefix_;
        Engine::Component::PropertyKey  keyCounter_ = 0;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================