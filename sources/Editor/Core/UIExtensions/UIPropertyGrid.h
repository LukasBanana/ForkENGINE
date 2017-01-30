/*
 * Editor UI property grid header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_GRID_H__
#define __FORK_EDITOR_UI_PROPERTY_GRID_H__


#include "../WxHelper.h"
#include "PGVector3Property.h"
#include "PGVector4Property.h"
#include "../ColorScheme.h"
#include "PropertyState.h"
#include "Core/Container/Observable.h"
#include "../Component/UIComponent.h"
#include "Core/Exception/InvalidArgumentException.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include <initializer_list>
#include <functional>


namespace Fork
{

namespace Editor
{


class UIPropertyGridObserver;

/**
Generates a simple property ID name for the specified entry 'e'.
\note 'e' must be an entry of an enumeration named "PropertyIDs".
\see UIPropertyGrid::GetId
*/
#define PROPERTY_ID(e) UIPropertyGrid::GetID(PropertyIDs::e)

//! Property grid class. This class is observable by a "UIPropertyGridObserver".
class UIPropertyGrid :
    public wxPropertyGrid,
    public Observable<UIPropertyGridObserver, const Engine::Component::Types, const PropertyState&>
{
    
    public:
        
        //! Property prefix separator character (this is '@').
        static const char propPrefixSep;

        /* === Enumerations === */

        //! Property types.
        enum class PropertyTypes
        {
            Category,       //!< Not supported in "PropertyDescription" structure. Use "AddPropertyCategory".
            Bool,
            Float,
            Int,
            RangedInt,
            UInt,           //!< Not supported in "PropertyDescription" structure. Use "AddUIntProperty".
            ColorRGB,
            ColorRGBA,      //!< Not supported in "PropertyDescription" structure. Use "AddColorRGBAProperty".
            SystemColor,    //!< Not supported in "PropertyDescription" structure. Use "AddSystemColorProperty".
            Enum,           //!< Not supported in "PropertyDescription" structure. Use "AddEnumProperty".
            String,
            LongString,     //!< Not supported in "PropertyDescription" structure. Use "AddLongStringProperty".
            ArrayString,
            File,           //!< Not supported in "PropertyDescription" structure. Use "AddFileProperty".
            Dir,            //!< Not supported in "PropertyDescription" structure. Use "AddDirProperty".
            Vector3,
            Vector4,
        };

        /* === Structures === */

        //! Property description structure.
        struct PropertyDescription
        {
            PropertyDescription() = default;

            PropertyDescription(const PropertyTypes type, const wxString& label, const wxString& name = wxPG_LABEL);

            PropertyDescription(const wxString& label, const wxString& name, bool value);
            PropertyDescription(const wxString& label, const wxString& name, float value);
            PropertyDescription(const wxString& label, const wxString& name, int value, bool hasSpin = false);
            PropertyDescription(const wxString& label, const wxString& name, int value, long minRange, long maxRange, bool hasSpin = false);
            PropertyDescription(const wxString& label, const wxString& name, const wxColour& value = *wxWHITE);
            PropertyDescription(const wxString& label, const wxString& name, const wxString& value, const wxString& hint = "");
            PropertyDescription(const wxString& label, const wxString& name, const wxArrayString& value, const wxString& hint = "");
            PropertyDescription(const wxString& label, const wxString& name, const WxVector3& value);
            PropertyDescription(const wxString& label, const wxString& name, const WxVector4& value);

            PropertyTypes   type                = PropertyTypes::Category;
            wxString        label;
            wxString        name                = wxPG_LABEL;
            wxString        hint;
            wxPGProperty*   parent              = nullptr;
            
            bool            valueBool           = false;
            float           valueFloat          = 0.0f;
            int             valueInt            = 0;
            unsigned int    valueUInt           = 0;
            wxColour        valueColor          = *wxWHITE;
            wxString        valueString;
            wxArrayString   valueArrayString;
            WxVector3       valueVector3;
            WxVector4       valueVector4;

            long            minRange            = 0;
            long            maxRange            = 0;
            bool            hasSpin             = false;
        };

        /* === Functions === */

        UIPropertyGrid(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxPG_SPLITTER_AUTO_CENTER
        );
        virtual ~UIPropertyGrid();

        /* --- Callbacks --- */

        //! This will be called when a property has changed in this property grid.
        virtual void OnApplyProperty(const PropertyState& state);
        //! Notifies this property grid about a change of the specifid property.
        void NotifyPGChanged(wxPGProperty* prop);

        /* --- Static functions --- */

        //! Generates a simple property ID name.
        static wxString GetID(int id);
        /**
        Generates a property ID name with a prefix.
        A prefix should be used in a property grid where several ID numbers will occure more than once.
        */
        static wxString GetID(const wxString& prefix, int id);
        //! Generates a property ID name with a prefix and key string.
        static wxString GetID(const wxString& prefix, const wxString& key);

        /**
        Extracts the ID name from the specified property name.
        This name must have been generated by "UIPropertyGrid::GetId(int id)".
        \param[in] name Specifies the property ID name.
        \retrun The property ID or -1 if the ID extraction failed.
        \see GetId
        */
        static int ExtractPropertyID(const wxString& name);
        /**
        Extracts the key from the specified property name.
        This name must have been generated by "UIPropertyGrid::GetId(int id)".
        \param[in] name Specifies the property ID name.
        \retrun The property key or the input name if there is no separated key.
        \see GetId
        */
        static wxString ExtractPropertyKey(const wxString& name);
        /**
        Extracts the ID name and the prefix from the specified property name.
        This name must have been generated by "UIPropertyGrid::GetId(const wxString& prefix, int id)".
        \param[in] name Specifies the property ID name.
        \param[out] id Specifies the resulting property ID. This will be -1 if the extractionn failed.
        \param[out] prefix Specifies the resulting name prefix.
        \retrun True on success, otherwise the extraction failed.
        \see GetId
        */
        static bool ExtractPropertyIDAndPrefix(const wxString& name, int& id, wxString& prefix);

        /* --- Property category --- */

        wxPropertyCategory*     AddPropertyCategory     (const wxString& label, const wxString& name = wxPG_LABEL, wxPGProperty* parent = nullptr);

        /* --- Properties --- */

        wxBoolProperty*         AddBoolProperty         (const wxString& label, const wxString& name = wxPG_LABEL, bool value = false);
        
        wxFloatProperty*        AddFloatProperty        (const wxString& label, const wxString& name = wxPG_LABEL, float value = 0.0f);
        
        wxIntProperty*          AddIntProperty          (const wxString& label, const wxString& name = wxPG_LABEL, int value = 0, bool hasSpin = false);
        wxIntProperty*          AddIntProperty          (const wxString& label, const wxString& name, int value, long minRange, long maxRange, bool hasSpin = false);
        wxUIntProperty*         AddUIntProperty         (const wxString& label, const wxString& name = wxPG_LABEL, unsigned int value = 0);
        
        wxColourProperty*       AddColorRGBProperty     (const wxString& label, const wxString& name = wxPG_LABEL, const wxColour& value = *wxWHITE);
        wxColourProperty*       AddColorRGBAProperty    (const wxString& label, const wxString& name = wxPG_LABEL, const wxColour& value = *wxWHITE);
        wxSystemColourProperty* AddSystemColorProperty  (const wxString& label, const wxString& name = wxPG_LABEL, const wxColour& value = *wxWHITE);
        
        wxEnumProperty*         AddEnumProperty         (const wxString& label, const wxString& name, wxPGChoices& choices, int value = 0);
        wxEnumProperty*         AddEnumProperty         (const wxString& label, const wxString& name);
        
        wxStringProperty*       AddStringProperty       (const wxString& label, const wxString& name = wxPG_LABEL, const wxString& value = "", const wxString& hint = "");
        wxLongStringProperty*   AddLongStringProperty   (const wxString& label, const wxString& name = wxPG_LABEL, const wxString& value = "", const wxString& hint = "");
        wxArrayStringProperty*  AddArrayStringProperty  (const wxString& label, const wxString& name = wxPG_LABEL, const wxArrayString& value = wxArrayString(), const wxString& hint = "");
        
        wxFileProperty*         AddFileProperty         (
            const wxString& label, const wxString& name = wxPG_LABEL,
            const wxString& value = "", const wxString& hint = "", long style = 0,
            const wxString& dialogLabel = "", const wxString& wildCard = "", const wxString& initialPath = ""
        );
        
        wxDirProperty*          AddDirProperty          (
            const wxString& label, const wxString& name = wxPG_LABEL,
            const wxString& value = "", const wxString& hint = "", const wxString& dialogLabel = ""
        );

        PGVector3Property*      AddVector3Property      (const wxString& label, const wxString& name = wxPG_LABEL, const WxVector3& value = WxVector3());
        PGVector4Property*      AddVector4Property      (const wxString& label, const wxString& name = wxPG_LABEL, const WxVector4& value = WxVector4());

        /* --- Setter --- */

        template <typename T> void SetProperty(const wxString& name, const T& value)
        {
            auto prop = GetPropertyByName(name);
            if (prop)
                prop->SetValue(wxVariant(value));
        }
        
        void SetPropertyBool    (const wxString& name, bool             value);
        void SetPropertyInt     (const wxString& name, int              value);
        void SetPropertyFloat   (const wxString& name, float            value);
        void SetPropertyEnum    (const wxString& name, int              value);
        void SetPropertyEnum    (const wxString& name, const wxString&  value);
        void SetPropertyString  (const wxString& name, const wxString&  value);
        void SetPropertyVector3 (const wxString& name, const WxVector3& value);
        void SetPropertyVector4 (const wxString& name, const WxVector4& value);
        void SetPropertyColor   (const wxString& name, const wxColour&  value);

        /* --- Getter --- */

        bool        GetPropertyBool     (const wxString& name) const;
        int         GetPropertyInt      (const wxString& name) const;
        float       GetPropertyFloat    (const wxString& name) const;
        int         GetPropertyEnum     (const wxString& name) const;
        wxString    GetPropertyString   (const wxString& name) const;
        WxVector3   GetPropertyVector3  (const wxString& name) const;
        WxVector4   GetPropertyVector4  (const wxString& name) const;
        wxColour    GetPropertyColor    (const wxString& name) const;

        /* --- Common --- */

        void HideAllCategories();
        void ShowProperty(const wxString& name, bool show = true);

        void AddProperties(const std::initializer_list<PropertyDescription>& propDescList);

        void ChangeColorScheme(const ColorScheme& scheme);

        //! Prints all property names (with prefix) to the log output.
        void DebugPropertyNames();

        static void ChangeColorSchemeForAll(const ColorScheme& scheme);

        /**
        Returns the property with the specified name.
        \tparam T Specifies the property type. This must be a sub class of "wxPGProperty".
        \return Pointer to the wxWidgets property.
        \throws InvalidArgumentException If the property does not exist or
        is from a different type (i.e. the dynamic_cast failed).
        */
        template <class T> T* FindBoundedProperty(const wxString& propName)
        {
            auto prop = dynamic_cast<T*>(GetPropertyByName(propName));
            if (!prop)
            {
                throw InvalidArgumentException(
                    __FUNCTION__, "propName",
                    "Property \"" + propName.ToStdString() + "\" does not exist or is from different type"
                );
            }
            return prop;
        }

    private:

        /* === Functions === */

        //! Callback function when a property has changed.
        void OnPGChanged(wxPropertyGridEvent& event);
        void OnPGStateChanged(const PropertyState& state);

        //! Implements the notification function of the "Observable" interface.
        void NotifyObserver(
            UIPropertyGridObserver& observer,
            const Engine::Component::Types componentType,
            const PropertyState& propertyState
        ) const override;

        /* === Templates === */

        template <class T> inline T* AppendProperty(T* prop)
        {
            Append(prop);
            return prop;
        }

        template <class T> inline T* CreateProperty()
        {
            return AppendProperty(MakeWxObject<T>());
        }
        template <class T, typename... Args> inline T* CreateProperty(Args&&... args)
        {
            return AppendProperty(MakeWxObject<T>(std::forward<Args>(args)...));
        }

        /* === Members === */

        std::vector<wxPropertyCategory*> categoryProperties_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================