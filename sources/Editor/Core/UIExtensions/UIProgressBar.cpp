/*
 * Editor UI progress bar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIProgressBar.h"


namespace Fork
{

namespace Editor
{


UIProgressBar::UIProgressBar(
    wxWindow* parent, const wxPoint& position, const wxSize& size, int range, long style) :
        wxGauge(parent, wxID_ANY, range, position, size, style)
{
}
UIProgressBar::~UIProgressBar()
{
}

void UIProgressBar::ChangeState(float state)
{
    auto range = GetRange();
    if (range > 0)
        SetValue(static_cast<int>(state * range));
}

float UIProgressBar::State() const
{
    auto range = GetRange();
    return range > 0 ? static_cast<float>(GetValue()) / range : 0.0f;
}


} // /namespace Editor

} // /namespace Fork



// ========================