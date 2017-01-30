/*
 * Editor scoped progress bar state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_SCOPED_PROGRESS_BAR_STATE_H__
#define __FORK_EDITOR_SCOPED_PROGRESS_BAR_STATE_H__


namespace Fork
{

namespace Editor
{


class UIProgressBar;

//! Scoped progress bar state transition class for the main frame's status bar.
class ScopedProgressBarState
{
    
    public:
        
        /**
        Scoped progress bar state constructor.
        \param[in] numSteps Specifies the number of progress steps. This will be clamped to [1 .. +inf).
        \throws NullPointerException If 'progressBar' is empty.
        */
        ScopedProgressBarState(UIProgressBar* progressBar, unsigned int numSteps, bool hideAfterProgress = true);
        ScopedProgressBarState(ScopedProgressBarState&& other);
        ~ScopedProgressBarState();

        //! Increases the progress bar state.
        void NextStep();

    private:
        
        float ProgressStep(unsigned int numSteps) const;

        UIProgressBar*  progressBar_        = nullptr;

        float           progress_           = 0.0f;
        float           progressStep_       = 0.0f;

        bool            hideAfterProgress_  = true;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================