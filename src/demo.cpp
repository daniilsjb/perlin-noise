#include <SFML/Graphics.hpp>
#include "PerlinNoise.h"

int main() {
    const int width = 800;
    const int height = 600;

    sf::RenderWindow window(sf::VideoMode(width, height), "Perlin Noise");

    sf::Image image;
    image.create(width, height);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite output;
    output.setTexture(texture);

    sf::Clock clock;
    while (window.isOpen()) {
        float elapsedTime = clock.getElapsedTime().asSeconds();

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                float noise = (
                    PerlinNoise::Noise((float)x / 64.f, (float)y / 64.f, elapsedTime * 0.25f) * 1.0f +
                    PerlinNoise::Noise((float)x / 32.f, (float)y / 32.f, elapsedTime * 0.75f) * 0.5f
                ) / 1.5f;

                sf::Uint8 brightness = (int)((noise * 0.5f + 0.5f) * 255.0f);
                image.setPixel(x, y, sf::Color(brightness, brightness, brightness));
            }
        }

        window.clear();
        texture.update(image);
        window.draw(output);
        window.display();
    }

    return 0;
}