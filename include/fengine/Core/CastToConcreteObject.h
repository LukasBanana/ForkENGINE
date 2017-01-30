/*
 * Cast-to-concrete-object template header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CAST_TO_CONCRETE_OBJECT_H__
#define __FORK_CAST_TO_CONCRETE_OBJECT_H__


#include "Core/StaticConfig.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/PointerConversionException.h"

#include <type_traits>


namespace Fork
{


/**
Casts the specified interface object into a specific implementation object.
\tparam Dest Specifies the destination pointer type. This must be a base class of 'Src'.
\tparam Src Specifies the source pointer type. This will be autmatically deduced.
\param[in] sourceObject Pointer to the source object. This must never be null!
\param[in] procName Name of the caller procedure. Commonly set to "__FUNCTION__".
\param[in] sourceName Name of the source class type.
\param[in] destName Name of the destination class type.
\throws NullPointerException (Only in debug mode) If 'sourceObject' is null.
\throws PointerConversionException (Only in debug mode) If 'sourceObject' can not be casted to 'Dest'.
\remarks Usage example:
\code
struct Base { };
struct Concrete : public Base { };

void DoSomethingWithBase(Base* interfaceObject)
{
    auto concreteObject = CastToConcreteObject<Concrete>(interfaceObject, __FUNCTION__, "Base", "Concrete");
}

Concrete object;
DoSomethingWithBase(&object);
\endcode
*/
template <class Dest, class Src> Dest* CastToConcreteObject(
    Src* sourceObject, const char* procName, const char* sourceName, const char* destName)
{
    static_assert(
        !std::is_pointer<Dest>::value && !std::is_pointer<Src>::value,
        "\"" __FUNCTION__ "\" does not allow pointer types"
    );

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    
    /* Null pointer exception (for debugging) */
    if (!sourceObject)
        throw NullPointerException(procName, sourceName);
    
    #endif

    #ifdef FORK_DEBUG
    
    /* Dynamic cast (for debugging) */
    auto destObject = dynamic_cast<Dest*>(sourceObject);
    if (!destObject)
        throw PointerConversionException(procName, sourceName, destName);
    return destObject;

    #else

    /* Static cast */
    return static_cast<Dest*>(sourceObject);

    #endif
}


} // /namespace Fork


#endif



// ========================