#ifndef _GameLogic_H_
#define _GameLogic_H_
#include "Player.h"
#include "Stage.h"

class GameLogic {
  public:
    GameLogic();
    void update(float);
    void handlePlayerDeath(int);
    void togglePause();
    void generateMap();
    void playGame();
    enum GameState {mainMenu, playing, pauseMenu, gameOverMenu};
    GameState getState();

    std::unique_ptr<Player> walrus1;
    std::unique_ptr<Player> walrus2;
    Stage stage;

private:
    GameState state;

};

#endif /* _GameLogic_H_ */
