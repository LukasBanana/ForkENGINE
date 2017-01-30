/*
 * Renderer details header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_DETAILS_H__
#define __FORK_RENDERER_DETAILS_H__


namespace Fork
{

namespace Video
{


/**
Hardware renderer details structure.
Contains information about the low-level 3D rendering API (such as OpenGL and Direct3D).
\remarks The default values are derived from the engine design, i.e. the engine uses a left-handed coordinate system,
but OpenGL uses a right-handed coordinate system, therefore the engine balances this to be unique between OpenGL and Direct3D.
*/
struct RendererDetails
{
    //! Specifies whether the origin is on the left-top of the screen. Otherwise it is on the left-bottom.
    bool hasOriginLeftTop               = true;

    /**
    Specifies whether the coordinate system is left-handed (positive Z values point into the screen).
    Otherwise it is right-handed (positive Z values point out of the screen.)
    */
    bool hasLeftHandedCoordinateSystem  = true;
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================