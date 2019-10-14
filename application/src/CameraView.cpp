#include "CameraView.h"
#include <iostream>
#include <PlayerController.h>

CameraView::CameraView() {

}

void CameraView::init(GameLogic logic) {
  this->logic = logic;
  this->player1Controller = createController(true);
  this->player2Controller = createController(true);
}

void CameraView::draw(sf::RenderWindow &window) {

    GameLogic::GameState state = logic.getState();
    switch (state) {
        case GameLogic::GameState::mainMenu:
            window.clear(sf::Color::Blue);
            break;
        case GameLogic::GameState::playing:
            window.clear(sf::Color::Green);
            break;
        case GameLogic::GameState::pauseMenu:
            window.clear(sf::Color::Yellow);
            break;
        case GameLogic::GameState::gameOverMenu:
            window.clear(sf::Color::Red);
            break;
    }

    // display
    window.display();
}


void CameraView::switchScreen(int screen) {

}


void CameraView::processInput(sf::RenderWindow &window, float dSec) {


    if (logic.getState() == GameLogic::GameState::playing) {
        //ignore input here and instead handle input in instantiated player controllers
        this->player1Controller->update(window, logic, dSec);
        this->player2Controller->update(window, logic, dSec);

    } else {
        //handle game input here (for MainMenu, PauseMenu, GameOverMenu, etc)

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
                    //logic.pauseGame();
                    break;
                case sf::Event::GainedFocus:
                    //logic.resumeGame();
                    break;
                case sf::Event::KeyPressed:
                    if (Event.key.code == sf::Keyboard::Up) {
                        std::cout << "menu up" << std::endl;
                    } else if (Event.key.code == sf::Keyboard::Down) {
                        std::cout << "menu down" << std::endl;
                    } else if (Event.key.code == sf::Keyboard::Return) {
                        std::cout << "start game!" << std::endl;
                        logic.playGame();
                    } else if (Event.key.code == sf::Keyboard::P && logic.getState() == GameLogic::GameState::pauseMenu) {
                        std::cout << "toggle pause" << std::endl;
                        logic.togglePause();
                    }
                    break;
            }
        }

    }



}

std::unique_ptr<Controller> CameraView::createController(bool player) {
    if (player) {
        return std::unique_ptr<Controller>(new PlayerController());
    } else {
        //return BotController()
    }
}
