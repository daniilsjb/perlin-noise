#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

namespace PerlinNoise {
    template<typename T>
    T Noise(T x);

    template<typename T>
    T Noise(T x, T y);

    template<typename T>
    T Noise(T x, T y, T z);
}

#endif
