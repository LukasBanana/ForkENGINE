/*
 * Python "Keyboard" module file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PyModuleImportHelper.h"
#include "IO/InputDevice/Keyboard.h"

#include <functional>


namespace Fork
{

namespace Script
{


#ifndef _USE_BOOST_PYTHON_

using namespace std::placeholders;

/* --- Module functions --- */

static PyObject* Keyboard_AbstractKeyEvent(
    const std::function<bool (const IO::KeyCodes)>& keyEventFunc, PyObject* self, PyObject* args)
{
    /* Parse arguments */
    int keyCode;
    if (!PyArg_ParseTuple(args, "i", &keyCode))
        return NULL;

    /* Run actual function */
    if (keyEventFunc(static_cast<IO::KeyCodes>(keyCode)))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyObject* Keyboard_KeyDown(PyObject* self, PyObject* args)
{
    return Keyboard_AbstractKeyEvent(std::bind(&IO::Keyboard::KeyDown, IO::Keyboard::Instance(), _1), self, args);
}

static PyObject* Keyboard_KeyHit(PyObject* self, PyObject* args)
{
    return Keyboard_AbstractKeyEvent(std::bind(&IO::Keyboard::KeyHit, IO::Keyboard::Instance(), _1), self, args);
}

static PyObject* Keyboard_KeyHitRepeat(PyObject* self, PyObject* args)
{
    return Keyboard_AbstractKeyEvent(std::bind(&IO::Keyboard::KeyHitRepeat, IO::Keyboard::Instance(), _1), self, args);
}

static PyObject* Keyboard_KeyReleased(PyObject* self, PyObject* args)
{
    return Keyboard_AbstractKeyEvent(std::bind(&IO::Keyboard::KeyReleased, IO::Keyboard::Instance(), _1), self, args);
}


/* --- Module setup --- */

static PyMethodDef Keyboard_Methods[] =
{
    { "KeyDown",        Keyboard_KeyDown,       METH_VARARGS, "Returns true if the specified key is pressed down"     },
    { "KeyHit",         Keyboard_KeyHit,        METH_VARARGS, "Returns true if the specified key was hit"             },
    { "KeyHitRepeat",   Keyboard_KeyHitRepeat,  METH_VARARGS, "Returns true if the specified key was hit (repeating)" },
    { "KeyReleased",    Keyboard_KeyReleased,   METH_VARARGS, "Returns true if the specified key was released"        },
    { NULL, NULL, 0, NULL }
};

static PyModuleDef Keyboard_Module =
{
    PyModuleDef_HEAD_INIT,
    "Keyboard",
    NULL,
    -1,
    Keyboard_Methods
};

PyMODINIT_FUNC PyInit_Keyboard()
{
    auto module = PyModule_Create(&Keyboard_Module);

    typedef IO::KeyCodes Ty;

    AddIntConstantsToModule<IO::KeyCodes>(
        module,
        {
            { "KEY_LBUTTON",                Ty::KeyLButton           },
            { "KEY_RBUTTON",                Ty::KeyRButton           },
            { "KEY_CANCEL",                 Ty::KeyCancel            },
            { "KEY_MBUTTON",                Ty::KeyMButton           },
            { "KEY_XBUTTON1",               Ty::KeyXButton1          },
            { "KEY_XBUTTON2",               Ty::KeyXButton2          },

            { "KEY_BACK",                   Ty::KeyBack              },
            { "KEY_TAB",                    Ty::KeyTab               },
            { "KEY_CLEAR",                  Ty::KeyClear             },
            { "KEY_RETURN",                 Ty::KeyReturn            },
            { "KEY_SHIFT",                  Ty::KeyShift             },
            { "KEY_CONTROL",                Ty::KeyControl           },

            { "KEY_MENU",                   Ty::KeyMenu              },
            { "KEY_PAUSE",                  Ty::KeyPause             },
            { "KEY_CAPITAL",                Ty::KeyCapital           },

            { "KEY_ESCAPE",                 Ty::KeyEscape            },
            { "KEY_SPACE",                  Ty::KeySpace             },
            { "KEY_PAGE_UP",                Ty::KeyPageUp            },
            { "KEY_PAGE_DOWN",              Ty::KeyPageDown          },
            { "KEY_END",                    Ty::KeyEnd               },
            { "KEY_HOME",                   Ty::KeyHome              },
    
            { "KEY_LEFT",                   Ty::KeyLeft              },
            { "KEY_UP",                     Ty::KeyUp                },
            { "KEY_RIGHT",                  Ty::KeyRight             },
            { "KEY_DOWN",                   Ty::KeyDown              },

            { "KEY_SELECT",                 Ty::KeySelect            },
            { "KEY_PRINT",                  Ty::KeyPrint             },
            { "KEY_EXE",                    Ty::KeyExe               },
            { "KEY_SNAPSHOT",               Ty::KeySnapshot          },
            { "KEY_INSERT",                 Ty::KeyInsert            },
            { "KEY_DELETE",                 Ty::KeyDelete            },
            { "KEY_HELP",                   Ty::KeyHelp              },

            { "KEY_0",                      Ty::Key0                 },
            { "KEY_1",                      Ty::Key1                 },
            { "KEY_2",                      Ty::Key2                 },
            { "KEY_3",                      Ty::Key3                 },
            { "KEY_4",                      Ty::Key4                 },
            { "KEY_5",                      Ty::Key5                 },
            { "KEY_6",                      Ty::Key6                 },
            { "KEY_7",                      Ty::Key7                 },
            { "KEY_8",                      Ty::Key8                 },
            { "KEY_9",                      Ty::Key9                 },

            { "KEY_A",                      Ty::KeyA                 },
            { "KEY_B",                      Ty::KeyB                 },
            { "KEY_C",                      Ty::KeyC                 },
            { "KEY_D",                      Ty::KeyD                 },
            { "KEY_E",                      Ty::KeyE                 },
            { "KEY_F",                      Ty::KeyF                 },
            { "KEY_G",                      Ty::KeyG                 },
            { "KEY_H",                      Ty::KeyH                 },
            { "KEY_I",                      Ty::KeyI                 },
            { "KEY_J",                      Ty::KeyJ                 },
            { "KEY_K",                      Ty::KeyK                 },
            { "KEY_L",                      Ty::KeyL                 },
            { "KEY_M",                      Ty::KeyM                 },
            { "KEY_N",                      Ty::KeyN                 },
            { "KEY_O",                      Ty::KeyO                 },
            { "KEY_P",                      Ty::KeyP                 },
            { "KEY_Q",                      Ty::KeyQ                 },
            { "KEY_R",                      Ty::KeyR                 },
            { "KEY_S",                      Ty::KeyS                 },
            { "KEY_T",                      Ty::KeyT                 },
            { "KEY_U",                      Ty::KeyU                 },
            { "KEY_V",                      Ty::KeyV                 },
            { "KEY_W",                      Ty::KeyW                 },
            { "KEY_X",                      Ty::KeyX                 },
            { "KEY_Y",                      Ty::KeyY                 },
            { "KEY_Z",                      Ty::KeyZ                 },

            { "KEY_WIN_LEFT",               Ty::KeyWinLeft           },
            { "KEY_WIN_RIGHT",              Ty::KeyWinRight          },
            { "KEY_APPS",                   Ty::KeyApps              },
            { "KEY_SLEEP",                  Ty::KeySleep             },

            { "KEY_NUMPAD_0",               Ty::KeyNumPad0           },
            { "KEY_NUMPAD_1",               Ty::KeyNumPad1           },
            { "KEY_NUMPAD_2",               Ty::KeyNumPad2           },
            { "KEY_NUMPAD_3",               Ty::KeyNumPad3           },
            { "KEY_NUMPAD_4",               Ty::KeyNumPad4           },
            { "KEY_NUMPAD_5",               Ty::KeyNumPad5           },
            { "KEY_NUMPAD_6",               Ty::KeyNumPad6           },
            { "KEY_NUMPAD_7",               Ty::KeyNumPad7           },
            { "KEY_NUMPAD_8",               Ty::KeyNumPad8           },
            { "KEY_NUMPAD_9",               Ty::KeyNumPad9           },

            { "KEY_MULTIPLY",               Ty::KeyMultiply          },
            { "KEY_ADD",                    Ty::KeyAdd               },
            { "KEY_SEPARATOR",              Ty::KeySeparator         },
            { "KEY_SUBTRACT",               Ty::KeySubtract          },
            { "KEY_DECIMAL",                Ty::KeyDecimal           },
            { "KEY_DIVIDE",                 Ty::KeyDivide            },

            { "KEY_F1",                     Ty::KeyF1                },
            { "KEY_F2",                     Ty::KeyF2                },
            { "KEY_F3",                     Ty::KeyF3                },
            { "KEY_F4",                     Ty::KeyF4                },
            { "KEY_F5",                     Ty::KeyF5                },
            { "KEY_F6",                     Ty::KeyF6                },
            { "KEY_F7",                     Ty::KeyF7                },
            { "KEY_F8",                     Ty::KeyF8                },
            { "KEY_F9",                     Ty::KeyF9                },
            { "KEY_F10",                    Ty::KeyF10               },
            { "KEY_F11",                    Ty::KeyF11               },
            { "KEY_F12",                    Ty::KeyF12               },
            { "KEY_F13",                    Ty::KeyF13               },
            { "KEY_F14",                    Ty::KeyF14               },
            { "KEY_F15",                    Ty::KeyF15               },
            { "KEY_F16",                    Ty::KeyF16               },
            { "KEY_F17",                    Ty::KeyF17               },
            { "KEY_F18",                    Ty::KeyF18               },
            { "KEY_F19",                    Ty::KeyF19               },
            { "KEY_F20",                    Ty::KeyF20               },
            { "KEY_F21",                    Ty::KeyF21               },
            { "KEY_F22",                    Ty::KeyF22               },
            { "KEY_F23",                    Ty::KeyF23               },
            { "KEY_F24",                    Ty::KeyF24               },

            { "KEY_NUM_LOCK", Ty            ::KeyNumLock           },
            { "KEY_SCROLL",                 Ty::KeyScroll            },

            { "KEY_LSHIFT",                 Ty::KeyLShift            },
            { "KEY_RSHIFT",                 Ty::KeyRShift            },
            { "KEY_LCONTROL",               Ty::KeyLControl          },
            { "KEY_RCONTROL",               Ty::KeyRControl          },
            { "KEY_LMENU",                  Ty::KeyLMenu             },
            { "KEY_RMENU",                  Ty::KeyRMenu             },

            { "KEY_BROWSER_BACK",           Ty::KeyBrowserBack       },
            { "KEY_BROWSER_FORWARD",        Ty::KeyBrowserForward    },
            { "KEY_BROWSER_REFRESH",        Ty::KeyBrowserRefresh    },
            { "KEY_BROWSER_STOP",           Ty::KeyBrowserStop       },
            { "KEY_BROWSER_SEARCH",         Ty::KeyBrowserSearch     },
            { "KEY_BROWSER_FAVORITS",       Ty::KeyBrowserFavorits   },
            { "KEY_BROWSER_HOME",           Ty::KeyBrowserHome       },

            { "KEY_VOLUME_MUTE",            Ty::KeyVolumeMute        },
            { "KEY_VOLUME_DOWN",            Ty::KeyVolumeDown        },
            { "KEY_VOLUME_UP",              Ty::KeyVolumeUp          },

            { "KEY_MEDIA_NEXT_TRACK",       Ty::KeyMediaNextTrack    },
            { "KEY_MEDIA_PREV_TRACK",       Ty::KeyMediaPrevTrack    },
            { "KEY_MEDIA_STOP",             Ty::KeyMediaStop         },
            { "KEY_MEDIA_PLAY_PAUSE",       Ty::KeyMediaPlayPause    },

            { "KEY_LAUNCH_MAIL",            Ty::KeyLaunchMail        },
            { "KEY_LAUNCH_MEDIA_SELECT",    Ty::KeyLaunchMediaSelect },
            { "KEY_LAUNCH_APP1",            Ty::KeyLaunchApp1        },
            { "KEY_LAUNCH_APP2",            Ty::KeyLaunchApp2        },

            { "KEY_PLUS",                   Ty::KeyPlus              },
            { "KEY_COMMA",                  Ty::KeyComma             },
            { "KEY_MINUS",                  Ty::KeyMinus             },
            { "KEY_PERIOD",                 Ty::KeyPeriod            },

            { "KEY_EXPONENT",               Ty::KeyExponent          },

            { "KEY_AATN",                   Ty::KeyAttn              },
            { "KEY_CRSEL",                  Ty::KeyCrSel             },
            { "KEY_EXSEL",                  Ty::KeyExSel             },
            { "KEY_EREOF",                  Ty::KeyErEOF             },
            { "KEY_PLAY",                   Ty::KeyPlay              },
            { "KEY_ZOOM",                   Ty::KeyZoom              },
            { "KEY_NONAME",                 Ty::KeyNoName            },
            { "KEY_PA1",                    Ty::KeyPA1               },
            { "KEY_OEMCLEAR",               Ty::KeyOEMClear          },
        }
    );

    return module;
}

#endif


} // /namespace Script

} // /namespace Fork



// ========================