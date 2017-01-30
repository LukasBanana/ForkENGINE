/*
 * Editor property-grid vector4 property header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PG_VECTOR4_PROPERTY_H__
#define __FORK_EDITOR_PG_VECTOR4_PROPERTY_H__


#include "WxVector4.h"


namespace Fork
{

namespace Editor
{


//! Property grid vector4 property.
class PGVector4Property : public wxPGProperty
{
    
        WX_PG_DECLARE_PROPERTY_CLASS(PGVector4Property)

    public:
        
        PGVector4Property(
            const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const WxVector4& value = WxVector4()
        );

        wxVariant ChildChanged(
            wxVariant& thisValue, int childIndex, wxVariant& childValue
        ) const;

        void RefreshChildren();

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================