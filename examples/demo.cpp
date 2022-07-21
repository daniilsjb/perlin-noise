/*
 * Simple demonstration of the Perlin Noise algorithm.
 *
 * To set up SFML, refer to the official website:
 * https://www.sfml-dev.org/
 */

#include <SFML/Graphics.hpp>

#define DB_PERLIN_IMPL
#include "../db_perlin.hpp"

int main() {
    int const width { 800 };
    int const height { 600 };

    sf::RenderWindow window {
        sf::VideoMode { width, height },"Perlin Noise"
    };

    window.setFramerateLimit(60);

    sf::Image image;
    image.create(width, height);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite output;
    output.setTexture(texture);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event ev {};
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto const dt = clock.getElapsedTime().asSeconds();
        for (auto y = 0u; y < height; ++y) {
            for (auto x = 0u; x < width; ++x) {
                auto const noise = (
                    db::perlin(float(x) / 64.0f, float(y) / 64.0f, dt * 0.25f) * 1.0f +
                    db::perlin(float(x) / 32.0f, float(y) / 32.0f, dt * 0.75f) * 0.5f
                ) / 1.5f;

                auto const brightness = sf::Uint8((noise * 0.5f + 0.5f) * 255.0f);
                image.setPixel(x, y, sf::Color(brightness, brightness, brightness));
            }
        }

        texture.update(image);

        window.clear();
        window.draw(output);
        window.display();
    }
}
