/*
 * To compile the library as a WASM module using the Emscripten compiler, execute
 * the following command in this directory:
 *
 *   em++ db_perlin.cpp -o static/db_perlin.js -lembind -O3
 */

#define DB_PERLIN_IMPL
#include "../../db_perlin.hpp"
#include "../../db_perlin_wasm.hpp"
