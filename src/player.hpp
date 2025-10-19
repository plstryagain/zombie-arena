#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

public:
    void spawn(sf::IntRect arena, sf::Vector2f resolution, int32_t tile_size);
    void resetPlayerStats();
    bool hit(sf::Time time_hit);
    sf::Time getLastHitTime() const;
    sf::FloatRect getPosition() const;
    sf::Vector2f getCenter() const;
    float getRotation() const;
    sf::Sprite getSprite() const;
    int32_t getHealth() const;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopLeft();
    void stopRight();
    void stopUp();
    void stopDown();
    void update(float elapsed_time, sf::Vector2i mouse_position);
    void upgradeSpeed();
    void upgradeHealth();
    void increaseHealthLevel(int32_t amount);
    sf::RectangleShape frame();

private:
    const float START_SPEED = 200;
    const int32_t START_HEALTH = 100;
    sf::Vector2f position_;
    sf::Sprite sprite_;
    sf::Vector2f resolution_;
    sf::IntRect arena_;
    int32_t tile_size_;
    bool up_pressed_;
    bool down_pressed_;
    bool left_pressed_;
    bool right_pressed_;
    int32_t health_;
    int32_t max_health_;
    sf::Time last_hit_;
    float speed_;
};