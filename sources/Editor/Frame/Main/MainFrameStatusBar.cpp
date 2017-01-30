/*
 * Main frame status bar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameStatusBar.h"
#include "../../Core/UIExtensions/UIProgressBar.h"
#include "../../Core/WxHelper.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Language/LanguageLoader.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Editor
{


using namespace StaticLayout::MainFrameLayout::StatusBarLayout;

MainFrameStatusBar::MainFrameStatusBar(wxWindow* parent) :
    wxStatusBar{ parent, wxID_ANY }
{
    BuildLayout();
}

void MainFrameStatusBar::ShowProgressBar(bool show)
{
    progressBar_->Show(show);
}

void MainFrameStatusBar::ChangeProgressBarState(float state)
{
    progressBar_->ChangeState(state);
}

ScopedProgressBarState MainFrameStatusBar::ScopedProgress(unsigned int numSteps)
{
    return std::move(ScopedProgressBarState(progressBar_, numSteps));
}

void MainFrameStatusBar::ChangeProgressDesc(const wxString& text)
{
    SetStatusText(text, 1);
}

void MainFrameStatusBar::SetupErrorCounter(size_t numErrors)
{
    if (numErrors > 0)
        SetStatusText("( " + ToStr(numErrors) + " " + LANG_STRING(Errors) + " )", 2);
    else
        SetStatusText("", 2);
}

void MainFrameStatusBar::SetupRendererInfo(const wxString& text)
{
    SetStatusText(text, 3);
}


/*
 * ======= Private: =======
 */

void MainFrameStatusBar::BuildLayout()
{
    /* Create status bar field */
    const int widths[4] = { progressBarSize.width, 200, 150, 300 };
    SetFieldsCount(4, widths);

    /* Create extra elements for some status bar fields */
    BuildProgressBar();
    //BuildErrorReporterHint();
}

void MainFrameStatusBar::BuildProgressBar()
{
    /* Create progress bar */
    progressBar_ = MakeWxObject<UIProgressBar>(
        this,
        wxPoint(5, GetSize().GetHeight()/2 - progressBarSize.height/2),
        wxSize(progressBarSize.width - 5, progressBarSize.height)
    );
    progressBar_->Show(false);
}

void MainFrameStatusBar::BuildErrorReporterHint()
{
    /* Create button for error reporter hint */
    errorReporterHint_ = MakeWxObject<UIButton>(
        this,
        wxPoint(
            progressBarSize.width + 345 - errorHintButtonSize.width,
            GetSize().GetHeight()/2 - errorHintButtonSize.height/2
        ),
        wxSize(errorHintButtonSize.width, errorHintButtonSize.height),
        "^"
    );
}


} // /namespace Editor

} // /namespace Fork



// ========================