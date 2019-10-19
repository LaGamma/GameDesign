#pragma once
#include <SFML/Graphics.hpp>

class animation
{
public:
  animation(sf::Texture* texture, sf::Vector2u spriteCount, float switchTime);
  void Update(int row, float dSec);
  int getSpriteRow();

public:
  sf::IntRect uvRect;

private:
  sf::Vector2u spriteCount; //stores number of rows and columns of sprites
  sf::Vector2u currentSprite; //stores the row and column of current sprites

  float totalTime; //time since last change in sprite
  float switchTime; //time it takes before switching to new image
};
