/*
 * Simple demonstration of the Perlin Noise algorithm.
 *
 * To set up SFML, refer to the official website:
 * https://www.sfml-dev.org/
 */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define DB_PERLIN_IMPL
#include "../db_perlin.hpp"

auto main() -> int {
    auto const width { 800 };
    auto const height { 600 };

    sf::RenderWindow window {
        sf::VideoMode { width, height }, "Perlin Noise"
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
                    db::perlin(double(x) / 64.0, double(y) / 64.0, dt * 0.25) * 1.0 +
                    db::perlin(double(x) / 32.0, double(y) / 32.0, dt * 0.75) * 0.5
                ) / 1.5;

                auto const brightness = sf::Uint8((noise * 0.5 + 0.5) * 255.0);
                image.setPixel(x, y, sf::Color(brightness, brightness, brightness));
            }
        }

        texture.update(image);

        window.clear();
        window.draw(output);
        window.display();
    }
}
