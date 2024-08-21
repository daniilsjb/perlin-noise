# Perlin Noise

![Preview Image](.github/noise.png)

## Features

* Simple implementation of the Perlin noise algorithm in 1D, 2D, and 3D.
* Pure C++11 without any dependencies on standard or external libraries.
* Templated functions with specializations for `float` and `double` data types.
* An [stb][0]-style single-header library that can be easily included into any project.
* Support for WebAssembly compilation through [Emscripten bindings](./db_perlin_wasm.hpp).
* Interactive [web demonstration](https://daniilsjb.github.io/perlin-noise/) with sliders, zooming and panning.
* Example of writing a noise gradient to a [simple bitmap file](./examples/bmp/main.cpp).

## About

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

[0]: https://github.com/nothings/stb
[1]: https://emscripten.org/
