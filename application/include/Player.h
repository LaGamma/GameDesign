#ifndef _Player_H_
#define _Player_H_
#include <SFML/System.hpp>
#include <SFML/Graphics/Color.hpp>


class Player {

public:
    Player();
    
    void kill();
    void slash();
    void raiseTusks(float);
    void spawn(sf::Vector2f);
    void applyPassiveForce(float);
    void applyActiveForce(sf::Vector2f, float);
    void tickUpdate(float);
    void handlePowerUp(int);

    // getters and setters
    enum PlayerState {idle, normal, running, raising_tusks, attacking, resting, dead};
    PlayerState getState();
    sf::Vector2f getPos();
    sf::Vector2f getVel();
    sf::Vector2f getFacingDir();
    sf::Color getColor();
    std::string getName();
    float getAttackCharge();
    float getMass();
    float getStamina();
    void setVel(sf::Vector2f);
    void setStamina(float);
    void setColor(sf::Color);
    void setName(std::string);
    bool isDead();
    void resetStats();

    //stats: kills, deaths, powerups collected, slash attacks used, meters travelled (need to figure out a way to do this)
    //will be private, public now for testing
    int kills;
    int deaths;
    int powerups_collected;
    int slash_attack_num;
    float distance_travelled;

private:
    std::string name;
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f facing_dir;
    sf::Vector2f attack_collision_pt;
    sf::Color color = sf::Color(255,255,255,255);
    PlayerState state;
    float mass;
    float stamina;
    float speed_boost;
    float attack_charge;
    float attack_release_timer;
    float attack_duration_timer;
};

#endif