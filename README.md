ForkENGINE
==========

Note that this is an unfinished project, which I (Lukas Hermanns) worked on from 2014 to 2015.
Feel free to use it for 3D demos or prototyping, but I highly recommend you to **not** use it for actual game projects.

<p align="center"><img src="docu/Screenshots/Editor_1.png.png" width="768" height="486"/></p>


License
-------

[3-Clause BSD License](https://github.com/LukasBanana/ForkENGINE/blob/master/LICENSE.txt)


Documentation
-------------

- [Installation Guides](https://github.com/LukasBanana/ForkENGINE/blob/master/docu/TeX/Installation%20Guides/Installation%20Guides.pdf) (PDF)
- [Coding Conventions](https://github.com/LukasBanana/ForkENGINE/blob/master/docu/TeX/Coding%20Conventions/Coding%20Conventions.pdf) (PDF)
- [Name Conventions](https://github.com/LukasBanana/ForkENGINE/blob/master/docu/TeX/Name%20Conventions/Name%20Conventions.pdf) (PDF)


Build
-----

At least *VisualC++ 2012* is required on *Windows 7* or later.

Required third party libraries are:
* Newton Dynamics 3.11 (Collision Detection/ Physics Simulation)
* wxWidgets 3.1.0 (GUI for Editor)
* Ogg/Vorbis (Audio Streaming)

Optional third party libraries are:
* Python 3.2 (Scripting)


Debug
-----

The pre-build binaries require the [VisualC++ 2015 redistributables](https://www.microsoft.com/en-us/download/details.aspx?id=48145) to be installed.

Set the working directory of the editor ("ForkEd" project) to "editor/"
or copy this folder to another location on your system.

For all the tests, the working directory is the respective folder of the test,
e.g. for the RayTracing test it's "tests/RayTracing/".

Most textures and models are **not** included in this repository, so you might add your own textures.


Status
------

Version 0.1 Alpha


Features
--------

* High-Level Editor named "ForkEd"
* Component System
* Modular Loading of Sub Systems
* Supported Renderers: OpenGL 4, Direct3D 11
* Supported Physics Engine: Newton Game Dynamics
