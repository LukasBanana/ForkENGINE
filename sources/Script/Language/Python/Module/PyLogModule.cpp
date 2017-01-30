/*
 * Python "Log" module file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../PyScriptCore.h"
#include "IO/Core/Log.h"

#include <functional>


namespace Fork
{

namespace Script
{


#ifdef _USE_BOOST_PYTHON_

void _Log_Message(const char* arg0)
{
    IO::Log::Message(arg0);
}

BOOST_PYTHON_MODULE(Log)
{
    PyEval_InitThreads();
    PyThreadState* st = PyEval_SaveThread();
    using namespace boost::python;
    def("Message", _Log_Message);
}

#else

/* --- Module functions --- */

static PyObject* Log_AbstractMessage(
    const std::function<void (const std::string&)> logFunc, PyObject* self, PyObject* args)
{
    /* Parse arguments */
    char* input;
    if (!PyArg_ParseTuple(args, "s", &input))
        return NULL;

    /* Run actual function */
    auto msg = std::string(input);
    if (msg != "\n")
        logFunc(msg);

    Py_RETURN_NONE;
}

static PyObject* Log_Message(PyObject* self, PyObject* args)
{
    return Log_AbstractMessage([](const std::string& msg) { IO::Log::Message(msg); }, self, args);
}

static PyObject* Log_Warning(PyObject* self, PyObject* args)
{
    return Log_AbstractMessage([](const std::string& msg) { IO::Log::Warning(msg); }, self, args);
}

static PyObject* Log_Error(PyObject* self, PyObject* args)
{
    return Log_AbstractMessage([](const std::string& msg) { IO::Log::Error(msg); }, self, args);
}

static PyObject* Log_Success(PyObject* self, PyObject* args)
{
    return Log_AbstractMessage([](const std::string& msg) { IO::Log::Success(msg); }, self, args);
}

static PyObject* Log_IncIndent(PyObject* self, PyObject* args)
{
    IO::Log::IncIndent();
    Py_RETURN_NONE;
}

static PyObject* Log_DecIndent(PyObject* self, PyObject* args)
{
    IO::Log::DecIndent();
    Py_RETURN_NONE;
}


/* --- Module setup --- */

static PyMethodDef Log_Methods[] =
{
    { "Message",    Log_Message,    METH_VARARGS,   "Print a log message"        },
    { "Warning",    Log_Warning,    METH_VARARGS,   "Print a log error"          },
    { "Error",      Log_Error,      METH_VARARGS,   "Print a log warning"        },
    { "Success",    Log_Success,    METH_VARARGS,   "Print a log success"        },
    { "IncIndent",  Log_IncIndent,  METH_NOARGS,    "Increments the indentation" },
    { "DecIndent",  Log_DecIndent,  METH_NOARGS,    "Decrements the indentation" },
    { NULL, NULL, 0, NULL }
};

static PyModuleDef Log_Module =
{
    PyModuleDef_HEAD_INIT,
    "Log",
    NULL,
    -1,
    Log_Methods
};

PyMODINIT_FUNC PyInit_Log()
{
    return PyModule_Create(&Log_Module);
}

#endif


} // /namespace Script

} // /namespace Fork



// ========================