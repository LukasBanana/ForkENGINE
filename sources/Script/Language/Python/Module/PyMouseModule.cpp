/*
 * Python "Mouse" module file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PyModuleImportHelper.h"
#include "IO/InputDevice/Mouse.h"


namespace Fork
{

namespace Script
{


#ifndef _USE_BOOST_PYTHON_

using namespace std::placeholders;

/* --- Module functions --- */

static PyObject* Mouse_AbstractKeyEvent(
    const std::function<bool (const IO::MouseKeyCodes)>& keyEventFunc, PyObject* self, PyObject* args)
{
    /* Parse arguments */
    int keyCode;
    if (!PyArg_ParseTuple(args, "i", &keyCode))
        return NULL;

    /* Run actual function */
    if (keyEventFunc(static_cast<IO::MouseKeyCodes>(keyCode)))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyObject* Mouse_KeyDown(PyObject* self, PyObject* args)
{
    return Mouse_AbstractKeyEvent(std::bind(&IO::Mouse::ButtonDown, IO::Mouse::Instance(), _1), self, args);
}

static PyObject* Mouse_KeyHit(PyObject* self, PyObject* args)
{
    return Mouse_AbstractKeyEvent(std::bind(&IO::Mouse::ButtonHit, IO::Mouse::Instance(), _1), self, args);
}

static PyObject* Mouse_KeyReleased(PyObject* self, PyObject* args)
{
    return Mouse_AbstractKeyEvent(std::bind(&IO::Mouse::ButtonReleased, IO::Mouse::Instance(), _1), self, args);
}

static PyObject* Mouse_KeyDoubleClicked(PyObject* self, PyObject* args)
{
    return Mouse_AbstractKeyEvent(std::bind(&IO::Mouse::ButtonDoubleClicked, IO::Mouse::Instance(), _1), self, args);
}


/* --- Module setup --- */

static PyMethodDef Mouse_Methods[] =
{
    { "ButtonDown",             Mouse_KeyDown,          METH_VARARGS, "Returns true if the specified mouse button is pressed down" },
    { "ButtonHit",              Mouse_KeyHit,           METH_VARARGS, "Returns true if the specified mouse button was hit"         },
    { "ButtonReleased",         Mouse_KeyReleased,      METH_VARARGS, "Returns true if the specified mouse button was released"    },
    { "ButtonDoubleClicked",    Mouse_KeyDoubleClicked, METH_VARARGS, "Returns true if the specified key was hit (repeating)"      },
    { NULL, NULL, 0, NULL }
};

static PyModuleDef Mouse_Module =
{
    PyModuleDef_HEAD_INIT,
    "Mouse",
    NULL,
    -1,
    Mouse_Methods
};

PyMODINIT_FUNC PyInit_Mouse()
{
    auto module = PyModule_Create(&Mouse_Module);

    typedef IO::MouseKeyCodes Ty;

    AddIntConstantsToModule<IO::MouseKeyCodes>(
        module,
        {
            { "MOUSE_LEFT",     Ty::MouseLeft     },
            { "MOUSE_RIGHT",    Ty::MouseRight    },
            { "MOUSE_MIDDLE",   Ty::MouseMiddle   },
            { "MOUSE_XBUTTON1", Ty::MouseXButton1 },
            { "MOUSE_XBUTTON2", Ty::MouseXButton2 },
        }
    );

    return module;
}

#endif


} // /namespace Script

} // /namespace Fork



// ========================