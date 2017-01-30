/*
 * Editor property state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PROPERTY_STATE_H__
#define __FORK_EDITOR_PROPERTY_STATE_H__


#include "IO/Core/Variant.h"
#include "Engine/Component/Component.h"
#include "WxVector3.h"
#include "WxVector4.h"

#include <wx/colour.h>
#include <wx/propgrid/propgrid.h>


namespace Fork
{

namespace Editor
{


/**
Property state structure.
Stores all information about a PGProperty state change.
*/
class PropertyState : public IO::Variant
{
    
    public:
        
        typedef Engine::Component::PropertyKey PropertyKey;

        PropertyState() = default;
        PropertyState(const wxPropertyGridEvent& event);
        PropertyState(wxPGProperty& prop);

        //! \see UIPropertyGrid::ExtractPropertyID
        int ExtractPropertyID() const;
        //! \see UIPropertyGrid::ExtractPropertyKey
        PropertyKey ExtractKey() const;
        //! \see UIPropertyGrid::ExtractPropertyIdAndPrefix
        bool ExtractPropertyIDAndPrefix(int& id, wxString& prefix) const;

        //! Returns true if this property state's name matches the specified comparision name.
        inline bool operator () (const wxString& compareName) const
        {
            return name == compareName;
        }

        wxString    name;       //!< Full property name (e.g. "comp_mesh_geometry@1" or "AttrTexture@2").
        PropertyKey key = 0;    //!< Property key.

    private:
        
        void Init(const wxVariant& variant);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================