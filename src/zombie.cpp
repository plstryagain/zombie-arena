#include "zombie.hpp"
#include "texture_holder.hpp"
#include <cstdlib>
#include <ctime>

Zombie::Zombie()
{

}

bool Zombie::hit()
{
    --health_;
    if (health_ < 0) {
        is_alive_ = false;
        sprite_.setTexture(TextureHolder::getInstance().GetTexture("asets/graphics/blood.png"));
        return true;
    }
    return false;
}

bool Zombie::isAlive() const
{
    return is_alive_;
}

void Zombie::spawn(float start_x, float start_y, Zombie::TYPE type, int seed)
{
    switch (type) {
    case TYPE::kBloater:
        sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/bloater.png")};
        speed_ = BLOATER_SPEED;
        health_ = BLOATER_HEALTH;
        break;
    case TYPE::kChaser:
        sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/chaser.png")};
        speed_ = CHASER_SPEED;
        health_ = CHASER_HEALTH;
        break;
    case TYPE::kCrawler:
        sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/crawler.png")};
        speed_ = CRAWLER_SPEED;
        health_ = CRAWLER_HEALTH;
        break;
    }
    srand(static_cast<int32_t>(time(0) * seed));
    float modifier = (rand() % MAX_VARIANCE) + OFFSET;
    modifier /= 100;
    speed_ += modifier;

    position_.x = start_x;
    position_.y = start_y;
    sprite_.setOrigin(25, 25);
    sprite_.setPosition(position_);
}

sf::FloatRect Zombie::getPosition() const
{
    return sprite_.getLocalBounds();
}

sf::Sprite Zombie::getSprite() const
{
    return sprite_;
}

void Zombie::update(float elapsed_time, sf::Vector2f player_location)
{
    float player_x = player_location.x;
    float player_y = player_location.y;
    if (player_x > position_.x) {
        position_.x = position_.x + speed_ * elapsed_time;
    }
    if (player_y > position_.y) {
        position_.y = position_.y + speed_ * elapsed_time;
    }
    if (player_x < position_.x) {
        position_.x = position_.x - speed_ * elapsed_time;
    }
    if (player_y < position_.y) {
        position_.y = position_.y - speed_ * elapsed_time;
    }
    sprite_.setPosition(position_);
    float angle = (atan2(player_y - position_.y, player_x - position_.x) * 180) / 3.14;
    sprite_.setRotation(angle);
}