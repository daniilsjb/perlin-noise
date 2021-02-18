#include "PerlinNoise.h"

// The following is an implementation of Ken Perlin's "Improved Noise" in 1D, 2D, and 3D.
// All of the code may easily be reused, since it relies on no external dependencies.
// The implementation was based on this article:
// https://flafla2.github.io/2014/08/09/perlinnoise.html

static int p[512] = {
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
    78,66,215,61,156,180, 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,
    225,140,36,103,30,69,142, 8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,
    197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,
    171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,
    211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,
    209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
    3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,
    227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
    221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,
    185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
    81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,
    115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,
    78,66,215,61,156,180,
};

template<typename T>
static inline T Lerp(T a, T b, T t) {
    return a + t * (b - a);
}

template<typename T>
static inline T Fade(T t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

template<typename T>
static inline T DotGrad(int hash, T xf) {
    // In 1D case, the gradient may be either 1 or -1
    // The distance vector is the input offset (relative to the smallest bound)
    return (hash & 0x1) ? xf : -xf;
}

template<typename T>
static inline T DotGrad(int hash, T xf, T yf) {
    // In 2D case, the gradient may be any of 8 direction vectors pointing to the edges of a unit-square
    // The distance vector is the input offset (relative to the smallest bound)
    switch (hash & 0x7) {
        case 0x0: return  xf + yf;
        case 0x1: return  xf;
        case 0x2: return  xf - yf;
        case 0x3: return -yf;
        case 0x4: return -xf - yf;
        case 0x5: return -xf;
        case 0x6: return -xf + yf;
        case 0x7: return  yf;
        default:  return  T(0);
    }
}

template<typename T>
static inline T DotGrad(int hash, T xf, T yf, T zf) {
    // In 3D case, the gradient may be any of 12 direction vectors pointing to the edges of a unit-cube (rounded to 16 with duplications)
    // The distance vector is the input offset (relative to the smallest bound)
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
        default:  return  T(0);
    }
};

template<typename T>
static inline int Floor(T x) {
    return x > T(0) ? (int)x : (int)x - 1;
}

namespace PerlinNoise {

    template<typename T>
    T Noise(T x) {
        // Left coordinate of the unit-line that contains the input
        int xi = Floor(x);

        // Input location in the unit-line
        T xf = x - xi;

        // Wrap to range 0-255
        xi &= 0xFF;

        // Apply the fade function to the location
        T u = Fade(xf);

        // Generate hash values for each point of the unit-line
        int h0, h1;
        h0 = p[xi    ];
        h1 = p[xi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        return Lerp(DotGrad(h0, xf), DotGrad(h1, xf - T(1)), u);
    }

    template<typename T>
    T Noise(T x, T y) {
        // Top-left coordinates of the unit-square
        int xi = Floor(x);
        int yi = Floor(y);

        // Input location in the unit-square
        T xf = x - xi;
        T yf = y - yi;

        // Wrap to range 0-255
        xi &= 0xFF;
        yi &= 0xFF;

        // Apply the fade function to the location
        T u = Fade(xf);
        T v = Fade(yf);

        // Generate hash values for each point of the unit-square
        int h00, h01, h10, h11;
        h00 = p[p[xi    ] + yi    ];
        h01 = p[p[xi    ] + yi + 1];
        h10 = p[p[xi + 1] + yi    ];
        h11 = p[p[xi + 1] + yi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        T x1 = Lerp(DotGrad(h00, xf, yf       ), DotGrad(h10, xf - T(1), yf       ), u);
        T x2 = Lerp(DotGrad(h01, xf, yf - T(1)), DotGrad(h11, xf - T(1), yf - T(1)), u);
        return Lerp(x1, x2, v);
    }

    template<typename T>
    T Noise(T x, T y, T z) {
        // Top-left coordinates of the unit-cube
        int xi = Floor(x);
        int yi = Floor(y);
        int zi = Floor(z);

        // Input location in the unit-cube
        T xf = x - xi;
        T yf = y - yi;
        T zf = z - zi;

        // Wrap to range 0-255
        xi &= 0xFF;
        yi &= 0xFF;
        zi &= 0xFF;

        // Apply the fade function to the location
        T u = Fade(xf);
        T v = Fade(yf);
        T w = Fade(zf);

        // Generate hash values for each point of the unit-cube
        int h000, h001, h010, h011, h100, h101, h110, h111;
        h000 = p[p[p[xi    ] + yi    ] + zi    ];
        h001 = p[p[p[xi    ] + yi    ] + zi + 1];
        h010 = p[p[p[xi    ] + yi + 1] + zi    ];
        h011 = p[p[p[xi    ] + yi + 1] + zi + 1];
        h100 = p[p[p[xi + 1] + yi    ] + zi    ];
        h101 = p[p[p[xi + 1] + yi    ] + zi + 1];
        h110 = p[p[p[xi + 1] + yi + 1] + zi    ];
        h111 = p[p[p[xi + 1] + yi + 1] + zi + 1];

        // Linearly interpolate between dot products of each gradient with its distance to the input location
        T x1, x2, y1, y2;
        x1 = Lerp(DotGrad(h000, xf, yf       , zf), DotGrad(h100, xf - T(1), yf       , zf), u);
        x2 = Lerp(DotGrad(h010, xf, yf - T(1), zf), DotGrad(h110, xf - T(1), yf - T(1), zf), u);
        y1 = Lerp(x1, x2, v);

        x1 = Lerp(DotGrad(h001, xf, yf       , zf - T(1)), DotGrad(h101, xf - T(1), yf       , zf - T(1)), u);
        x2 = Lerp(DotGrad(h011, xf, yf - T(1), zf - T(1)), DotGrad(h111, xf - T(1), yf - T(1), zf - T(1)), u);
        y2 = Lerp(x1, x2, v);

        return Lerp(y1, y2, w);
    }
}

template float PerlinNoise::Noise<float>(float x);
template float PerlinNoise::Noise<float>(float x, float y);
template float PerlinNoise::Noise<float>(float x, float y, float z);

template double PerlinNoise::Noise<double>(double x);
template double PerlinNoise::Noise<double>(double x, double y);
template double PerlinNoise::Noise<double>(double x, double y, double z);
