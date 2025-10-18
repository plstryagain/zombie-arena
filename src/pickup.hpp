#pragma once

#include <SFML/Graphics.hpp>

class Pickup
{
public:
    enum class TYPE { kHealth = 1, kAmmo = 2 };
public:
    Pickup(Pickup::TYPE type);
    ~Pickup();

public:
    void setArena(sf::IntRect arena);
    void spawn();
    sf::FloatRect getPosition() const;
    sf::Sprite getSprite() const;
    void update(float elapsed_time);
    bool isSpawned() const;
    int32_t gotIt();
    void upgrade();

private:
    static inline constexpr int32_t HEALTH_START_VALUE = 50;
    static inline constexpr int32_t AMMO_START_VALUE = 12;
    static inline constexpr int32_t START_WAIT_TIME = 10;
    static inline constexpr int32_t START_SECONDS_TO_LIVE = 5;
    
    sf::Sprite sprite_;
    sf::IntRect arena_;
    int32_t value_;
    TYPE type_;
    bool spawned_;
    float seconds_since_spawn_;
    float seconds_since_despawn_;
    float seconds_to_live_;
    float seconds_to_wait_;
};