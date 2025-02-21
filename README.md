# Substrate_SDL3
Implementation of greate substrate screen saver in SDL3.

## Where it comes from
Implementation is based on a [SDL3 hello](https://github.com/libsdl-org/SDL/blob/main/docs/hello.c) and [Haiku](https://github.com/stephanie-fu/Substrate) implementation of substrate algorithm.


Do not forget to visit webpage of a original author of substrate algorithm:
[Jared Tarbell](http://www.complexification.net/gallery/machines/substrate/)


## Build for desktop
Assuming that you have CMake installed and C/C++ compiler.
Create a build directory named ***build*** and enter it.
Then just type:

    cmake ..\
    cmake --build .

Executable should be created.
Note that it's dynamically linked binary so you have to make sure
that necessary dynamic libs are visible. 
For SDL3 on Windows platform you can find it at:

    build\_deps\sdl3-build\Debug

Simply copy it next to your substrate.exe and run.

## Build for Web

Follow the instruction to install [emscripten](https://emscripten.org/docs/getting_started/downloads.html).
Then create build directory called ***build_emscripten*** and eneter it.
You have to type following commands:

    emcmake cmake -S ..\
    emcmake cmake --build .
    python3 -m  http.server 9000

It will build substrate for web, and run local webserver.
Then simply open your browser and type
    127.0.0.1:9000/substrate.html

## ***NOTE***
Above instructions are suitable for Windows platform.
On Linux change the path separator to proper one.
