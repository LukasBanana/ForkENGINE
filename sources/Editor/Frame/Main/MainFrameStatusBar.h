/*
 * Main frame status bar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_STATUS_BAR_H__
#define __FORK_EDITOR_MAIN_FRAME_STATUS_BAR_H__


#include "../../Core/UIExtensions/ScopedProgressBarState.h"
#include "../../Core/UIExtensions/UIButton.h"

#include <wx/statusbr.h>


namespace Fork
{

namespace Editor
{


class UIProgressBar;

class MainFrameStatusBar : public wxStatusBar
{
    
    public:
        
        MainFrameStatusBar(wxWindow* parent);

        void ShowProgressBar(bool show);

        void ChangeProgressBarState(float state);
        void ChangeProgressDesc(const wxString& text);

        void SetupErrorCounter(size_t numErrors);
        void SetupRendererInfo(const wxString& text);

        //! Returns a scoped progress bar state for this status-bar's progress-bar.
        ScopedProgressBarState ScopedProgress(unsigned int numSteps);

        inline UIProgressBar* GetProgressBar() const
        {
            return progressBar_;
        }

    private:
        
        void BuildLayout();
        void BuildProgressBar();
        void BuildErrorReporterHint();

        UIProgressBar*  progressBar_        = nullptr;
        UIButton*       errorReporterHint_  = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================