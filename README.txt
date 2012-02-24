How to make and run the examples:
---------------------------------

1. Please make sure, you have installed a Windows OpenGL 3.2 driver. For some examples you need a OpenGL 4.2 driver. As Mac OS X only supports OpenGL 3.2, these examples do not work.
2. Install Eclipse IDE for C/C++ Developers and a GNU Compiler Collection for your operating system.
3. Extract this ZIP file and set the workspace-cpp folder as your Eclipse workspace.
4. Import each folder as a separate, existing project.
5. Set the build configuration in Eclispe to your operating system.
6. Build GLUS.
7. Build any of the examples.
8. The executables are located in the Binaries folder. Execute them directly or create a run configuration in Eclipse.

If you get build errors:
------------------------
- Please make sure, that you install all the needed header and libraries.
- Pre-build libraries are located in the External project folder. Replace these libraries with your own build.

If the examples are not working with your NVIDIA card:
------------------------------------------------------

If the OpenGL 3.2 examples are not working, please try the following:
- Replace "#version 150" in the shaders with "#version 330"
- Init GLUS with "glusPrepareContext(3, 3, GLUS_FORWARD_COMPATIBLE_BIT);"

17.02.2012 Norbert Nopper (norbert@nopper.tv)
