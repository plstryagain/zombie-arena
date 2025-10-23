#include "player.hpp"
#include "zombie_arena.hpp"
#include "texture_holder.hpp"
#include "bullet.hpp"
#include "pickup.hpp"

#include <array>
#include <iostream>
#include <sstream>
#include <fstream>

#include <SFML/Audio.hpp>

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
    bool is_draw_frame = false;
    STATE state{STATE::kGameOver};
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode{static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y)}, "Zombie Arena", sf::Style::Fullscreen);
    // sf::RenderWindow window(sf::VideoMode{static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y)}, "Zombie Arena", sf::Style::Default);
    sf::View view_main{sf::FloatRect{0, 0, resolution.x, resolution.y}};
    sf::Clock clock;
    // in state kPlaying
    sf::Time game_time_total;
    sf::Vector2f mouse_world_position;
    sf::Vector2i mouse_screen_position;
    Player player;
    sf::IntRect arena;
    sf::VertexArray background;
    sf::Texture texture_background = TextureHolder::getInstance().GetTexture("assets/graphics/background_sheet.png");
    int32_t num_zombies = 0;
    int32_t num_zombies_alive = 0;
    std::vector<Zombie> zombies;
    std::array<Bullet, 100> bullets;
    int32_t current_bullet = 0;
    int32_t bullets_spare = 24;
    int32_t bullets_in_clip = 6;
    int32_t clip_size = 6;
    float fire_rate = 1;
    sf::Time last_pressed;
    window.setMouseCursorVisible(false);
    sf::Sprite sprite_crosshair;
    sf::Texture crosshair_texture = TextureHolder::getInstance().GetTexture("assets/graphics/crosshair.png");
    sprite_crosshair.setTexture(crosshair_texture);
    sprite_crosshair.setOrigin(25, 25);

    Pickup health_pickup{Pickup::TYPE::kHealth};
    Pickup ammo_pickup{Pickup::TYPE::kAmmo};
    int32_t score = 0;
    int32_t hiscore = 0;

    sf::Sprite sprite_game_over;
    sf::Texture texture_game_over = TextureHolder::getInstance().GetTexture("assets/graphics/background.png");
    sprite_game_over.setTexture(texture_game_over);
    sprite_game_over.setPosition(0, 0);
    sf::View hud_view{sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)};
    sf::Sprite sprite_ammo_icon;
    sf::Texture texture_ammo_icon = TextureHolder::getInstance().GetTexture("assets/graphics/ammo_icon.png");
    sprite_ammo_icon.setTexture(texture_ammo_icon);
    sprite_ammo_icon.setPosition(20, 980);

    sf::Font font;
    font.loadFromFile("assets/fonts/zombiecontrol.ttf");
    
    sf::Text paused_text;
    paused_text.setFont(font);
    paused_text.setCharacterSize(155);
    paused_text.setFillColor(sf::Color::White);
    paused_text.setPosition(400, 400);
    paused_text.setString("Press Enter \nto continue");

    sf::Text game_over_text;
    game_over_text.setFont(font);
    game_over_text.setCharacterSize(125);
    game_over_text.setFillColor(sf::Color::White);
    game_over_text.setPosition(250, 250);
    game_over_text.setString("Press Enter to play");

    sf::Text level_up_text;
    level_up_text.setFont(font);
    level_up_text.setCharacterSize(80);
    level_up_text.setFillColor(sf::Color::White);
    level_up_text.setPosition(150, 250);
    std::stringstream level_up_string;
    // level_up_string <<
    //     "1 - Повышение скорострельности" <<
    //     "\n2 - Повышение размера обоймы (при следующей перезарядки)" <<
    //     "\n3 - Повышение максимального уровня здоровья" <<
    //     "\n4 - Повышение скорости передвижения" <<
    //     "\n5 - Улучшение аптечки" <<
    //     "\n6 - Улучшение юоеприпсов";
    level_up_string <<
        "1 - Increase shooting rate" <<
        "\n2 - Increase magazine capacity (next reload)" <<
        "\n3 - Increase maximum health" <<
        "\n4 - Increase move speed" <<
        "\n5 - Update health pickup" <<
        "\n6 - Update ammo pickup";
    level_up_text.setString(level_up_string.str());

    sf::Text ammo_text;
    ammo_text.setFont(font);
    ammo_text.setCharacterSize(55);
    ammo_text.setFillColor(sf::Color::White);
    ammo_text.setPosition(200, 980);

    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(55);
    score_text.setFillColor(sf::Color::White);
    score_text.setPosition(20, 0);
    
    std::ifstream input_file{"gamedata/scores.txt"};
    if (input_file.is_open()) {
        input_file >> hiscore;
        input_file.close();
    }
    sf::Text hiscore_text;
    hiscore_text.setFont(font);
    hiscore_text.setCharacterSize(55);
    hiscore_text.setFillColor(sf::Color::White);
    hiscore_text.setPosition(1400, 0);
    std::stringstream hiscore_string;
    hiscore_string << "Hi score: " << hiscore;
    hiscore_text.setString(hiscore_string.str());

    sf::Text zombies_remaining_text;
    zombies_remaining_text.setFont(font);
    zombies_remaining_text.setCharacterSize(55);
    zombies_remaining_text.setFillColor(sf::Color::White);
    zombies_remaining_text.setPosition(1500, 980);
    zombies_remaining_text.setString("Zombies: 100");

    int32_t wave = 0;
    sf::Text wave_number_text;
    wave_number_text.setFont(font);
    wave_number_text.setCharacterSize(55);
    wave_number_text.setFillColor(sf::Color::White);
    wave_number_text.setPosition(1250, 980);
    wave_number_text.setString("Wave: 0");

    sf::RectangleShape health_bar;
    health_bar.setFillColor(sf::Color::Red);
    health_bar.setPosition(450, 980);

    int32_t frames_since_last_hud_update = 0;
    int32_t fps_measurement_frame_interval = 1000;

    sf::SoundBuffer hit_buffer;
    hit_buffer.loadFromFile("assets/sound/hit.wav");
    sf::Sound hit{hit_buffer};

    sf::SoundBuffer splat_buffer;
    splat_buffer.loadFromFile("assets/sound/splat.wav");
    sf::Sound splat{splat_buffer};

    sf::SoundBuffer shoot_buffer;
    shoot_buffer.loadFromFile("assets/sound/shoot.wav");
    sf::Sound shoot{shoot_buffer};

    sf::SoundBuffer reload_buffer;
    reload_buffer.loadFromFile("assets/sound/reload.wav");
    sf::Sound reload{reload_buffer};

    sf::SoundBuffer reload_failed_buffer;
    reload_failed_buffer.loadFromFile("assets/sound/reload_failed.wav");
    sf::Sound reload_failed{reload_failed_buffer};

    sf::SoundBuffer powerup_buffer;
    powerup_buffer.loadFromFile("assets/sound/powerup.wav");
    sf::Sound powerup{powerup_buffer};

    sf::SoundBuffer pickup_buffer;
    pickup_buffer.loadFromFile("assets/sound/pickup.wav");
    sf::Sound pickup{pickup_buffer};

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
                    wave = 0;
                    score = 0;
                    current_bullet = 0;
                    bullets_spare = 24;
                    bullets_in_clip = 6;
                    clip_size = 6;
                    fire_rate = 1;
                    player.resetPlayerStats();
                }
                if (state == STATE::kPlaying) {
                    if (event.key.code == sf::Keyboard::R) {
                        if (bullets_spare >= clip_size) {
                            bullets_in_clip = clip_size;
                            bullets_spare -= clip_size;
                            reload.play();
                        } else if (bullets_spare > 0) {
                            bullets_in_clip = bullets_spare;
                            bullets_spare = 0;
                            reload.play();
                        } else {
                            reload_failed.play();
                        }
                    }
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (game_time_total.asMilliseconds() - last_pressed.asMilliseconds() > (1000 / fire_rate) &&
                    bullets_in_clip > 0) {
                    bullets[current_bullet].shoot(player.getCenter().x, player.getCenter().y, mouse_world_position.x, mouse_world_position.y);
                    ++current_bullet;
                    if (current_bullet > 99) {
                        current_bullet = 0;
                    }
                    last_pressed = game_time_total;
                    shoot.play();
                    --bullets_in_clip;
                }
            }
        }
        if (state == STATE::kLevelingUp) {
            if (event.key.code == sf::Keyboard::Num1) {
                ++fire_rate;
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num2) {
                clip_size += clip_size;
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num3) {
                player.upgradeHealth();
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num4) {
                player.upgradeSpeed();
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num5) {
                health_pickup.upgrade();
                state = STATE::kPlaying;
            }
            if (event.key.code == sf::Keyboard::Num6) {
                ammo_pickup.upgrade();
                state = STATE::kPlaying;
            }
            if (state == STATE::kPlaying) {
                ++wave;
                arena.width = 500 * wave;
                arena.height = 500 * wave;
                arena.left = 0;
                arena.top = 0;
                int32_t tile_size = create_background(background, arena);
                player.spawn(arena, resolution, tile_size);
                health_pickup.setArena(arena);
                ammo_pickup.setArena(arena);
                num_zombies = 5 * wave;
                zombies = create_horde(num_zombies, arena);
                num_zombies_alive = num_zombies;
                powerup.play();
                clock.restart();
            }
        }

        if (state == STATE::kPlaying) {
            sf::Time dt = clock.restart();
            game_time_total += dt;
            float dt_as_seconds = dt.asSeconds();
            mouse_screen_position = sf::Mouse::getPosition();
            mouse_world_position = window.mapPixelToCoords(sf::Mouse::getPosition(), view_main);
            sprite_crosshair.setPosition(mouse_world_position);
            player.update(dt_as_seconds, sf::Mouse::getPosition());
            sf::Vector2f player_position{player.getCenter()};
            view_main.setCenter(player.getCenter());
            for (int32_t i = 0; i < num_zombies; ++i) {
                if (zombies[i].isAlive()) {
                    zombies[i].update(dt_as_seconds, player_position);
                }
            }
            for (int32_t i = 0; i < 100; ++i) {
                if (bullets[i].isInFlight()) {
                    bullets[i].update(dt_as_seconds);
                }
            }
            health_pickup.update(dt_as_seconds);
            ammo_pickup.update(dt_as_seconds);
            int32_t bullets_num = bullets.size();
            for (int32_t i = 0; i < bullets_num; ++i) {
                for (int32_t j = 0; j < num_zombies; ++j) {
                    if (bullets[i].isInFlight() && zombies[j].isAlive()) {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
                            bullets[i].stop();
                            if (zombies[j].hit()) {
                                score += 10;
                                if (score >= hiscore) {
                                    hiscore = score;
                                }
                                --num_zombies_alive;
                                if (num_zombies_alive == 0) {
                                    state = STATE::kLevelingUp;
                                }
                            }
                            splat.play();
                        }
                    }
                }
            }
            for (int32_t i = 0; i < num_zombies; ++i) {
                if (player.getPosition().intersects(zombies[i].getPosition()) &&
                    zombies[i].isAlive()) {
                    if (player.hit(game_time_total)) {
                        hit.play();
                    }
                    if (player.getHealth() <= 0) {
                        state = STATE::kGameOver;
                        std::ofstream output_file{"gamedata/scores.txt"};
                        output_file << hiscore;
                        output_file.close();
                    }
                }
            }
            if (player.getPosition().intersects(health_pickup.getPosition()) &&
                health_pickup.isSpawned()) {
                player.increaseHealthLevel(health_pickup.gotIt());
                pickup.play();
            }
            if (player.getPosition().intersects(ammo_pickup.getPosition()) &&
                ammo_pickup.isSpawned()) {
                bullets_spare += ammo_pickup.gotIt();
                pickup.play();
            }
            health_bar.setSize(sf::Vector2f{static_cast<float>(player.getHealth() * 3), 50});
            ++frames_since_last_hud_update;
            if (frames_since_last_hud_update > fps_measurement_frame_interval) {
                std::stringstream ss_ammo;
                std::stringstream ss_score;
                std::stringstream ss_hiscore;
                std::stringstream ss_wave;
                std::stringstream ss_zombies_alive;
                ss_ammo << bullets_in_clip << "/" << bullets_spare;
                ammo_text.setString(ss_ammo.str());
                ss_score << "Score: " << score;
                score_text.setString(ss_score.str());
                ss_hiscore << "Hi score: " << hiscore;
                hiscore_text.setString(ss_hiscore.str());
                ss_wave << "Wave: " << wave;
                wave_number_text.setString(ss_wave.str());
                ss_zombies_alive << "Zombies: " << num_zombies_alive;
                zombies_remaining_text.setString(ss_zombies_alive.str());
                frames_since_last_hud_update = 0;
            }
        }

        if (state == STATE::kPlaying) {
            window.clear();
            window.setView(view_main);
            window.draw(background, &texture_background);
            for (int32_t i = 0; i < num_zombies; ++i) {
                window.draw(zombies[i].getSprite());
                if (is_draw_frame) {
                    window.draw(zombies[i].frame());
                }
            }
            for (int32_t i = 0; i < 100; ++i) {
                if (bullets[i].isInFlight()) {
                    window.draw(bullets[i].getShape());
                }
            }
            window.draw(player.getSprite());
            if (is_draw_frame) {
                window.draw(player.frame());
            }
            if (ammo_pickup.isSpawned()) {
                window.draw(ammo_pickup.getSprite());
            }
            if (health_pickup.isSpawned()) {
                window.draw(health_pickup.getSprite());
            }
            window.draw(sprite_crosshair);
            window.setView(hud_view);
            window.draw(sprite_ammo_icon);
            window.draw(ammo_text);
            window.draw(score_text);
            window.draw(hiscore_text);
            window.draw(health_bar);
            window.draw(wave_number_text);
            window.draw(zombies_remaining_text);
        }
        if (state == STATE::kLevelingUp) {
            window.draw(sprite_game_over);
            window.draw(level_up_text);
        }
        if (state == STATE::kPaused) {
            window.draw(paused_text);
        }
        if (state == STATE::kGameOver) {
            window.draw(sprite_game_over);
            window.draw(game_over_text);
            window.draw(score_text);
            window.draw(hiscore_text);
        }
        window.display();
    }
    return 0;
}