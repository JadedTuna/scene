scene
=====

Python bindings for SDL.

Installation
------------

You can install it from source using `python setup.py build` or by downloading precompiled binaries at the bottom of README (only for Linux for now).
To install and use it you need `SDL` and `SDL_gfx` libraries. Both can be obtained using your packaging managers or from their websites: http://libsdl.org and http://sourceforge.net/projects/sdlgfx/.

Make sure to download files from `python` directory. Then put `scene.py`, `_scene_types.py` and `_scene.so` (rename `scene.so` to `_scene.so` if needed) in the same directory and make sure it is in the PYTHONPATH. Then you can just `import scene` and it should work :].

Usage
-----

For now it supports only basic functions, see [SUPPORTED.md](SUPPORTED.md). If you want to change window's size, set environ variable `SCENE_SIZE` to `SX SY`, where `SX` is width and `SY` is height. Example: `export SCENE_SIZE="320 480"`.

Binaries
--------

Linux: http://victorkindhart.com/projects/scene/_scene.so
