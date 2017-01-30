/*
 * Editor UI progress bar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROGRESS_BAR_H__
#define __FORK_EDITOR_UI_PROGRESS_BAR_H__


#include <wx/gauge.h>


namespace Fork
{

namespace Editor
{


class UIProgressBar : public wxGauge
{
    
    public:
        
        UIProgressBar(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, int range = 1000, long style = wxGA_HORIZONTAL
        );
        virtual ~UIProgressBar();

        //! Changes the progress bar state. This is in the range [0.0 .. 1.0].
        void ChangeState(float state);
        //! Returns the progress bar state. This is in the range [0.0 .. 1.0].
        float State() const;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================