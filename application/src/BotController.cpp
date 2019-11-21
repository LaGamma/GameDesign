#include "BotController.h"
#include <iostream>
#include <cstring>
#include <cfloat>

//for ai, generally move towards center if player near edge shortest path towards where player is headed or is depending on difficulty
// , if in close proximity to fish or closer to fish than player shortest path to fish.

BotController::BotController() {
  state = 0;
};

void BotController::update(sf::RenderWindow &window, GameLogic &logic, float dSec, int playerNum, Animation &anim) {


    float bot_handicap = 1.0;  // higher number == slower bot

    sf::Vector2f dir = sf::Vector2f(0,0);
    sf::Vector2f w1_pos = logic.walrus1.getPos();
    sf::Vector2f w2_pos = logic.walrus2.getPos();
    sf::Vector2f w1_vel = logic.walrus1.getVel();
    sf::Vector2f w2_vel = logic.walrus2.getVel();


    if (playerNum == 1) {
        //process input for player 1
        if(!logic.walrus2.isDead() && (state != 0) && (w2_vel.x>=15 || w2_vel.y>=15)){
          changeState(0);
          calculatePath(logic, playerNum);
        }
        if(!logic.walrus2.isDead() && (state != 1) && (w2_vel.x<15 || w2_vel.y<15)){
          changeState(1);
          calculatePath(logic, playerNum);
        }
        if(logic.walrus2.isDead() && (state != 2)){
          changeState(2);
          calculatePath(logic, playerNum);
        }
        if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)].pj){
          //direction = left
          dir.x -=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)].pj){
          //direction = right
          dir.x +=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)][int(w1_pos.y/20)+1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)][int(w1_pos.y/20)+1].pj){
          //direction = down
          dir.y +=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)][int(w1_pos.y/20)-1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)][int(w1_pos.y/20)-1].pj){
          //direction = up
          dir.y -=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)-1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)-1].pj){
          //direction = left up
          dir.x -=1;
          dir.y -= 1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)-1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)-1].pj){
          //direction = right up
          dir.x +=1;
          dir.y-=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)+1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)-1][int(w1_pos.y/20)+1].pj){
          //direction = down left
          dir.x -= 1;
          dir.y +=1;
        }
        else if(int(w1_pos.x/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)+1].pi && int(w1_pos.y/20)==cellDetails[int(w1_pos.x/20)+1][int(w1_pos.y/20)+1].pj){
          //direction = down right
          dir.y +=1;
          dir.x += 1;
        }
        logic.walrus1.applyActiveForce(dir, dSec/bot_handicap);

    } else {
        //process input for player 2
        if(!logic.walrus1.isDead() && (state != 0) && (w1_vel.x>=15 || w1_vel.y>=15)){
          changeState(0);
          calculatePath(logic, playerNum);
        }
        if(!logic.walrus1.isDead() && (state != 1)  && (w1_vel.x<15 || w1_vel.y<15)){
          changeState(1);
          calculatePath(logic, playerNum);
        }
        if(logic.walrus1.isDead() && (state != 2)){
          changeState(2);
          calculatePath(logic, playerNum);
        }
        if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)].pj){
          //direction = left
          dir.x -=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)].pj){
          //direction = right
          dir.x +=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)][int(w2_pos.y/20)+1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)][int(w2_pos.y/20)+1].pj){
          //direction = down
          dir.y +=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)][int(w2_pos.y/20)-1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)][int(w2_pos.y/20)-1].pj){
          //direction = up
          dir.y -=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)-1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)-1].pj){
          //direction = left up
          dir.x -=1;
          dir.y -= 1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)-1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)-1].pj){
          //direction = right up
          dir.x +=1;
          dir.y-=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)+1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)-1][int(w2_pos.y/20)+1].pj){
          //direction = down left
          dir.x -= 1;
          dir.y +=1;
        }
        else if(int(w2_pos.x/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)+1].pi && int(w2_pos.y/20)==cellDetails[int(w2_pos.x/20)+1][int(w2_pos.y/20)+1].pj){
          //direction = down right
          dir.y +=1;
          dir.x += 1;
        }
        logic.walrus2.applyActiveForce(dir, dSec/bot_handicap);

    }
    anim.updateMovement(dir, dSec/bot_handicap);

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
                logic.togglePause();
                break;
            case sf::Event::GainedFocus:
                break;
            case sf::Event::KeyPressed:
                if (Event.key.code == sf::Keyboard::P) {
                    logic.togglePause();
                }
                break;
        }
    }
};

void BotController::calculatePath(GameLogic &logic, int playerNum){
  sf::Vector2f w_pos;
  sf::Vector2f w_vel;
  if(playerNum==1){
    w_pos = logic.walrus1.getPos();
    w_vel = logic.walrus1.getVel();
  }
  else{
    w_pos = logic.walrus2.getPos();
    w_vel = logic.walrus2.getVel();
  }
  memset(closedList, false, sizeof (closedList));
  int i, j, x, y;
  int newCost;
  if(state == 0){
    //calculate safe positions, for now middle of the stage.

    std::cout<<state<<"\n";
    x = 20;
    y = 15;
  }
  else if(state == 1){

    std::cout<<state<<"\n";
    if(playerNum==1){
      x = int(logic.walrus2.getPos().x/20);
      y = int(logic.walrus2.getPos().y/20);
    }
    else {
      x = int(logic.walrus1.getPos().x/20);
      y = int(logic.walrus1.getPos().y/20);
    }
  }
  else{
    std::cout<<state<<"\n";
    if(playerNum==1){
      x = 0;
      y = 15;
    }
    else {
      x = 39;
      y = 15;
    }
  }
  for (i=0; i<ICE_BLOCKS_WIDTH; i++)
  {
      for (j=0; j<ICE_BLOCKS_HEIGHT; j++)
      {
          cellDetails[i][j].f = FLT_MAX;
          cellDetails[i][j].g = FLT_MAX;
          cellDetails[i][j].h = FLT_MAX;
          cellDetails[i][j].pos_x = i;
          cellDetails[i][j].pos_y = j;
          cellDetails[i][j].pi = -1;
          cellDetails[i][j].pj = -1;
      }
  }
  i = w_pos.x/ICE_BLOCKS_SIZE_X;
  j = w_pos.y/ICE_BLOCKS_SIZE_Y;
  while(!openList.empty()){
    openList.pop();
  } //crucial to start with an empty list!!

  cellDetails[i][j].f = 0;
  cellDetails[i][j].g = 0;
  cellDetails[i][j].h = 0;
  cellDetails[i][j].pi = i;
  cellDetails[i][j].pj = j;
  openList.push(cellDetails[i][j]);
  while(!openList.empty()){
    cell n = openList.front();
    i = n.pos_x;
    j = n.pos_y;
    openList.pop();
    closedList[i][j] = true;

    if (i == x && j == y){
      break;
    }; //if destination, end search
    newCost = cellDetails[i][j].g+std::abs((i-1)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j-1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i-1][j-1] && newCost<cellDetails[i-1][j-1].g && logic.stage.getTileDura(i-1,j-1,logic.getStageProgression())>=.5){
      cellDetails[i-1][j-1].g = newCost;
      cellDetails[i-1][j-1].h = std::abs((i-1)-(x))+std::abs((j-1)-(y));
      cellDetails[i-1][j-1].f = cellDetails[i-1][j-1].h + cellDetails[i-1][j-1].g;
      cellDetails[i-1][j-1].pi = i;
      cellDetails[i-1][j-1].pj = j;
      openList.push(cellDetails[i-1][j-1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j-1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i][j-1] && newCost<cellDetails[i][j-1].g && logic.stage.getTileDura(i,j-1,logic.getStageProgression())>=.5){
      cellDetails[i][j-1].g = newCost;
      cellDetails[i][j-1].h = std::abs((i)-(x))+std::abs((j-1)-(y));
      cellDetails[i][j-1].f = cellDetails[i][j-1].h + cellDetails[i][j-1].g;
      cellDetails[i][j-1].pi = i;
      cellDetails[i][j-1].pj = j;
      openList.push(cellDetails[i][j-1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i-1)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i-1][j] && newCost<cellDetails[i-1][j].g && logic.stage.getTileDura(i-1,j,logic.getStageProgression())>=.5){
      cellDetails[i-1][j].g = newCost;
      cellDetails[i-1][j].h = std::abs((i-1)-(x))+std::abs((j)-(y));
      cellDetails[i-1][j].f = cellDetails[i-1][j].h + cellDetails[i-1][j].g;
      cellDetails[i-1][j].pi = i;
      cellDetails[i-1][j].pj = j;
      openList.push(cellDetails[i-1][j]);
    }
    newCost = cellDetails[i][j].g+std::abs((i-1)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j+1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i-1][j+1] && newCost<cellDetails[i-1][j+1].g && logic.stage.getTileDura(i-1,j+1,logic.getStageProgression())>=.5){
      cellDetails[i-1][j+1].g = newCost;
      cellDetails[i-1][j+1].h = std::abs((i-1)-(x))+std::abs((j+1)-(y));
      cellDetails[i-1][j+1].f = cellDetails[i-1][j+1].h + cellDetails[i-1][j+1].g;
      cellDetails[i-1][j+1].pi = i;
      cellDetails[i-1][j+1].pj = j;
      openList.push(cellDetails[i-1][j+1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j+1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i][j+1] && newCost<cellDetails[i][j+1].g && logic.stage.getTileDura(i,j+1,logic.getStageProgression())>=.5){
      cellDetails[i][j+1].g = newCost;
      cellDetails[i][j+1].h = std::abs((i)-(x))+std::abs((j+1)-(y));
      cellDetails[i][j+1].f = cellDetails[i][j+1].h + cellDetails[i][j+1].g;
      cellDetails[i][j+1].pi = i;
      cellDetails[i][j+1].pj = j;
      openList.push(cellDetails[i][j+1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i+1)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j+1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i+1][j+1] && newCost<cellDetails[i+1][j+1].g && logic.stage.getTileDura(i+1,j+1,logic.getStageProgression())>=.5){
      cellDetails[i+1][j+1].g = newCost;
      cellDetails[i+1][j+1].h = std::abs((i+1)-(x))+std::abs((j+1)-(y));
      cellDetails[i+1][j+1].f = cellDetails[i+1][j+1].h + cellDetails[i+1][j+1].g;
      cellDetails[i+1][j+1].pi = i;
      cellDetails[i+1][j+1].pj = j;
      openList.push(cellDetails[i+1][j+1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i+1)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j-1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i+1][j-1] && newCost<cellDetails[i+1][j-1].g && logic.stage.getTileDura(i+1,j-1,logic.getStageProgression())>=.5){
      cellDetails[i+1][j-1].g = newCost;
      cellDetails[i+1][j-1].h = std::abs((i+1)-(x))+std::abs((j-1)-(y));
      cellDetails[i+1][j-1].f = cellDetails[i+1][j-1].h + cellDetails[i+1][j-1].g;
      cellDetails[i+1][j-1].pi = i;
      cellDetails[i+1][j-1].pj = j;
      openList.push(cellDetails[i+1][j-1]);
    }
    newCost = cellDetails[i][j].g+std::abs((i)-(w_pos.x/ICE_BLOCKS_SIZE_X))+std::abs((j+1)-(w_pos.y/ICE_BLOCKS_SIZE_Y));
    if(!closedList[i][j+1] && newCost<cellDetails[i][j+1].g && logic.stage.getTileDura(i,j+1,logic.getStageProgression())>=.5){
      cellDetails[i][j+1].g = newCost;
      cellDetails[i][j+1].h = std::abs((i)-(x))+std::abs((j+1)-(y));
      cellDetails[i][j+1].f = cellDetails[i][j+1].h + cellDetails[i][j+1].g;
      cellDetails[i][j+1].pi = i;
      cellDetails[i][j+1].pj = j;
      openList.push(cellDetails[i][j+1]);
    }


  }
  while(!directionStack.empty()){
    directionStack.pop();
  }
  i = w_pos.x/ICE_BLOCKS_SIZE_X;
  j = w_pos.y/ICE_BLOCKS_SIZE_Y;
  while((i!=x)||(j!=y)){
    if((cellDetails[x][y].pi<x)&&(cellDetails[x][y].pj<y)) {  // top left corner (1)
      directionStack.push(1);
    }
    else if((cellDetails[x][y].pi==x)&&(cellDetails[x][y].pj<y)) {  // top center (2)
      directionStack.push(2);
    }
    else if((cellDetails[x][y].pi>x)&&(cellDetails[x][y].pj<y)) {  // top right corner (3)
      directionStack.push(3);
    }
    else if((cellDetails[x][y].pi<x)&&(cellDetails[x][y].pj==y)) {  // left (4)
      directionStack.push(4);
    }
    else if((cellDetails[x][y].pi>x)&&(cellDetails[x][y].pj==y)) {  // right (5)
      directionStack.push(5);
    }
    else if((cellDetails[x][y].pi<x)&&(cellDetails[x][y].pj>y)) {  // bottom left (6)
      directionStack.push(6);
    }
    else if((cellDetails[x][y].pi==x)&&(cellDetails[x][y].pj>y)) {  // bottom center (7)
      directionStack.push(7);
    }
    else if((cellDetails[x][y].pi>x)&&(cellDetails[x][y].pj>y)) {  // bottom center (8)
      directionStack.push(8);
    }
    x = cellDetails[x][y].pi;
    y = cellDetails[x][y].pj;
  }
  //this is just a printing system to check paths, not necessary
  // maybe at this point it could store the moves in a stack? and then the update function changes direction based on that stack?
  //just an idea
  //i = cellDetails[x][y].pi;
  //j = cellDetails[x][y].pj;
  //while(true){
  //  if(i==w_pos.x/20 && j==w_pos.y/20){
  //    break;
  //  }
  //  std::cout<<"the x position: "<<i<<"\n";
  //  std::cout<<"the y position: "<<j<<"\n";
  //  std::cout<<"The parent x: "<<cellDetails[i][j].pi<<"\n";
  //  std::cout<<"The parent y: "<<cellDetails[i][j].pj<<"\n\n";
  //  i = cellDetails[i][j].pi;
  //  j = cellDetails[i][j].pj;
  //}
}

void BotController::changeState(int x){
  state = x;
};
