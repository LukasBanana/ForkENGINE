/*
 * Physics material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_MATERIAL_H__
#define __FORK_PHYSICS_MATERIAL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Line.h"


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(Material);

/**
Physics material interface.
Physics materials are used to specify the friction,
softness, elasticity and other settings for physics objects.
*/
class FORK_EXPORT Material
{
    
    public:
        
        /* === Structures == */

        //! Structure for the behavior between two materials.
        struct FORK_EXPORT Behavior
        {
            Behavior() = default;
            Behavior(
                bool collidable,
                float staticFriction = 0.9f, float kineticFriction = 0.5f,
                float elasticity = 0.4f, float softness = 0.1f) :
                    collidable      ( collidable      ),
                    staticFriction  ( staticFriction  ),
                    kineticFriction ( kineticFriction ),
                    elasticity      ( elasticity      ),
                    softness        ( softness        )
            {
            }

            //! Specifies whether two materials are collidable. By default true.
            bool collidable  = true;

            //! Static friction coefficient. By default 0.9.
            float staticFriction = 0.9f;

            //! Kinetic (or rather dynamic) friction coefficient. By default 0.5.
            float kineticFriction = 0.5f;

            //! Elasticity coefficient. This should be less or equal to 1.0. By default 0.4.
            float elasticity = 0.4f;

            /**
            Specifies the softness coefficient. This should be less or equal to 1.0.
            If this is a low value, the materials will be soft. By default 0.1.
            */
            float softness = 0.1f;
        };

        /* === Interface === */

        virtual ~Material()
        {
        }

    protected:
        
        Material() = default;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================