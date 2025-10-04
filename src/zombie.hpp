#pragma once

#include <SFML/Graphics.hpp>

class Zombie
{
public:
    Zombie();

public:
    enum class TYPE
    {
        kBloater,
        kChaser,
        kCrawler
    };

public:
    bool hit();
    bool isAlive() const;
    void spawn(float start_x, float start_y, Zombie::TYPE type, int seed);
    sf::FloatRect getPosition() const;
    sf::Sprite getSprite() const;
    void update(float elapsed_time, sf::Vector2f player_location);

private:
    static inline constexpr float BLOATER_SPEED = 40;
    static inline constexpr float CHASER_SPEED = 80;
    static inline constexpr float CRAWLER_SPEED = 20;

    static inline constexpr float BLOATER_HEALTH = 5;
    static inline constexpr float CHASER_HEALTH = 1;
    static inline constexpr float CRAWLER_HEALTH = 3;

    static inline constexpr int32_t MAX_VARIANCE = 30;
    static inline constexpr int32_t OFFSET = 101 - MAX_VARIANCE;

    sf::Vector2f position_;
    sf::Sprite sprite_;
    float speed_;
    float health_;
    bool is_alive_;
};