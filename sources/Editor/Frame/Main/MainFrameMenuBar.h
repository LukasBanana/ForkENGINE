/*
 * Main frame menu bar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_MENU_BAR_H__
#define __FORK_EDITOR_MAIN_FRAME_MENU_BAR_H__


#include <wx/menu.h>
#include <array>


namespace Fork
{

namespace Editor
{


class UIMenuList;

class MainFrameMenuBar : public wxMenuBar
{
    
    public:
        
        //! Menu lists enumeration.
        enum class MenuLists
        {
            Editor = 0,
            Project,
            Entity,
            Edit,
            //View,
            Help,

            __Num__
        };

        MainFrameMenuBar();

        UIMenuList* MenuList(const MenuLists menuList) const;

        static void BuildMenuOpenRecent         (UIMenuList* menuList);
        static void BuildMenuSelectorOrientation(UIMenuList* menuList);
        static void BuildMenuSelectorPivot      (UIMenuList* menuList);
        static void BuildMenuRaster             (UIMenuList* menuList);
        static void BuildMenuWireframe          (UIMenuList* menuList);
        static void BuildMenuEditorViews        (UIMenuList* menuList);
        static void BuildMenuComponent          (UIMenuList& menuList);

    private:
        
        void BuildMenuListEditor    (UIMenuList* menuList);
        void BuildMenuListProject   (UIMenuList* menuList);
        void BuildMenuListEntity    (UIMenuList* menuList);
        void BuildMenuListEdit      (UIMenuList* menuList);
        void BuildMenuListHelp      (UIMenuList* menuList);

        std::array<UIMenuList*, static_cast<unsigned int>(MenuLists::__Num__)> menuLists_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================