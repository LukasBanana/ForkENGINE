/*
 * Python module import helper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PY_MODULE_IMPORT_HELPER_H__
#define __FORK_PY_MODULE_IMPORT_HELPER_H__


#include "../PyScriptCore.h"

#include <functional>
#include <initializer_list>
#include <utility>


namespace Fork
{

namespace Script
{


template <typename T> void AddIntConstantsToModule(
    PyObject* module, const std::initializer_list<std::pair<const char*, T>>& constants)
{
    for (const auto& value : constants)
    {
        PyModule_AddIntConstant(
            module, value.first, static_cast<long>(value.second)
        );
    }
}


} // /namespace Script

} // /namespace Fork


#endif



// ========================