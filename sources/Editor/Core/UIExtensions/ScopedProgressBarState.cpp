/*
 * Editor scoped progress bar state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ScopedProgressBarState.h"
#include "UIProgressBar.h"
#include "Core/Exception/NullPointerException.h"

#include <algorithm>


namespace Fork
{

namespace Editor
{


ScopedProgressBarState::ScopedProgressBarState(
    UIProgressBar* progressBar, unsigned int numSteps, bool hideAfterProgress) :
        progressBar_        { progressBar            },
        progressStep_       { ProgressStep(numSteps) },
        hideAfterProgress_  { hideAfterProgress      }
{
    ASSERT_POINTER(progressBar);

    if (hideAfterProgress_)
        progressBar_->Show(true);

    progressBar_->SetValue(0);
}
ScopedProgressBarState::ScopedProgressBarState(ScopedProgressBarState&& other) :
    progressBar_        { other.progressBar_       },
    progress_           { other.progress_          },
    progressStep_       { other.progressStep_      },
    hideAfterProgress_  { other.hideAfterProgress_ }
{
    other.hideAfterProgress_ = false;
}
ScopedProgressBarState::~ScopedProgressBarState()
{
    if (hideAfterProgress_)
        progressBar_->Show(false);
}

void ScopedProgressBarState::NextStep()
{
    if (progress_ < 1.0f)
    {
        progress_ += progressStep_;
        progressBar_->ChangeState(progress_);
    }
    else
        progressBar_->SetValue(progressBar_->GetRange());
}


/*
 * ======= Private: =======
 */

float ScopedProgressBarState::ProgressStep(unsigned int numSteps) const
{
    return 1.0f / static_cast<float>(std::max(1u, numSteps));
}


} // /namespace Editor

} // /namespace Fork



// ========================