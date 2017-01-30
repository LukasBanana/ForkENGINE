/*
 * Editor timeline header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_HISTORY_TIMELINE_H__
#define __FORK_EDITOR_HISTORY_TIMELINE_H__


//#include 


namespace Fork
{

namespace Editor
{

namespace History
{


//! Class for editor memento timeline.
class Timeline
{
    
    public:
        
        Timeline() = default;

        Timeline(const Timeline&) = delete;
        Timeline& operator = (const Timeline&) = delete;
        
        //void StoreState(const TimelineStatePtr& state);
        //void RestoreState(const TimelineStatePtr& state);

        void Undo();
        void Redo();

        bool CanUndo() const;
        bool CanRedo() const;

        void SetupMementoSize(unsigned int size);
        unsigned int MementoSize() const;

    private:
        
        //...

};


} // /namespace History

} // /namespace Editor

} // /namespace Fork


#endif



// ========================