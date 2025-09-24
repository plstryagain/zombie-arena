#include "player.hpp"

inline static constexpr uint32_t SCREEN_WIDTH = 1920;
inline static constexpr uint32_t SCREEN_HEIGHT = 1080;
#ifdef __APPLE__
inline static constexpr uint32_t DPI_SCALE = 2;
#else
inline static constexpr uint32_t DPI_SCALE = 1;
#endif

int main()
{
    enum class STATE { kPaused, kLevelingUp, kGameOver, kPlaying };
    STATE state{STATE::kGameOver};
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode{static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y)}, "Zombie Arena", sf::Style::Fullscreen);
    sf::View view_main{sf::FloatRect{0, 0, resolution.x, resolution.y}};
    sf::Clock clock;
    // in state kPlaying
    sf::Time game_time_total;
    sf::Vector2f mouse_world_position;
    sf::Vector2i mouse_screen_position;
    Player player;
    sf::IntRect arena;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return && state == STATE::kPlaying) {
                    state = STATE::kPaused;
                } else if (event.key.code == sf::Keyboard::Return && state == STATE::kPaused) {
                    state = STATE::kPlaying;
                    clock.restart();
                } else if (event.key.code == sf::Keyboard::Return && state == STATE::kGameOver) {
                    state = STATE::kLevelingUp;
                }
                if (state == STATE::kPlaying) {

                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        if (state == STATE::kPlaying) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                player.moveUp();
            } else {
                player.stopUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                player.moveDown();
            } else {
                player.stopDown();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player.moveLeft();
            } else {
                player.stopLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.moveRight();
            } else {
                player.stopRight();
            }
        }
        if (state == STATE::kLevelingUp) {
            if (event.key.code == sf::Keyboard::Num1) {
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num2) {
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num3) {
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num4) {
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num5) {
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num6) {
                state = STATE::kPlaying;
            }
            if (state == STATE::kPlaying) {
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;
                int32_t tile_size = 50;
                player.spawn(arena, resolution, tile_size);
                clock.restart();
            }
        }

        if (state == STATE::kPlaying) {
            sf::Time dt = clock.restart();
            game_time_total += dt;
            float dt_as_seconds = dt.asSeconds();
            mouse_screen_position = sf::Mouse::getPosition();
            mouse_world_position = window.mapPixelToCoords(sf::Mouse::getPosition(), view_main);
            player.update(dt_as_seconds, sf::Mouse::getPosition());
            sf::Vector2f player_position{player.getCenter()};
            view_main.setCenter(player.getCenter());
        }

        if (state == STATE::kPlaying) {
            window.clear();
            window.setView(view_main);
            window.draw(player.getSprite());
        }
        if (state == STATE::kLevelingUp) {

        }
        if (state == STATE::kPaused) {

        }
        if (state == STATE::kGameOver) {

        }
        window.display();
    }
    return 0;
}