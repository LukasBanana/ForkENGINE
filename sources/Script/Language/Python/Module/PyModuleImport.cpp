/*
 * Python module import file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../PyScriptCore.h"
#include "PyModuleImport.h"


namespace Fork
{

namespace Script
{


#define DECL_PY_MODULE(name) PyMODINIT_FUNC PyInit_##name()
#define IMPORT_PY_MODULE(name) PyImport_AppendInittab(#name, PyInit_##name)


/* --- Declare --- */

DECL_PY_MODULE( Log        );
#ifndef _USE_BOOST_PYTHON_
DECL_PY_MODULE( Keyboard   );
DECL_PY_MODULE( Mouse      );
#endif
DECL_PY_MODULE( PrimRender );


/* --- Import --- */

void ImportAllPythonModules()
{
    IMPORT_PY_MODULE( Log        );
    #ifndef _USE_BOOST_PYTHON_
    IMPORT_PY_MODULE( Keyboard   );
    IMPORT_PY_MODULE( Mouse      );
    #endif
    IMPORT_PY_MODULE( PrimRender );
}


#undef DECL_PY_MODULE
#undef IMPORT_PY_MODULE


} // /namespace Script

} // /namespace Fork



// ========================