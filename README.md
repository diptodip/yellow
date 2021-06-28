# yellow🍂
Simple implementation of path tracing, based on the [Ray Tracing in One
Weekend](https://raytracing.github.io/) series. None of this is good
programming -- it's mostly a learning exercise for different ways to speed up a
program.

![720p sphere render](/img/720p.bmp)

## Features
* *Very* simple code
* No scene configuration, you can only write a new function and recompile the
whole program
* Multithreaded ray tracing with either POSIX threads or Windows threading
* Probably way slower than it could/should be

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
