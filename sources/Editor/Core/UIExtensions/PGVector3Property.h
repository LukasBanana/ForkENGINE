/*
 * Editor property-grid vector3 property header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PG_VECTOR3_PROPERTY_H__
#define __FORK_EDITOR_PG_VECTOR3_PROPERTY_H__


#include "WxVector3.h"


namespace Fork
{

namespace Editor
{


//! Property grid vector3 property.
class PGVector3Property : public wxPGProperty
{
    
        WX_PG_DECLARE_PROPERTY_CLASS(PGVector3Property)

    public:
        
        PGVector3Property(
            const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, const WxVector3& value = WxVector3()
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