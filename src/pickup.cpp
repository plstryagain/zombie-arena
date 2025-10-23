#include "pickup.hpp"
#include "texture_holder.hpp"

Pickup::Pickup(Pickup::TYPE type)
    : type_{type}
{
    if (type_ == TYPE::kHealth) {
        sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/health_pickup.png")};
        value_ = HEALTH_START_VALUE;
    } else if (type_ == TYPE::kAmmo) {
        sprite_ = sf::Sprite{TextureHolder::getInstance().GetTexture("assets/graphics/ammo_pickup.png")};
        value_ = AMMO_START_VALUE;
    }
    sprite_.setOrigin(25, 25);
    seconds_to_live_ = START_SECONDS_TO_LIVE;
    seconds_to_wait_ = START_WAIT_TIME;
}

Pickup::~Pickup()
{

}


void Pickup::setArena(sf::IntRect arena)
{
    arena_.left = arena.left + 50;
    arena_.width = arena.width - 50;
    arena_.top = arena.top + 50;
    arena_.height = arena.height - 50;
    spawn();
}

void Pickup::spawn()
{
    srand(static_cast<int32_t>(time(0)) / static_cast<int32_t>(type_));
    int32_t x = rand() % arena_.width;
    srand(static_cast<int32_t>(time(0)) * static_cast<int32_t>(type_));
    int32_t y = rand() % arena_.height;
    seconds_since_spawn_ = 0;
    spawned_ = true;
    sprite_.setPosition(x, y);
}

sf::FloatRect Pickup::getPosition() const
{
    return sprite_.getGlobalBounds();
}

sf::Sprite Pickup::getSprite() const
{
    return sprite_;
}

void Pickup::update(float elapsed_time)
{
    if (spawned_) {
        seconds_since_spawn_ += elapsed_time;
    } else {
        seconds_since_despawn_ += elapsed_time;
    }
    if (seconds_since_spawn_ > seconds_to_live_ && spawned_) {
        spawned_ = false;
        seconds_since_despawn_ = 0;
    }
    if (seconds_since_despawn_ > seconds_to_wait_ && !spawned_) {
        spawn();
    }
}

bool Pickup::isSpawned() const
{
    return spawned_;
}

int32_t Pickup::gotIt()
{
    spawned_ = false;
    seconds_since_despawn_ = 0;
    return value_;
}

void Pickup::upgrade()
{
    switch (type_) {
    case TYPE::kHealth:
        value_ += (HEALTH_START_VALUE * 0.5);
        break;
    case TYPE::kAmmo:
        value_ += (AMMO_START_VALUE * 0.5);
        break;
    }
    seconds_to_live_ += static_cast<float>(START_SECONDS_TO_LIVE) / 10;
    seconds_to_wait_ -= static_cast<float>(START_WAIT_TIME) / 10;
}