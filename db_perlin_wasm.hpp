/*
 * db-perlin - see license at the bottom, no warranty implied, use at your own risk;
 *     made by daniilsjb (https://github.com/daniilsjb/perlin-noise)
 *
 * The following is a set of Emscripten bindings which can be used to call the noise
 * functions from JavaScript code after compiling the library as a WebAssembly module.
 * This allows the noise to be used for procedural generation in web applications.
 *
 * For instructions on how to integrate Emscripten into your project, refer to the tool's
 * official documentation (https://emscripten.org/). For a simple example of using the
 * library as a standalone WASM module, refer to the /examples/wasm/ directory.
 *
 * Usage:
 *
 * This header does not expose any new declarations. Instead, the bindings must be included
 * as part of any C++ source file used to compile the WASM module. It is recommended to put
 * this header in the same place where the library itself is implemented, as follows:
 *
 * ```cpp
 * #define DB_PERLIN_IMPL
 * #include "db_perlin.hpp"
 * #include "db_perlin_wasm.hpp"
 * ```
 *
 * Note that it is required for db_perlin.hpp to be included prior to this header. Naturally,
 * this header expects that the code is compiled using Emscripten. To conditionally support WASM
 * compilation, you may place the inclusion within a preprocessor guard, like so:
 *
 * ```cpp
 * #define DB_PERLIN_IMPL
 * #include "db_perlin.hpp"
 *
 * #ifdef __EMSCRIPTEN__
 * #include "db_perlin_wasm.hpp"
 * #endif
 * ```
 *
 * In JavaScript code, each overload of the noise function is assigned a unique name in the format
 * of `perlin<n><type>`, where `n` is the number of dimensions and `type` is a short prefix of the
 * template specialization type. For example, `perlin1f` is the 1D noise using `float` numbers,
 * whereas `perlin3d` is the 3D noise using `double` numbers.
 */

#ifndef DB_PERLIN_WASM_HPP
#define DB_PERLIN_WASM_HPP

#ifndef DB_PERLIN_HPP
#error "Please, include db_perlin.hpp before db_perlin_wasm.hpp"
#endif

#ifndef __EMSCRIPTEN__
#error "Please, compile with Emscripten to enable WASM support"
#endif

#include <emscripten.h>
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(db_perlin) {
    emscripten::function("perlin1f", emscripten::select_overload<float(float)>(&db::perlin));
    emscripten::function("perlin2f", emscripten::select_overload<float(float, float)>(&db::perlin));
    emscripten::function("perlin3f", emscripten::select_overload<float(float, float, float)>(&db::perlin));

    emscripten::function("perlin1d", emscripten::select_overload<double(double)>(&db::perlin));
    emscripten::function("perlin2d", emscripten::select_overload<double(double, double)>(&db::perlin));
    emscripten::function("perlin3d", emscripten::select_overload<double(double, double, double)>(&db::perlin));
}

#endif // DB_PERLIN_WASM_HPP

/*
MIT License

Copyright (c) 2020-2024 Daniils Buts

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
