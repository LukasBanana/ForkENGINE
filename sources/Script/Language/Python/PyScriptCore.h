/*
 * Python script core header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PY_SCRIPT_CORE_H__
#define __FORK_PY_SCRIPT_CORE_H__


//#define _USE_BOOST_PYTHON_//!!!
#ifdef _USE_BOOST_PYTHON_

//D:\SoftwareEntwicklung\C++\Tools\Libraries\Python32\include
//D:\SoftwareEntwicklung\C++\Tools\Libraries\Python32\libs\python32.lib
//D:\SoftwareEntwicklung\C++\Tools\Libraries\boost_1_56_0\build_msvc12\include\boost-1_56
//D:\SoftwareEntwicklung\C++\Tools\Libraries\boost_1_56_0\build_msvc12\lib\libboost_python-vc120-mt-gd-1_56.lib
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#else

//D:\SoftwareEntwicklung\C++\Tools\Libraries\Python-3.4.2rc1\Include
//D:\SoftwareEntwicklung\C++\Tools\Libraries\Python-3.4.2rc1\PCbuild\python34_d.lib
#include <Python.h>

#endif


#endif



// ========================