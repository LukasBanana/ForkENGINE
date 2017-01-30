/*
 * Editor error reporter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ErrorReporter.h"
#include "../Frame/Main/MainFrame.h"
#include "../Frame/Main/MainFrameStatusBar.h"


namespace Fork
{

namespace Editor
{


void ErrorReporter::RegisterError(const std::string& message)
{
    errorMessages_.push_back(message);
    RefreshMainFrameStatusBar();
}

void ErrorReporter::RefreshMainFrameStatusBar()
{
    auto mainFrame = MainFrame::Instance();
    if (mainFrame)
        mainFrame->GetMainStatusBar()->SetupErrorCounter(errorMessages_.size());
}

void ErrorReporter::Clear()
{
    errorMessages_.clear();
    RefreshMainFrameStatusBar();
}


} // /namespace Editor

} // /namespace Fork



// ========================