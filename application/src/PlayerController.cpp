#include "PlayerController.h"
#include <iostream>

PlayerController::PlayerController() {

};

void PlayerController::update(sf::RenderWindow &window, float dSec, int playerNum, GameLogic* logic) {

    if (playerNum == 1) {
        //process keyboard input for player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            logic->walrus1->applyActiveForce(sf::Vector2f(0, -0.5));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            logic->walrus1->applyActiveForce(sf::Vector2f(0, 0.5));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            logic->walrus1->applyActiveForce(sf::Vector2f(-0.5, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            logic->walrus1->applyActiveForce(sf::Vector2f(0.5, 0));
        }
    } else {
        //process keyboard input for player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            logic->walrus2->applyActiveForce(sf::Vector2f(0, -0.5));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            logic->walrus2->applyActiveForce(sf::Vector2f(0,0.5));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            logic->walrus2->applyActiveForce(sf::Vector2f(-0.5, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            logic->walrus2->applyActiveForce(sf::Vector2f(0.5, 0));
        }
    }


    // process events
    sf::Event Event;
    while (window.pollEvent(Event)) {
        switch (Event.type) {
            //window closed
            case sf::Event::Closed:
                window.close();
                break;
                //window out of focus
            case sf::Event::LostFocus:
                logic->togglePause();
                break;
            case sf::Event::GainedFocus:
                break;
            case sf::Event::KeyPressed:
                if (Event.key.code == sf::Keyboard::P) {
                    std::cout << "Pause\n";
                    logic->togglePause();
                }
                break;
        }
    }
};
