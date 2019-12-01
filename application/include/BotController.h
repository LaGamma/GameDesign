#ifndef _BotController_H_
#define _BotController_H_
#include "Controller.h"


class BotController : public Controller {

  struct Ray {
      sf::Vector2<double> dir;
      double dist;
      int obj;
  };

  public:
      BotController();
      void update(sf::RenderWindow &window, GameLogic &logic, float dSec, int playerNum);
  private:
      void calculateRays(GameLogic &logic, int playerNum);
      void calculateForce(GameLogic &logic, int playerNum);

      enum ControllerState {fighting, exiting};
      ControllerState state;

      std::list<Ray> rays;
      sf::Vector2f dir;

    float top_wall_y;
    float bottom_wall_y;
    float left_wall_x;
    float right_wall_x;
};





#endif
