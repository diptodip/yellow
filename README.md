# yellow🍂
Simple implementation of path tracing, based on the [Ray Tracing in One
Weekend](https://raytracing.github.io/) series. Not good -- just an
exercise for different ways to speed up a program.

![preview image of spheres](/img/preview.bmp)

## Description
* *Very* simple raytracing
* No configuration or command line options, you can only write a new function
  or change existing variables in the code and recompile the whole thing
* Multithreaded ray tracing with either POSIX threads or Windows threads
* Almost definitely way slower than it could/should be

## How to build
I know that this build works using `clang` to compile, as used in the build
scripts. If you want to use a different compiler, `gcc` might also work.
Currently, I don't think `msvc` will work because I'm trying out using a
compiler extension for locked adds.

## Usage
There are no arguments. Just run
```
./build.sh
```
on \*nix or
```
.\build.bat
```
on Windows.

This will build the program and immediately render a `384 x 216` bitmap. If you
want to change the scene or render a larger image you'll have to modify
`yellow.cpp`. Note that you'll need `clang` in order to compile using these
scripts. I'm also using a `__sync_fetch_and_add` compiler extension.
