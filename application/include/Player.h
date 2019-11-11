#ifndef _Player_H_
#define _Player_H_

#include <SFML/System.hpp>


class Player {

public:
    Player();
    
    void kill();
    void spawn(sf::Vector2f);
    void applyPassiveForce(float);
    void applyActiveForce(sf::Vector2f, float);
    void tickUpdate(float);
    void handlePowerUp(int);
    void applyStaminaChange(bool moving, bool resting);

    // getters and setters
    enum PlayerState {normal, raising_tusks, attacking, resting, dead};
    PlayerState getState();
    sf::Vector2f getPos();
    sf::Vector2f getVel();
    float getMass();
    float getStamina();
    void setVel(sf::Vector2f);
    void setMass(float);
    void setStamina(float);
    bool isDead();

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    PlayerState state;
    float mass;
    float stamina;
    constexpr static const float decelerate_strength = 20.0f;
    constexpr static const float accelerate_strength = 500.0f;
    float speed;
};

#endif