#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet();
    ~Bullet();

public:
    void stop();
    bool isInFlight() const;
    void shoot(float start_x, float start_y, float target_x, float target_y);
    sf::FloatRect getPosition() const;
    sf::RectangleShape getShape() const;
    void update(float elapsed_time);

private:
    sf::Vector2f position_;
    sf::RectangleShape bullet_shape_;
    bool is_in_flight_ = false;
    float bullet_speed_ = 1000;
    float bullet_distance_x_;
    float bullet_distance_y_;
    float max_x_;
    float min_x_;
    float max_y_;
    float min_y_;
};