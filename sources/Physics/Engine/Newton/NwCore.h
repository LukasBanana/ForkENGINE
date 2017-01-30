/*
 * Newton core header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_CORE_H__
#define __FORK_NW_CORE_H__


#define CAST_TO_NW_OBJECT(type, obj) CastToConcreteObject<Nw##type>(obj, __FUNCTION__, #type, "Nw" #type)


#endif



// ========================