/*
 * Python "PrimitiveRenderer" module file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../PyScriptCore.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"

#include <functional>


namespace Fork
{

namespace Script
{


using namespace std::placeholders;

/* --- Module functions --- */

static PyObject* PrimRender_BeginDrawing2D(PyObject* self, PyObject* args)
{
    /* Run actual function */
    //...

    Py_RETURN_NONE;
}


/* --- Module setup --- */

static PyMethodDef PrimRender_Methods[] =
{
    { "BeginDrawing2D", PrimRender_BeginDrawing2D, METH_VARARGS, "Begins with 2D drawing mode" },
    //{ "EndDrawing2D",   PrimRender_EndDrawing2D, METH_VARARGS, "Ends with 2D drawing mode" },
    { NULL, NULL, 0, NULL }
};

static PyModuleDef PrimRender_Module =
{
    PyModuleDef_HEAD_INIT,
    "PrimitiveRenderer",
    NULL,
    -1,
    PrimRender_Methods
};

PyMODINIT_FUNC PyInit_PrimRender()
{
    return PyModule_Create(&PrimRender_Module);
}


} // /namespace Script

} // /namespace Fork



// ========================