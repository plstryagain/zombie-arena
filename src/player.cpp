#include "player.hpp"
#include "texture_holder.hpp"
#include <cmath>

Player::Player()
    : speed_{START_SPEED},
        health_{START_HEALTH},
        max_health_{START_HEALTH},
        sprite_{}
{
    sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/player.png")};
    sprite_.setOrigin(25, 25);
}

void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int32_t tile_size)
{
    position_.x = arena.width / 2.0f;
    position_.y = arena.height / 2.0f;
    arena_.left = arena.left;
    arena_.width = arena.width;
    arena_.top = arena.top;
    arena_.height = arena.height;
    tile_size_ = tile_size;
    resolution_.x = resolution.x;
    resolution_.y = resolution.y;
}

void Player::resetPlayerStats()
{
    speed_ = START_SPEED;
    health_ = START_HEALTH;
    max_health_ = START_HEALTH;
}

bool Player::hit(sf::Time time_hit)
{
    if (time_hit.asMicroseconds() - last_hit_.asMilliseconds() > 200) {
        last_hit_ = time_hit;
        health_ -= 10;
        return true;
    } else {
        return false;
    }
}

sf::Time Player::getLastHitTime() const
{
    return last_hit_;
}

sf::FloatRect Player::getPosition() const
{
    return sprite_.getGlobalBounds();
}

sf::Vector2f Player::getCenter() const
{
    return position_;
}

float Player::getRotation() const
{
    return sprite_.getRotation();
}

sf::Sprite Player::getSprite() const
{
    return sprite_;
}

int32_t Player::getHealth() const
{
    return health_;
}

void Player::moveLeft()
{
    left_pressed_ = true;
}

void Player::moveRight()
{
    right_pressed_ = true;
}

void Player::moveUp()
{
    up_pressed_ = true;
}

void Player::moveDown()
{
    down_pressed_ = true;
}

void Player::stopLeft()
{
    left_pressed_ = false;
}

void Player::stopRight()
{
    right_pressed_ = false;
}

void Player::stopUp()
{
    up_pressed_ = false;
}

void Player::stopDown()
{
    down_pressed_ = false;
}

void Player::update(float elapsed_time, sf::Vector2i mouse_position)
{
    if (up_pressed_) {
        position_.y -= speed_ * elapsed_time;
    }
    if (down_pressed_) {
        position_.y += speed_ * elapsed_time;
    }
    if (right_pressed_) {
        position_.x += speed_ * elapsed_time;
    }
    if (left_pressed_) {
        position_.x -= speed_ * elapsed_time;
    }
    sprite_.setPosition(position_);
    if (position_.x > arena_.width - tile_size_) {
        position_.x = arena_.width - tile_size_;
    }
    if (position_.x < arena_.left + tile_size_) {
        position_.x = arena_.left + tile_size_;
    }
    if (position_.y > arena_.height - tile_size_) {
        position_.y = arena_.height - tile_size_;
    }
    if (position_.y < arena_.top + tile_size_) {
        position_.y = arena_.top + tile_size_;
    }
    float angle = (atan2(mouse_position.y - resolution_.y / 2, mouse_position.x - resolution_.x / 2) * 180) / 3.141;
    sprite_.setRotation(angle);
}

void Player::upgradeSpeed()
{
    speed_ += (START_SPEED * 0.2);
}

void Player::upgradeHealth()
{
    max_health_ += (START_HEALTH * 0.2);
}

void Player::increaseHealthLevel(int32_t amount)
{
    health_ += amount;
    if (health_ > max_health_) {
        health_ = max_health_;
    }
}