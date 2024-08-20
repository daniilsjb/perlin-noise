/*
 * Demonstration of the Perlin noise algorithm.
 *
 * This program generates a grayscale bitmap file in the BMP format as a
 * simple visualization of the Perlin noise in 2D. For a more sophisticated
 * and dynamic demonstration, refer to the WASM example.
 */

#include <fstream>
#include <cstdint>

#define DB_PERLIN_IMPL
#include "../../db_perlin.hpp"

struct BmpFileHeader {
    uint8_t file_type[2] = { 0x42, 0x4D }; // ASCII signature 'BM' (little-endian)
    uint8_t file_size[4] = { 0 };          // The size of the file, in bytes (filled later).
    uint8_t reserved[4]  = { 0 };          // Reserved, always equal to 0.
    uint8_t offset[4]    = { 0 };          // Offset to the start of pixel data (filled later).
};

struct BmpInfoHeader {
    uint8_t size[4]             = { 0 };  // The size of this header, in bytes.
    uint8_t width[4]            = { 0 };  // The width of the bitmap, in pixels.
    uint8_t height[4]           = { 0 };  // The height of the bitmap, in pixels.
    uint8_t planes[2]           = { 1 };  // The number of color planes, always equal to 1.
    uint8_t bit_count[2]        = { 24 }; // The number of bits per pixel.
    uint8_t compression[4]      = { 0 };  // The compression method (0 for no compression).
    uint8_t image_size[4]       = { 0 };  // The size of the image (0 if no compression).
    uint8_t x_pixels_per_m[4]   = { 0 };  // Horizontal resolution.
    uint8_t y_pixels_per_m[4]   = { 0 };  // Vertical resolution.
    uint8_t colors_palette[4]   = { 0 };  // The number of indices in the color table (0 for maximum allowed).
    uint8_t colors_important[4] = { 0 };  // The number of colors used for displaying the bitmap (0 if all required).
};

auto main() -> int {
    auto const width { 600 };
    auto const height { 300 };

    std::ofstream fstream { "output.bmp", std::ios::binary };

    auto bmp_file_header = BmpFileHeader {};
    auto bmp_info_header = BmpInfoHeader {};

    bmp_info_header.size[0] = sizeof(bmp_info_header);
    bmp_file_header.offset[0] = sizeof(bmp_file_header) + sizeof(bmp_info_header);

    auto const file_size = bmp_file_header.offset[0] + width * height * 3;
    bmp_file_header.file_size[0] = static_cast<uint8_t>(file_size >> 0);
    bmp_file_header.file_size[1] = static_cast<uint8_t>(file_size >> 8);
    bmp_file_header.file_size[2] = static_cast<uint8_t>(file_size >> 16);
    bmp_file_header.file_size[3] = static_cast<uint8_t>(file_size >> 24);

    bmp_info_header.width[0] = static_cast<uint8_t>(width >> 0);
    bmp_info_header.width[1] = static_cast<uint8_t>(width >> 8);
    bmp_info_header.width[2] = static_cast<uint8_t>(width >> 16);
    bmp_info_header.width[3] = static_cast<uint8_t>(width >> 24);

    bmp_info_header.height[0] = static_cast<uint8_t>(height >> 0);
    bmp_info_header.height[1] = static_cast<uint8_t>(height >> 8);
    bmp_info_header.height[2] = static_cast<uint8_t>(height >> 16);
    bmp_info_header.height[3] = static_cast<uint8_t>(height >> 24);

    fstream.write(reinterpret_cast<char const *>(&bmp_file_header), sizeof(bmp_file_header));
    fstream.write(reinterpret_cast<char const *>(&bmp_info_header), sizeof(bmp_info_header));

    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            auto const noise = (
                db::perlin(double(x) / 64.0, double(y) / 64.0) * 1.00 +
                db::perlin(double(x) / 32.0, double(y) / 32.0) * 0.50 +
                db::perlin(double(x) / 16.0, double(y) / 16.0) * 0.25
            ) / 1.75;

            uint8_t const brightness = uint8_t((noise * 0.5 + 0.5) * 255.0);
            uint8_t const channels[3] = { brightness, brightness, brightness };

            fstream.write(reinterpret_cast<char const *>(channels), 3);
        }
    }
}
