# Perlin Noise

![Preview Image](preview/preview.png)

## Features

Quick and simple implementation of Perlin Noise algorithm in 1D, 2D, and 3D in C++ with no external dependencies. This
code may easily be reused by simply including the header into any project along with a `DB_PERLIN_IMPL` definition. All
relevant code is written as a small set of pure functions that work with both floats and doubles.

I wrote this with the primary goal of having a bit of fun and learning more about the famous algorithm used everywhere
in procedural generation. Ultimately, my goal was to use this implementation in several other projects (it's always good
to have a noise generator lying around). I hope it could be useful to other people, too!

## Demo

The repository contains a small demonstration written with [SFML](https://www.sfml-dev.org/) that displays the 3D noise
as an animated grayscale with respect to time.
