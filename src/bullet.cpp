#include "bullet.hpp"

Bullet::Bullet()
{
    bullet_shape_.setSize(sf::Vector2f{2, 2});
}

Bullet::~Bullet()
{

}

void Bullet::stop()
{
    is_in_flight_ = false;
}

bool Bullet::isInFlight() const
{
    return is_in_flight_;
}

void Bullet::shoot(float start_x, float start_y, float target_x, float target_y)
{
    is_in_flight_ = true;
    position_.x = start_x;
    position_.y = start_y;
    float gradient = (start_x - target_x) / (start_y - target_y);
    if (gradient < 0) {
        gradient *= -1;
    }
    float ratio_xy = bullet_speed_ / (1 + gradient);
    bullet_distance_y_ = ratio_xy;
    bullet_distance_x_ = ratio_xy * gradient;
    if (target_x < start_x) {
        bullet_distance_x_ *= -1;
    }
    if (target_y < start_y) {
        bullet_distance_y_ *= -1;
    }
    float range = 1000;
    min_x_ = start_x - range;
    max_x_ = start_x + range;
    min_y_ = start_y - range;
    max_y_ = start_y + range;

    bullet_shape_.setPosition(position_);
}

sf::FloatRect Bullet::getPosition() const
{
    return bullet_shape_.getGlobalBounds();
}

sf::RectangleShape Bullet::getShape() const
{
    return bullet_shape_;
}

void Bullet::update(float elapsed_time)
{
    position_.x += bullet_distance_x_ * elapsed_time;
    position_.y += bullet_distance_y_ * elapsed_time;
    bullet_shape_.setPosition(position_);
    if (position_.x < min_x_ || position_.x > max_x_ ||
        position_.y < min_y_ || position_.y > max_y_) {
        is_in_flight_ = false;
    }
}