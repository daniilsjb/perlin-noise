# Perlin Noise

![Preview Image](.github/noise.png)

## About

Quick and simple implementation of Perlin Noise algorithm in 1D, 2D, and 3D in C++11 with no external dependencies.
This code may be easily used by simply including the header into any project along with a `DB_PERLIN_IMPL` definition.
All relevant code is written as a small set of pure functions that work with both floats and doubles.

I wrote this with the primary goal of having a bit of fun and learning more about the famous algorithm used everywhere
in procedural generation. Ultimately, my goal was to use this implementation in several other projects (it's always good
to have a noise generator lying around). I hope it could be useful to other people, too!

## Usage

If you're familiar with [stb][0] libraries, there is nothing new to learn here: simply copy the entire `db_perlin.hpp`
file somewhere in your project and provide exactly one definition of `DB_PERLIN_IMPL` in a source file before including
the header. You may create a dedicated file to serve as the implementation:

```cpp
// file: db_perlin.cpp

#define DB_PERLIN_IMPL
#include "db_perlin.hpp"
```

Then you may use it from other files as you normally would any header file:

```cpp
// file: main.cpp

#include "db_perlin.hpp"

int main() {
double x = /* ... */;
double y = /* ... */;
double z = /* ... */;

double noise1D = db::perlin(x);
double noise2D = db::perlin(x, y);
double noise3D = db::perlin(x, y, z);
}
```

If you only need to use the library once you may avoid creating a dedicated implementation file and treat your program
source as the implementation:

```cpp
// file: main.cpp

#define DB_PERLIN_IMPL
#include "db_perlin.hpp"

int main() {
double x = /* ... */;
double y = /* ... */;
double z = /* ... */;

double noise1D = db::perlin(x);
double noise2D = db::perlin(x, y);
double noise3D = db::perlin(x, y, z);
}
```

Additionally, you may use this library in your JavaScript application by compiling the code as a WASM module using [Emscripten][1].
To automatically generate bindings, simply include the `db_perlin_wasm.hpp` header alongside the implementation:

```cpp
// file: db_perlin.cpp

#define DB_PERLIN_IMPL
#include "db_perlin.hpp"
#include "db_perlin_wasm.hpp"
```

## Examples

This repository contains two examples of noise generation:

* C++ program which generates a .BMP file of the 2D noise gradient ([examples/bmp](examples/bmp)).
* WASM-based JavaScript application with interactive controls, zooming, and panning ([examples/wasm](examples/wasm)).

[0]: https://github.com/nothings/stb
[1]: https://emscripten.org/
