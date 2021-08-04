/*
 * db-perlin - see license at the bottom, no warranty implied, use at your own risk;
 *     made by daniilsjb (https://github.com/daniilsjb/perlin-noise)
 *
 * The following is an implementation of Ken Perlin's Improved Noise in 1D, 2D, and 3D.
 * This code has no external dependencies and as such may easily be used as a library
 * in other projects.
 *
 * I wrote this with the primary goal of having a bit of fun and learning more about the
 * famous algorithm used everywhere in procedural generation. Ultimately, my goal was to
 * use this implementation in several other projects (it's always good to have a noise
 * generator lying around). I hope it could be useful to other people, too!
 *
 * Usage:
 *
 * This code is written as a single-header library using the same technique as stb libraries.
 * There must be exactly one source file (C++) which defines symbol `DB_PERLIN_IMPL` before
 * including this header. You may place such implementation in a dedicated file:
 *
 * ```cpp
 * #define DB_PERLIN_IMPL
 * #include "db_perlin.hpp"
 * ```
 *
 * Compile that file together with the rest of the program, and all other files may then simply
 * include this header without any additional work.
 *
 * To generate noise, simply use the `perlin` function under `db` namespace. There are three
 * overloads accounting for each dimension, so pass 1-3 arguments to generate noise in the
 * corresponding number of dimensions.
 *
 * The implementation has template specializations to work with either floats or doubles,
 * depending on the desired accuracy.
 */

#ifndef DB_PERLIN_HPP
#define DB_PERLIN_HPP

namespace db {
    template<typename T>
    auto perlin(T x) -> T;

    template<typename T>
    auto perlin(T x, T y) -> T;

    template<typename T>
    auto perlin(T x, T y, T z) -> T;
}

#ifdef DB_PERLIN_IMPL

/*
 * The implementation was based on this article:
 * https://flafla2.github.io/2014/08/09/perlinnoise.html
 *
 * A reference implementation in Java by Ken Perlin, the author of the algorithm:
 * https://mrl.cs.nyu.edu/~perlin/noise/
 *
 * Here are some alternative implementations that were used as inspirations:
 * https://github.com/nothings/stb/blob/master/stb_perlin.h
 * https://github.com/stegu/perlin-noise/blob/master/src/noise1234.c
 */

namespace db {
    // Permutation table, the second half is a mirror of the first half.
    static unsigned char const p[512] = {
        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,
        117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,
        165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
        105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
        187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
        3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,
        227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
        221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,
        185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
        81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,
        115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,
        78,66,215,61,156,180,

        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,
        117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,
        165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
        105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
        187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
        3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,
        227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
        221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,
        185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
        81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,
        115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,
        78,66,215,61,156,180,
    };

    template<typename T>
    static inline auto lerp(T a, T b, T t) -> T {
        return a + t * (b - a);
    }

    template<typename T>
    static inline auto floor(T x) -> int {
        int xi = (int)x;
        return (x < xi) ? xi - 1 : xi;
    }

    template<typename T>
    static inline auto fade(T t) -> T {
        return t * t * t * (t * (t * T(6.0) - T(15.0)) + T(10.0));
    }

    template<typename T>
    static inline auto dot_grad(int hash, T xf) -> T {
        // In 1D case, the gradient may be either 1 or -1
        // The distance vector is the input offset (relative to the smallest bound)
        return (hash & 0x1) ? xf : -xf;
    }

    template<typename T>
    static inline auto dot_grad(int hash, T xf, T yf) -> T {
        // In 2D case, the gradient may be any of 8 direction vectors pointing to the
        // edges of a unit-square. The distance vector is the input offset (relative to
        // the smallest bound)
        switch (hash & 0x7) {
            case 0x0: return  xf + yf;
            case 0x1: return  xf;
            case 0x2: return  xf - yf;
            case 0x3: return -yf;
            case 0x4: return -xf - yf;
            case 0x5: return -xf;
            case 0x6: return -xf + yf;
            case 0x7: return  yf;
            default:  return  T(0.0);
        }
    }

    template<typename T>
    static inline auto dot_grad(int hash, T xf, T yf, T zf) -> T {
        // In 3D case, the gradient may be any of 12 direction vectors pointing to the edges
        // of a unit-cube (rounded to 16 with duplications). The distance vector is the input
        // offset (relative to the smallest bound)
        switch (hash & 0xF) {
            case 0x0: return  xf + yf;
            case 0x1: return -xf + yf;
            case 0x2: return  xf - yf;
            case 0x3: return -xf - yf;
            case 0x4: return  xf + zf;
            case 0x5: return -xf + zf;
            case 0x6: return  xf - zf;
            case 0x7: return -xf - zf;
            case 0x8: return  yf + zf;
            case 0x9: return -yf + zf;
            case 0xA: return  yf - zf;
            case 0xB: return -yf - zf;
            case 0xC: return  yf + xf;
            case 0xD: return -yf + zf;
            case 0xE: return  yf - xf;
            case 0xF: return -yf - zf;
            default:  return  T(0.0);
        }
    }

    template<typename T>
    auto perlin(T x) -> T {
        // Left coordinate of the unit-line that contains the input
        int xi = floor(x);

        // Input location in the unit-line
        T xf = x - xi;

        // Wrap to range 0-255
        xi &= 0xFF;

        // Apply the fade function to the location
        T u = fade(xf);

        // Generate hash values for each point of the unit-line
        int h0 = p[xi    ];
        int h1 = p[xi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        return lerp(dot_grad(h0, xf), dot_grad(h1, xf - T(1.0)), u);
    }

    template<typename T>
    auto perlin(T x, T y) -> T {
        // Top-left coordinates of the unit-square
        int xi = floor(x);
        int yi = floor(y);

        // Input location in the unit-square
        T xf = x - xi;
        T yf = y - yi;

        // Wrap to range 0-255
        xi &= 0xFF;
        yi &= 0xFF;

        // Apply the fade function to the location
        T u = fade(xf);
        T v = fade(yf);

        // Generate hash values for each point of the unit-square
        int h00 = p[p[xi    ] + yi    ];
        int h01 = p[p[xi    ] + yi + 1];
        int h10 = p[p[xi + 1] + yi    ];
        int h11 = p[p[xi + 1] + yi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        T x1 = lerp(dot_grad(h00, xf, yf         ), dot_grad(h10, xf - T(1.0), yf         ), u);
        T x2 = lerp(dot_grad(h01, xf, yf - T(1.0)), dot_grad(h11, xf - T(1.0), yf - T(1.0)), u);
        return lerp(x1, x2, v);
    }

    template<typename T>
    auto perlin(T x, T y, T z) -> T {
        // Top-left coordinates of the unit-cube
        int xi = floor(x);
        int yi = floor(y);
        int zi = floor(z);

        // Input location in the unit-cube
        T xf = x - xi;
        T yf = y - yi;
        T zf = z - zi;

        // Wrap to range 0-255
        xi &= 0xFF;
        yi &= 0xFF;
        zi &= 0xFF;

        // Apply the fade function to the location
        T u = fade(xf);
        T v = fade(yf);
        T w = fade(zf);

        // Generate hash values for each point of the unit-cube
        int h000 = p[p[p[xi    ] + yi    ] + zi    ];
        int h001 = p[p[p[xi    ] + yi    ] + zi + 1];
        int h010 = p[p[p[xi    ] + yi + 1] + zi    ];
        int h011 = p[p[p[xi    ] + yi + 1] + zi + 1];
        int h100 = p[p[p[xi + 1] + yi    ] + zi    ];
        int h101 = p[p[p[xi + 1] + yi    ] + zi + 1];
        int h110 = p[p[p[xi + 1] + yi + 1] + zi    ];
        int h111 = p[p[p[xi + 1] + yi + 1] + zi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        T x1, x2, y1, y2;
        x1 = lerp(dot_grad(h000, xf, yf         , zf         ), dot_grad(h100, xf - T(1.0), yf         , zf         ), u);
        x2 = lerp(dot_grad(h010, xf, yf - T(1.0), zf         ), dot_grad(h110, xf - T(1.0), yf - T(1.0), zf         ), u);
        y1 = lerp(x1, x2, v);

        x1 = lerp(dot_grad(h001, xf, yf         , zf - T(1.0)), dot_grad(h101, xf - T(1.0), yf         , zf - T(1.0)), u);
        x2 = lerp(dot_grad(h011, xf, yf - T(1.0), zf - T(1.0)), dot_grad(h111, xf - T(1.0), yf - T(1.0), zf - T(1.0)), u);
        y2 = lerp(x1, x2, v);

        return lerp(y1, y2, w);
    }
}

template auto db::perlin<float>(float x) -> float;
template auto db::perlin<float>(float x, float y) -> float;
template auto db::perlin<float>(float x, float y, float z) -> float;

template auto db::perlin<double>(double x) -> double;
template auto db::perlin<double>(double x, double y) -> double;
template auto db::perlin<double>(double x, double y, double z) -> double;

#endif // DB_PERLIN_IMPL

#endif // DB_PERLIN_HPP

/*
MIT License

Copyright (c) 2020-2021 Daniils Buts

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
