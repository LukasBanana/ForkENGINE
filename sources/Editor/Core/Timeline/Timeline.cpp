/*
 * Editor timeline file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Timeline.h"


namespace Fork
{

namespace Editor
{

namespace History
{


//void Timeline::StoreState(const TimelineStatePtr& state);
//void Timeline::RestoreState(const TimelineStatePtr& state);

void Timeline::Undo()
{
    //todo
}

void Timeline::Redo()
{
    //todo
}

bool Timeline::CanUndo() const
{
    return false; //todo
}

bool Timeline::CanRedo() const
{
    return false; //todo
}

void Timeline::SetupMementoSize(unsigned int size)
{
    //todo
}

unsigned int Timeline::MementoSize() const
{
    return 0;//todo
}


} // /namespace History

} // /namespace Editor

} // /namespace Fork



// ========================