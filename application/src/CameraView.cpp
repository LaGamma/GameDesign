#include "CameraView.h"
#include "Animation.h"
#include "PlayerController.h"
#include "BotController.h"
#include <iostream>
#include <Definitions.h>
#include <cmath>

void CameraView::init() {
    //load in textures
    spriteMapWalrus.loadFromFile("../images/WalrusSS.png");
    spriteMapFish.loadFromFile("../images/FishSS.png");
    menu_background.loadFromFile("../images/menu_title.png");
    stage_progression.loadFromFile("../images/MinimapPlatform.png");
    stage_progression_active.loadFromFile("../images/MinimapPlatformActive.png");
    player1Select.loadFromFile("../images/player1Select.png");
    player2Select.loadFromFile("../images/player2Select.png");
    playerNeutralSelect.loadFromFile("../images/playerNeutralSelect.png");
    playerPortraitFrame.loadFromFile("../images/playerPortraitFrame.png");
    playerPortrait.loadFromFile("../images/playerPortrait.png");
    nameFrame.loadFromFile("../images/nameFrame.png");
    colorIcon.loadFromFile("../images/colorIcon.png");
    roundCounter.loadFromFile("../images/roundCounter.png");
    roundCounter10.loadFromFile("../images/roundCounter10.png");
    roundCounter20.loadFromFile("../images/roundCounter20.png");
    walrusSplash.loadFromFile("../images/walrusSplash.png");
    water.loadFromFile("../images/water_animation.png");
    font.loadFromFile("../fonts/menuFont.ttf");
    soundManager.load();
    walrus1_animation.init(&spriteMapWalrus, sf::Vector2u(3,11), 0.15);
    walrus2_animation.init(&spriteMapWalrus, sf::Vector2u(3,11), 0.15);
    roundCounter10_animation.init(&roundCounter10, sf::Vector2u(3,1), 0.3);
    roundCounter20_animation.init(&roundCounter20, sf::Vector2u(3,1), 0.3);
    walrusSplash_animation.init(&walrusSplash, sf::Vector2u(3,1), 0.3);
    water_animation.init(&water, sf::Vector2u(3,1), 0.9);
    soundManager.playMusic(SoundManager::Music::title);

    for (int i = 0; i < MAX_NUM_OF_FISH; i++) {
        fish_animation_list.push_back(std::unique_ptr<Animation>(new Animation()));
        fish_animation_list.back()->init(&spriteMapFish, sf::Vector2u(2,2), 0.2);
    }

    debug_mode = false;
}

void CameraView::draw(sf::RenderWindow &window, GameLogic &logic) {


    const GameLogic::GameState state = logic.getState();
    switch (state) {
        case GameLogic::GameState::mainMenu:
            drawMainMenu(window, logic);
            soundManager.setMusicVolume(logic.getMusicVolume());
            break;
        case GameLogic::GameState::pauseMenu:
            drawGame(window,logic);
            drawPauseMenu(window, logic);
            soundManager.setMusicVolume(logic.getMusicVolume()*.5);
            break;
        case GameLogic::GameState::playing:
            drawGame(window, logic);
            soundManager.setMusicVolume(logic.getMusicVolume());
            break;
        case GameLogic::GameState::gameOverMenu:
            drawGameOverMenu(window, logic);
            soundManager.setMusicVolume(logic.getMusicVolume());
            break;
        case GameLogic::GameState::optionsMenu:
            drawOptionsMenu(window, logic);
            soundManager.setMusicVolume(logic.getMusicVolume());
            break;
        case GameLogic::GameState::statsMenu:
            drawStatsMenu(window, logic);
            break;
        case GameLogic::GameState::playerSelectMenu:
            drawPlayerSelectMenu(window, logic);
            break;

            //player select sub menus
        case GameLogic::GameState::colorSelectSubMenu:
            drawPlayerSelectMenu(window, logic);
            break;
        case GameLogic::GameState::nameTextSubMenu:
            drawPlayerSelectMenu(window, logic);
            break;
        case GameLogic::GameState::helpMenu:
            drawHelpMenu(window, logic);
            break;

    }
    // display
    window.display();
}

void CameraView::drawMainMenu(sf::RenderWindow &window, GameLogic &logic) {

    window.clear(sf::Color(50,50,50));

    //draw water
    sf::RectangleShape water_object;
    water_object.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    water_object.setTexture(&water);
    water_object.setTextureRect(water_animation.uvRect);
    window.draw(water_object);

    //main menu background
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setTexture(&menu_background);
    window.draw(bg);

    //main menu items
    sf::Text Play(PLAY_STRING, font, 75);
    sf::Text Help(HELP_STRING, font, 75);
    sf::Text Options(OPTIONS_STRING, font, 75);
    Play.setPosition(33 * WINDOW_WIDTH / 75, 325 * WINDOW_HEIGHT / 600);
    Help.setPosition(33 * WINDOW_WIDTH / 75, 2 * WINDOW_HEIGHT / 3);
    Options.setPosition(33 * WINDOW_WIDTH / 80, 475 * WINDOW_HEIGHT / 600);

    //handle coloring of selection
    if (main_menu_selection == 'P'){
        Help.setFillColor(sf::Color::White);
        Play.setFillColor(sf::Color::Black);
    }
    if (main_menu_selection == 'H'){
        Play.setFillColor(sf::Color::White);
        Options.setFillColor(sf::Color::White);
        Help.setFillColor(sf::Color::Black);
    }
    if (main_menu_selection == 'O'){
        Help.setFillColor(sf::Color::White);
        Options.setFillColor(sf::Color::Black);
    }

    window.draw(Play);
    window.draw(Help);
    window.draw(Options);

}

void CameraView::drawPauseMenu(sf::RenderWindow &window, GameLogic &logic) {

    // draw transparent screen
    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    rect.setFillColor(sf::Color(255,255,0,128));
    window.draw(rect);

    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(UI_TEXT_SIZE);
    pauseText.setFillColor(sf::Color(200, 200, 200, 255));
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(2);
    pauseText.setPosition(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 4);
    pauseText.setString(PAUSED);
    window.draw(pauseText);

    sf::Text resumeText;
    resumeText.setFont(font);
    resumeText.setCharacterSize(UI_TEXT_SIZE);
    resumeText.setFillColor(sf::Color(255, 255, 255, 255));
    resumeText.setOutlineColor(sf::Color::Black);
    resumeText.setOutlineThickness(2);
    resumeText.setPosition(WINDOW_WIDTH / 2 - 110, WINDOW_HEIGHT / 2 + 100);
    resumeText.setString(RESUME);

    sf::Text quit_text;
    quit_text.setFont(font);
    quit_text.setCharacterSize(UI_TEXT_SIZE);
    quit_text.setFillColor(sf::Color(255, 255, 255, 255));
    quit_text.setOutlineColor(sf::Color::Black);
    quit_text.setOutlineThickness(2);
    quit_text.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 170);
    quit_text.setString(QUIT_STRING);


    if (pause_menu_selection == 'Q') {
        quit_text.setFillColor(sf::Color::Black);
        quit_text.setOutlineColor(sf::Color(50,50,50));
    } else if (pause_menu_selection == 'R') {
        resumeText.setFillColor(sf::Color::Black);
        resumeText.setOutlineColor(sf::Color(50,50,50));
    }

    window.draw(resumeText);
    window.draw(quit_text);

}

void CameraView::drawHelpMenu(sf::RenderWindow &window, GameLogic &logic) {
  window.clear(sf::Color(50,50,50));
  sf::Text instructions1(BACKGROUND_A + logic.walrus1->getName() + " & " + logic.walrus2->getName() + BACKGROUND_B, font, UI_TEXT_SIZE/2);
  instructions1.setFillColor(sf::Color(255, 255, 255, 255));
  instructions1.setPosition(WINDOW_WIDTH / 20, 25);
  window.draw(instructions1);
  sf::Text instructions2(CONTROL_INSTRUCTIONS, font, UI_TEXT_SIZE/2);
  instructions2.setFillColor(sf::Color(0, 0, 255, 255));
  instructions2.setPosition(WINDOW_WIDTH / 20, 350);
  window.draw(instructions2);
  sf::Text instructions3(GAME_RULES, font, UI_TEXT_SIZE/2);
  instructions3.setFillColor(sf::Color(255, 255, 0, 255));
  instructions3.setPosition(WINDOW_WIDTH / 20, 480);
  window.draw(instructions3);
  sf::Text instructions4(MAIN_MENU, font, UI_TEXT_SIZE);
  instructions4.setFillColor(sf::Color(0, 0, 0, 255));
  instructions4.setPosition(WINDOW_WIDTH / 2 - 300, 760);
  window.draw(instructions4);

}

void CameraView::drawOptionsMenu(sf::RenderWindow &window, GameLogic &logic) {
    window.clear(sf::Color(50,50,50));

    // draw options title text
    sf::Text options_text(OPTIONS_TITLE_STRING, font, UI_TEXT_SIZE*1.5);
    options_text.setFillColor(sf::Color(255, 255, 255, 255));
    options_text.setPosition(WINDOW_WIDTH / 2 - 200, 25);
    window.draw(options_text);

    // draw sfx option
    sf::Text sfx_text(SFX_VOLUME_STRING, font, UI_TEXT_SIZE/1.5);
    sfx_text.setFillColor(sf::Color(255, 255, 255, 255));
    sfx_text.setPosition(WINDOW_WIDTH / 10, WINDOW_HEIGHT / 4);

    // draw sfx line
    sf::RectangleShape sfx_line(sf::Vector2f(VOLUME_BAR_WIDTH, sfx_text.getGlobalBounds().height/2));
    sfx_line.setFillColor(sf::Color::Black);
    sfx_line.setPosition(sfx_text.getGlobalBounds().left + sfx_text.getGlobalBounds().width + 50, sfx_text.getGlobalBounds().top + 10);
    window.draw(sfx_line);
    // draw sfx circle
    sf::CircleShape sfx_choice(UI_TEXT_SIZE / 6);
    sfx_choice.setFillColor(sf::Color(0, 255, 255, 255));
    sfx_choice.setPosition(sfx_line.getPosition().x - sfx_choice.getRadius() + (logic.getSFXVolume()/SFX_VOLUME_MAX)*sfx_line.getLocalBounds().width, sfx_line.getPosition().y - 9);
    window.draw(sfx_choice);

    // draw music option
    sf::Text music_text(MUSIC_VOLUME_STRING, font, UI_TEXT_SIZE/1.5);
    music_text.setFillColor(sf::Color(255, 255, 255, 255));
    music_text.setPosition(WINDOW_WIDTH / 10, sfx_text.getPosition().y + 120);
    // draw music line
    sf::RectangleShape music_line(sf::Vector2f(VOLUME_BAR_WIDTH, music_text.getGlobalBounds().height/2));
    music_line.setFillColor(sf::Color::Black);
    music_line.setPosition(sfx_line.getPosition().x, music_text.getGlobalBounds().top + 10);
    window.draw(music_line);
    // draw music circle
    sf::CircleShape music_choice(UI_TEXT_SIZE / 6);
    music_choice.setFillColor(sf::Color(0, 255, 255, 255));
    music_choice.setPosition(music_line.getPosition().x - music_choice.getRadius() + (logic.getMusicVolume()/MUSIC_VOLUME_MAX)*music_line.getLocalBounds().width, music_line.getPosition().y - 9);
    window.draw(music_choice);

    sf::Text quit_text(QUIT_STRING, font, UI_TEXT_SIZE/1.5);
    quit_text.setFillColor(sf::Color(255, 255, 255, 255));
    quit_text.setPosition(WINDOW_WIDTH / 10, music_text.getPosition().y + 150);


    //handle coloring of selection
    if (options_menu_selection == 'S') {
        sfx_text.setFillColor(sf::Color::Cyan);
    }
    if (options_menu_selection == 'M') {
        music_text.setFillColor(sf::Color::Cyan);
    }
    if (options_menu_selection == 'Q') {
        quit_text.setFillColor(sf::Color::Cyan);
    }

    window.draw(sfx_text);
    window.draw(music_text);
    window.draw(quit_text);

}

void CameraView::drawPlayerSelectMenu(sf::RenderWindow &window, GameLogic &logic) {
    window.clear(sf::Color(50,50,50));

    //play text
    sf::Text play_text;
    play_text.setFont(font);
    play_text.setCharacterSize(UI_TEXT_SIZE);
    play_text.setFillColor(sf::Color(255, 255, 255, 255));
    play_text.setPosition(33 * WINDOW_WIDTH / 75, 2 * WINDOW_HEIGHT / 3);
    play_text.setString(PLAY_STRING);

    //quit text
    sf::Text quit_text;
    quit_text.setFont(font);
    quit_text.setCharacterSize(UI_TEXT_SIZE);
    quit_text.setFillColor(sf::Color(255, 255, 255, 255));
    quit_text.setPosition(33 * WINDOW_WIDTH / 75, 475 * WINDOW_HEIGHT / 600);
    quit_text.setString(QUIT_STRING);

    //Player portrait frame
    sf::RectangleShape player1_portrait_frame = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    sf::RectangleShape player2_portrait_frame = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    player1_portrait_frame.setTexture(&playerPortraitFrame);
    player2_portrait_frame.setTexture(&playerPortraitFrame);
    player1_portrait_frame.setPosition(50*WINDOW_WIDTH/800, 60*WINDOW_HEIGHT/600);
    player2_portrait_frame.setPosition(player1_portrait_frame.getPosition().x + player1_portrait_frame.getSize().x + 50*WINDOW_WIDTH/800, 60*WINDOW_HEIGHT/600);
    window.draw(player1_portrait_frame);
    window.draw(player2_portrait_frame);

    //bot or player text
    sf::Text player1Type_text;
    player1Type_text.setFont(font);
    player1Type_text.setCharacterSize(75);
    player1Type_text.setFillColor(sf::Color(255, 255, 255, 255));
    player1Type_text.setOutlineColor(sf::Color::Black);
    player1Type_text.setOutlineThickness(1);
    player1Type_text.setPosition(player1_portrait_frame.getPosition().x+200, player1_portrait_frame.getPosition().y -90);
    sf::Text player2Type_text;
    player2Type_text.setFont(font);
    player2Type_text.setCharacterSize(75);
    player2Type_text.setFillColor(sf::Color(255, 255, 255, 255));
    player2Type_text.setOutlineColor(sf::Color::Black);
    player2Type_text.setOutlineThickness(1);
    player2Type_text.setPosition(player2_portrait_frame.getPosition().x+200, player2_portrait_frame.getPosition().y -90);
    if (player1OrBot == 1) {
        player1Type_text.setString(P1);
        //adjust position so centered over portrait
        player1Type_text.setPosition(player1_portrait_frame.getPosition().x+150, player1_portrait_frame.getPosition().y -90);
        player1Type_text.setFillColor(sf::Color::Red);
    }
    else
        player1Type_text.setString(BOT);
    if (player2OrBot == 1) {
        player2Type_text.setString(P2);
        player2Type_text.setPosition(player2_portrait_frame.getPosition().x+150, player2_portrait_frame.getPosition().y -90);
        player2Type_text.setFillColor(sf::Color::Blue);
    }
    else
        player2Type_text.setString(BOT);
    window.draw(player1Type_text);
    window.draw(player2Type_text);

    //Player portraits
    sf::RectangleShape player1_portrait = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    sf::RectangleShape player2_portrait = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    player1_portrait.setTexture(&playerPortrait);
    player1_portrait.setFillColor(logic.walrus1->getColor());
    player2_portrait.setTexture(&playerPortrait);
    player2_portrait.setFillColor(logic.walrus2->getColor());
    player1_portrait.setPosition(player1_portrait_frame.getPosition());
    player2_portrait.setPosition(player2_portrait_frame.getPosition());
    window.draw(player1_portrait);
    window.draw(player2_portrait);

    //selected borders
    sf::RectangleShape player1_portrait_border = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    sf::RectangleShape player2_portrait_border = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    player1_portrait_border.setTexture(&player1Select);
    player2_portrait_border.setTexture(&player2Select);
    player1_portrait_border.setPosition(player1_portrait.getPosition());
    player2_portrait_border.setPosition(player2_portrait.getPosition());

    //selection borders
    sf::RectangleShape player1_portrait_indicator = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    sf::RectangleShape player2_portrait_indicator = sf::RectangleShape(sf::Vector2f(325*WINDOW_WIDTH/800, 425*WINDOW_HEIGHT/800));
    player1_portrait_indicator.setTexture(&playerNeutralSelect);
    player2_portrait_indicator.setTexture(&playerNeutralSelect);
    player1_portrait_indicator.setPosition(player1_portrait.getPosition());
    player2_portrait_indicator.setPosition(player2_portrait.getPosition());

    //name frame
    sf::RectangleShape player1_name_bg = sf::RectangleShape(sf::Vector2f(300,60));
    sf::RectangleShape player2_name_bg = sf::RectangleShape(sf::Vector2f(300,60));
    sf::RectangleShape player1_name_border = sf::RectangleShape(sf::Vector2f(330,90));
    sf::RectangleShape player2_name_border = sf::RectangleShape(sf::Vector2f(330,90));
    player1_name_bg.setFillColor(sf::Color(100,100,100));
    player2_name_bg.setFillColor(sf::Color(100,100,100));

    //name text
    sf::Text name1_text;
    sf::Text name2_text;
    name1_text.setFont(font);
    name1_text.setCharacterSize(UI_TEXT_SIZE);
    name1_text.setFillColor(sf::Color(255,255,255,255));
    name1_text.setString(logic.walrus1->getName());
    name2_text.setFont(font);
    name2_text.setCharacterSize(UI_TEXT_SIZE);
    name2_text.setFillColor(sf::Color(255,255,255,255));
    name2_text.setString(logic.walrus2->getName());


    if (enteringNameText) {
        if (player1_menu_selection == '1') {
            player1_name_border.setFillColor(sf::Color(255, 0, 0));
            name1_text.setString(walrus1_name_str);
        } else {
            player2_name_border.setFillColor(sf::Color(255, 0, 0));
            name2_text.setString(walrus2_name_str);
        }
    } else {
        player1_name_border.setFillColor(sf::Color(200, 200, 200));
        player2_name_border.setFillColor(sf::Color(200, 200, 200));
    }
    player1_name_bg.setPosition((player1_portrait.getPosition().x+115),player1_portrait.getPosition().y+320);
    player2_name_bg.setPosition((player2_portrait.getPosition().x+115),player2_portrait.getPosition().y+320);
    player1_name_border.setPosition((player1_portrait.getPosition().x+100), player1_portrait.getPosition().y+310);
    player2_name_border.setPosition((player2_portrait.getPosition().x+100), player2_portrait.getPosition().y+310);
    name1_text.setPosition(player1_name_bg.getPosition().x+10, player1_name_bg.getPosition().y-55);
    name2_text.setPosition(player2_name_bg.getPosition().x+10, player2_name_bg.getPosition().y-55);
    window.draw(player1_name_border);
    window.draw(player2_name_border);
    window.draw(player1_name_bg);
    window.draw(player2_name_bg);
    window.draw(name2_text);
    window.draw(name1_text);


    //color icons
    sf::RectangleShape ciBrown = sf::RectangleShape(sf::Vector2f(60,60));
    sf::RectangleShape ciDBrown  = sf::RectangleShape(sf::Vector2f(60,60));
    sf::RectangleShape ciWhite = sf::RectangleShape(sf::Vector2f(60,60));
    sf::RectangleShape ciBlack = sf::RectangleShape(sf::Vector2f(60,60));
    sf::RectangleShape ciGray = sf::RectangleShape(sf::Vector2f(60,60));
    ciBrown.setTexture(&colorIcon);
    ciDBrown.setTexture(&colorIcon);
    ciWhite.setTexture(&colorIcon);
    ciBlack.setTexture(&colorIcon);
    ciGray.setTexture(&colorIcon);
    ciBrown.setFillColor(player_color1);
    ciDBrown.setFillColor(player_color2);
    ciWhite.setFillColor(player_color3);
    ciBlack.setFillColor(player_color4);
    ciGray.setFillColor(player_color5);
    if (player1_menu_selection == '1')
        ciBrown.setPosition(player1_name_bg.getPosition().x-25,player1_name_bg.getPosition().y+65);
    else
        ciBrown.setPosition(player2_name_bg.getPosition().x-25,player2_name_bg.getPosition().y+65);
    ciDBrown.setPosition(ciBrown.getPosition().x+60, ciBrown.getPosition().y);
    ciWhite.setPosition(ciBrown.getPosition().x+120, ciBrown.getPosition().y);
    ciBlack.setPosition(ciBrown.getPosition().x+180, ciBrown.getPosition().y);
    ciGray.setPosition(ciBrown.getPosition().x+240, ciBrown.getPosition().y);

    //color icon indicator
    sf::RectangleShape colorSelectionIndicator = sf::RectangleShape(sf::Vector2f(50,50));
    colorSelectionIndicator.setOutlineColor(sf::Color(255,0,0,255));
    colorSelectionIndicator.setOutlineThickness(5);
    colorSelectionIndicator.setFillColor(sf::Color(255,255,255,0));
    if (colorSelector) {
        window.draw(ciBrown);
        window.draw(ciDBrown);
        window.draw(ciWhite);
        window.draw(ciBlack);
        window.draw(ciGray);
        if (color_selection == '1') {
            colorSelectionIndicator.setPosition(ciBrown.getPosition().x + 5, ciBrown.getPosition().y + 5);
            window.draw(colorSelectionIndicator);
            //change player and portrait colors
            if(player1_menu_selection == '1') {
                logic.walrus1->setColor(player_color1);
                player1_portrait.setFillColor(player_color1);
            }
            if(player1_menu_selection == '2') {
                logic.walrus2->setColor(player_color1);
                player2_portrait.setFillColor(player_color1);
            }
        }
        if (color_selection == '2') {
            colorSelectionIndicator.setPosition(ciDBrown.getPosition().x + 5, ciDBrown.getPosition().y + 5);
            window.draw(colorSelectionIndicator);
            //change player and portrait colors
            if(player1_menu_selection == '1') {
                logic.walrus1->setColor(player_color2);
                player1_portrait.setFillColor(player_color2);
            }
            if(player1_menu_selection == '2') {
                logic.walrus2->setColor(player_color2);
                player2_portrait.setFillColor(player_color2);
            }
        }
        if (color_selection == '3') {
            colorSelectionIndicator.setPosition(ciWhite.getPosition().x + 5, ciWhite.getPosition().y + 5);
            window.draw(colorSelectionIndicator);
            //change player and portrait colors
            if(player1_menu_selection == '1') {
                logic.walrus1->setColor(player_color3);
                player1_portrait.setFillColor(player_color3);
            }
            if(player1_menu_selection == '2') {
                logic.walrus2->setColor(player_color3);
                player2_portrait.setFillColor(player_color3);
            }
        }
        if (color_selection == '4') {
            colorSelectionIndicator.setPosition(ciBlack.getPosition().x + 5, ciBlack.getPosition().y + 5);
            window.draw(colorSelectionIndicator);
            //change player and portrait colors
            if(player1_menu_selection == '1') {
                logic.walrus1->setColor(player_color4);
                player1_portrait.setFillColor(player_color4);
            }
            if(player1_menu_selection == '2') {
                logic.walrus2->setColor(player_color4);
                player2_portrait.setFillColor(player_color4);
            }
        }
        if (color_selection == '5') {
            colorSelectionIndicator.setPosition(ciGray.getPosition().x + 5, ciGray.getPosition().y + 5);
            window.draw(colorSelectionIndicator);
            //change player and portrait colors
            if(player1_menu_selection == '1') {
                logic.walrus1->setColor(player_color5);
                player1_portrait.setFillColor(player_color5);
            }
            if(player1_menu_selection == '2') {
                logic.walrus2->setColor(player_color5);
                player2_portrait.setFillColor(player_color5);
            }
        }
    }


    //handle coloring of selection for player 1
    if (player1OrBot == 1 ) {
        window.draw(player1_portrait_border);
    }if (player2OrBot == 1 ) {
        window.draw(player2_portrait_border);
    }if (player1_menu_selection == '1'){
        window.draw(player1_portrait_indicator);
    }if (player1_menu_selection == '2'){
        window.draw(player2_portrait_indicator);
    }if (player1_menu_selection == 'P') {
        play_text.setFillColor(sf::Color::Cyan);
    }if (player1_menu_selection == 'Q') {
        quit_text.setFillColor(sf::Color::Cyan);
    }

    window.draw(play_text);
    window.draw(quit_text);

}


void CameraView::drawGameOverMenu(sf::RenderWindow &window, GameLogic &logic) {

    window.clear(sf::Color(20,20,150));
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(UI_TEXT_SIZE);
    text.setFillColor(sf::Color(255,255,255,255));
    text.setPosition(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 12);


    if (logic.winner1)
    {
        //walrus1 won
        walrus1_animation.setCurrentSprite(0,0);
        player1.setFillColor(logic.walrus1->getColor());
        player1.setTextureRect(walrus1_animation.uvRect);
        window.draw(player1);
        text.setString(logic.walrus1->getName() + WON);
    }

    else
    {
        //walrus2 won
        walrus2_animation.setCurrentSprite(0,0);
        player2.setFillColor(logic.walrus2->getColor());
        player2.setTextureRect(walrus2_animation.uvRect);
        window.draw(player2);
        text.setString(logic.walrus2->getName() + WON);
    }

    //draw Play Again, Stats, and Quit options
    sf::Text replay_text;
    replay_text.setFont(font);
    replay_text.setCharacterSize(UI_TEXT_SIZE);
    replay_text.setFillColor(sf::Color(255, 255, 255, 255));
    replay_text.setPosition(WINDOW_WIDTH / 4, 7*WINDOW_HEIGHT / 12);
    replay_text.setString(REPLAY);

    sf::Text stats_text;
    stats_text.setFont(font);
    stats_text.setCharacterSize(UI_TEXT_SIZE);
    stats_text.setFillColor(sf::Color(255, 255, 255, 255));
    stats_text.setPosition(replay_text.getPosition().x, replay_text.getPosition().y + 70);
    stats_text.setString(STATS_TEXT);

    sf::Text quit_text;
    quit_text.setFont(font);
    quit_text.setCharacterSize(UI_TEXT_SIZE);
    quit_text.setFillColor(sf::Color(255, 255, 255, 255));
    quit_text.setPosition(replay_text.getPosition().x, stats_text.getPosition().y + 70);
    quit_text.setString(QUIT_STRING);

    //handle coloring of selection
    if (game_over_menu_selection == 'P') {
        replay_text.setFillColor(sf::Color::Black);
    }
    if (game_over_menu_selection == 'S') {
        stats_text.setFillColor(sf::Color::Black);
    }
    if (game_over_menu_selection == 'Q') {
        quit_text.setFillColor(sf::Color::Black);
    }

    window.draw(stats_text);
    window.draw(replay_text);
    window.draw(quit_text);
    window.draw(text);
}

void CameraView::drawStatsMenu(sf::RenderWindow &window, GameLogic &logic) {
    //stats: kills, deaths, powerups collected, slash attacks used, meters travelled (need to figure out a way to do this)
    //divide screen in half, walrus 1 and walrus 2 stats:
    window.clear(sf::Color(137, 207, 240));

    //draw walrus 1 header
    sf::Text walrus1header(logic.walrus1->getName(), font, UI_TEXT_SIZE);
    walrus1header.setFillColor(sf::Color::Black);
    walrus1header.setPosition(WINDOW_WIDTH / 10, WINDOW_HEIGHT / 10);
    window.draw(walrus1header);

    //draw walrus 2 header
    sf::Text walrus2header;
    walrus2header.setFont(font);
    walrus2header.setCharacterSize(UI_TEXT_SIZE);
    walrus2header.setFillColor(sf::Color::Black);
    walrus2header.setPosition(walrus1header.getPosition().x + walrus1header.getLocalBounds().width + WINDOW_WIDTH/3, WINDOW_HEIGHT / 10);
    walrus2header.setString(logic.walrus2->getName());
    window.draw(walrus2header);

    sf::Text kills_text1;
    kills_text1.setFont(font);
    kills_text1.setCharacterSize(UI_TEXT_SIZE / 2);
    kills_text1.setFillColor(sf::Color::Black);
    kills_text1.setPosition(walrus1header.getPosition().x, walrus1header.getPosition().y + 100);
    std::string kills_string = KILLS + std::to_string(logic.walrus1->kills);
    kills_text1.setString(kills_string);
    window.draw(kills_text1);

    sf::Text kills_text2;
    kills_text2.setFont(font);
    kills_text2.setCharacterSize(UI_TEXT_SIZE / 2);
    kills_text2.setFillColor(sf::Color::Black);
    kills_text2.setPosition(walrus2header.getPosition().x, walrus1header.getPosition().y + 100);
    std::string kills_string2 = KILLS + std::to_string(logic.walrus2->kills);
    kills_text2.setString(kills_string2);
    window.draw(kills_text2);

    sf::Text deaths_text1;
    deaths_text1.setFont(font);
    deaths_text1.setCharacterSize(UI_TEXT_SIZE / 2);
    deaths_text1.setFillColor(sf::Color::Black);
    deaths_text1.setPosition(walrus1header.getPosition().x, kills_text1.getPosition().y + 50);
    std::string death_string1 = DEATHS + std::to_string(logic.walrus1->deaths);
    deaths_text1.setString(death_string1);
    window.draw(deaths_text1);

    sf::Text deaths_text2;
    deaths_text2.setFont(font);
    deaths_text2.setCharacterSize(UI_TEXT_SIZE / 2);
    deaths_text2.setFillColor(sf::Color::Black);
    deaths_text2.setPosition(walrus2header.getPosition().x, kills_text2.getPosition().y + 50);
    std::string death_string2 = DEATHS + std::to_string(logic.walrus2->deaths);
    deaths_text2.setString(death_string2);
    window.draw(deaths_text2);

    sf::Text powerup_text1;
    powerup_text1.setFont(font);
    powerup_text1.setCharacterSize(UI_TEXT_SIZE / 2);
    powerup_text1.setFillColor(sf::Color::Black);
    powerup_text1.setPosition(walrus1header.getPosition().x, deaths_text1.getPosition().y + 50);
    std::string powerup_string1 = POWERUPS + std::to_string(logic.walrus1->powerups_collected);
    powerup_text1.setString(powerup_string1);
    window.draw(powerup_text1);

    sf::Text powerup_text2;
    powerup_text2.setFont(font);
    powerup_text2.setCharacterSize(UI_TEXT_SIZE / 2);
    powerup_text2.setFillColor(sf::Color::Black);
    powerup_text2.setPosition(walrus2header.getPosition().x, deaths_text2.getPosition().y + 50);
    std::string powerup_string2 = POWERUPS + std::to_string(logic.walrus2->powerups_collected);
    powerup_text2.setString(powerup_string2);
    window.draw(powerup_text2);

    sf::Text slash_attacks_text1;
    slash_attacks_text1.setFont(font);
    slash_attacks_text1.setCharacterSize(UI_TEXT_SIZE / 2);
    slash_attacks_text1.setFillColor(sf::Color::Black);
    slash_attacks_text1.setPosition(walrus1header.getPosition().x, powerup_text1.getPosition().y + 50);
    std::string slash_string1 = SLASH_ATTACKS + std::to_string(logic.walrus1->slash_attack_num);
    slash_attacks_text1.setString(slash_string1);
    window.draw(slash_attacks_text1);

    sf::Text slash_attacks_text2;
    slash_attacks_text2.setFont(font);
    slash_attacks_text2.setCharacterSize(UI_TEXT_SIZE / 2);
    slash_attacks_text2.setFillColor(sf::Color::Black);
    slash_attacks_text2.setPosition(walrus2header.getPosition().x, powerup_text2.getPosition().y + 50);
    std::string slash_string2 = SLASH_ATTACKS + std::to_string(logic.walrus2->slash_attack_num);
    slash_attacks_text2.setString(slash_string2);
    window.draw(slash_attacks_text2);


    int distance1 = int(logic.walrus1->distance_travelled) / 10;
    sf::Text meter_text1;
    meter_text1.setFont(font);
    meter_text1.setCharacterSize(UI_TEXT_SIZE / 2);
    meter_text1.setFillColor(sf::Color::Black);
    meter_text1.setPosition(walrus1header.getPosition().x, slash_attacks_text1.getPosition().y + 50);
    std::string meter_string1 = DIST + std::to_string(distance1);
    meter_text1.setString(meter_string1);
    window.draw(meter_text1);


    int distance2 = int(logic.walrus2->distance_travelled) / 10;
    sf::Text meter_text2;
    meter_text2.setFont(font);
    meter_text2.setCharacterSize(UI_TEXT_SIZE / 2);
    meter_text2.setFillColor(sf::Color::Black);
    meter_text2.setPosition(walrus2header.getPosition().x, slash_attacks_text2.getPosition().y + 50);
    std::string meter_string2 = DIST + std::to_string(distance2);
    meter_text2.setString(meter_string2);
    window.draw(meter_text2);

    //Main Menu text
    sf::Text quit_text;
    quit_text.setFont(font);
    quit_text.setCharacterSize(UI_TEXT_SIZE / 2);
    quit_text.setFillColor(sf::Color::Black);
    quit_text.setPosition(2*WINDOW_WIDTH / 5, WINDOW_HEIGHT - 100);
    quit_text.setString(MAIN_MENU);
    window.draw(quit_text);

}


void CameraView::drawGame(sf::RenderWindow &window, GameLogic &logic) {

    window.clear(sf::Color::Blue);

    //draw water
    sf::RectangleShape water_object;
    // add a bit extra on the edges to account for screen shake
    water_object.setSize(sf::Vector2f(WINDOW_WIDTH+50, WINDOW_HEIGHT+50));
    water_object.setPosition(-25, -25);
    water_object.setTexture(&water);
    water_object.setTextureRect(water_animation.uvRect);
    window.draw(water_object);

    //draw ice blocks
    for (int i = 0; i < ICE_BLOCKS_WIDTH+2; i++) {
        for (int j = 0; j < ICE_BLOCKS_HEIGHT+2; j++) {
            float dura = logic.stage->getTileDura(i, j, logic.getStageProgression());
            if (dura > 0) {
                // draw ice graphics based on melt
                ice.setSize(sf::Vector2f(ICE_BLOCKS_SIZE_X * dura, ICE_BLOCKS_SIZE_Y * dura));
                ice.setPosition((i * ICE_BLOCKS_SIZE_X + ((ICE_BLOCKS_SIZE_X - ice.getSize().x) / 2)), (j * ICE_BLOCKS_SIZE_Y + ((ICE_BLOCKS_SIZE_Y - ice.getSize().y) / 2)));
                ice.setFillColor(sf::Color(50, 247, 250, 230 * dura));
                ice.setOutlineColor(sf::Color(255, 255, 255, 255));
                ice.setOutlineThickness(4 * dura);
                window.draw(ice);

            }
        }
    }

    //draw splash
    sf::RectangleShape walrusSplash_object = sf::RectangleShape(sf::Vector2f(60,60));
    walrusSplash_object.setTexture(&walrusSplash);
    walrusSplash_object.setTextureRect(walrusSplash_animation.uvRect);
    if (logic.walrus1->isDead() && splash_timer) {
        walrusSplash_object.setPosition(logic.walrus1->getPos() - sf::Vector2f(30,30));
        window.draw(walrusSplash_object);
    }
    else if (logic.walrus2->isDead() && splash_timer) {
        walrusSplash_object.setPosition(logic.walrus2->getPos() - sf::Vector2f(30,30));
        window.draw(walrusSplash_object);
    }

    // draw fish
    auto anim = fish_animation_list.begin();
    for (auto fish = logic.fish_list.begin(); fish != logic.fish_list.end(); fish++, anim++) {
        // size of drawn fish depends on fish flop - modeled with a simple quadratic to make it look like it's jumping towards screen
        double size_offset = FISH_SIZE + (-FISH_SIZE*((*fish)->flop_progress_timer-0.5)*((*fish)->flop_progress_timer-0.5) + 1);
        sf::CircleShape fish_circle = sf::CircleShape(size_offset);
        fish_circle.setPosition((*fish)->getPosition() - sf::Vector2f(size_offset, size_offset));
        fish_circle.setTexture(&spriteMapFish);
        fish_circle.setTextureRect((*anim)->uvRect);
        (*anim)->setCurrentSpriteY((*fish)->getColor());
        window.draw(fish_circle);
    }

    /*
    divide the image up in to its individual sprites by using dimensions of
    the image and dividing by the number of images in the rows and columns
    */
    sf::Vector2u textureSize = spriteMapWalrus.getSize();
    textureSize.x /= 3;
    textureSize.y /= 10;
    //circle.setTextureRect(sf::IntRect(textureSize.x * 2, textureSize.y * 4, textureSize.x, textureSize.y));

    // draw Player1
    player1.setSize(sf::Vector2f(logic.walrus1->getMass(), logic.walrus1->getMass()));
    //player1.setPosition(logic.walrus1->getPos().x - player1.getSize().x / 2, logic.walrus1->getPos().y - player1.getSize().y / 2);
    player1.setPosition((logic.walrus1->getPos().x - player1.getSize().x / 2) + rand() % 2*logic.walrus1->getAttackCharge(), (logic.walrus1->getPos().y - player1.getSize().y / 2) + rand() % 2*logic.walrus1->getAttackCharge());
    player1.setFillColor(logic.walrus1->getColor());
    player1.setTexture(&spriteMapWalrus);
    player1.setTextureRect(walrus1_animation.uvRect);

    // draw Player2
    player2.setSize(sf::Vector2f(logic.walrus2->getMass(), logic.walrus2->getMass()));
    //player2.setPosition(logic.walrus2->getPos().x - player2.getSize().x / 2,logic.walrus2->getPos().y - player2.getSize().y / 2);
    player2.setPosition((logic.walrus2->getPos().x - player2.getSize().x / 2) + rand() % 2*logic.walrus2->getAttackCharge(), (logic.walrus2->getPos().y - player2.getSize().y / 2) + rand() % 2*logic.walrus2->getAttackCharge());
    player2.setFillColor(logic.walrus2->getColor());
    player2.setTexture(&spriteMapWalrus);
    player2.setTextureRect(walrus2_animation.uvRect);

    // draw in order of depth
    if (logic.walrus1->getPos().y > logic.walrus2->getPos().y) {
        if (!logic.walrus2->isDead()) {
            window.draw(player2);
            player2.setFillColor(sf::Color(255*sin(15*logic.walrus2->getAttackCharge())+255, 0, 0, 40*logic.walrus2->getAttackCharge()));
            window.draw(player2);
        }
        if (!logic.walrus1->isDead()) {
            window.draw(player1);
            player1.setFillColor(sf::Color(255*sin(15*logic.walrus1->getAttackCharge())+255, 0, 0, 40*logic.walrus1->getAttackCharge()));
            window.draw(player1);
        }
    } else {
        if (!logic.walrus1->isDead()) {
            window.draw(player1);
            player1.setFillColor(sf::Color(255*sin(15*logic.walrus1->getAttackCharge())+255, 0, 0, 40*logic.walrus1->getAttackCharge()));
            window.draw(player1);
        }
        if (!logic.walrus2->isDead()) {
            window.draw(player2);
            player2.setFillColor(sf::Color(255*sin(15*logic.walrus2->getAttackCharge()+255), 0, 0, 40*logic.walrus2->getAttackCharge()));
            window.draw(player2);
        }
    }

    //hitbox.setOutlineThickness(4);
    hitbox.setRadius(logic.walrus1->getMass() * PLAYER_HITBOX_SCALE);
    hitbox.setPosition(logic.walrus1->getPos().x - hitbox.getRadius(), logic.walrus1->getPos().y - hitbox.getRadius());
    hitbox.setFillColor(sf::Color(0, 0, 0, 0));
    window.draw(hitbox);
    hitbox.setRadius(logic.walrus2->getMass() * PLAYER_HITBOX_SCALE);
    hitbox.setPosition(logic.walrus2->getPos().x - hitbox.getRadius(), logic.walrus2->getPos().y - hitbox.getRadius());
    window.draw(hitbox);


    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(UI_TEXT_SIZE);
    text.setFillColor(sf::Color(255, 0, 0, 255));
    if (logic.walrus1->isDead()) {
        text.setString(GO_RIGHT);
        text.setPosition(3 * WINDOW_WIDTH / 4, (WINDOW_HEIGHT / 2) - (UI_TEXT_SIZE / 2));
        window.draw(text);
    } else if (logic.walrus2->isDead()) {
        text.setString(GO_LEFT);
        text.setPosition(WINDOW_WIDTH / 4 - text.getGlobalBounds().width, (WINDOW_HEIGHT / 2) - (UI_TEXT_SIZE / 2));
        window.draw(text);
    }
    // draw collision point
    /*collision_pt.setPosition(logic.playerCollisionPoint - sf::Vector2f(5, 5));
    collision_pt.setRadius(5);
    collision_pt.setFillColor(sf::Color::Red);
    window.draw(collision_pt);*/

    /*// draw w1 attack point
    if (logic.walrus1->getState() == Player::attacking) {
        collision_pt.setRadius(logic.walrus1->getMass()*PLAYER_HITBOX_SCALE);
        collision_pt.setPosition(logic.p1AttackPoint - sf::Vector2f(collision_pt.getRadius(), collision_pt.getRadius()));
        collision_pt.setFillColor(sf::Color(255,0,0,100));
        window.draw(collision_pt);
    }
    // draw w2 attack point
    if (logic.walrus2->getState() == Player::attacking) {
        collision_pt.setRadius(logic.walrus2->getMass()*PLAYER_HITBOX_SCALE);
        collision_pt.setPosition(logic.p2AttackPoint - sf::Vector2f(collision_pt.getRadius(), collision_pt.getRadius()));
        collision_pt.setFillColor(sf::Color(255,0,0,100));
        window.draw(collision_pt);
    }*/

    //draw minimap background
    /*
    minimapbg.setSize(sf::Vector2f(440, 70));
    minimapbg.setPosition(180, 515);
    minimapbg.setFillColor(sf::Color::Blue);
    window.draw(minimapbg);
    */

    //draw minimap, drawn as individual platforms
    mmStageL2.setSize(sf::Vector2f(140*WINDOW_WIDTH/800, WINDOW_HEIGHT/6));
    mmStageL.setSize(sf::Vector2f(170*WINDOW_WIDTH/800, 120*WINDOW_HEIGHT/600));
    mmStage.setSize(sf::Vector2f(200*WINDOW_WIDTH/800, 140*WINDOW_HEIGHT/600));
    mmStageR.setSize(sf::Vector2f(170*WINDOW_WIDTH/800, 120*WINDOW_HEIGHT/600));
    mmStageR2.setSize(sf::Vector2f(140*WINDOW_WIDTH/800, WINDOW_HEIGHT/6));
    mmStageL2.setTexture(&stage_progression);
    mmStageL.setTexture(&stage_progression);
    mmStage.setTexture(&stage_progression);
    mmStageR.setTexture(&stage_progression);
    mmStageR2.setTexture(&stage_progression);
    mmStageL2.setPosition(190*WINDOW_WIDTH/800, -25*WINDOW_HEIGHT/600);
    mmStageL.setPosition(240*WINDOW_WIDTH/800, -30*WINDOW_HEIGHT/600);
    mmStage.setPosition(300*WINDOW_WIDTH/800, -35*WINDOW_HEIGHT/600);
    mmStageR.setPosition(390*WINDOW_WIDTH/800, -30*WINDOW_HEIGHT/600);
    mmStageR2.setPosition(470*WINDOW_WIDTH/800, -25*WINDOW_HEIGHT/600);
    window.draw(mmStageL2);
    window.draw(mmStageL);
    window.draw(mmStage);
    window.draw(mmStageR);
    window.draw(mmStageR2);

    //initialize minimap indicators
    mmStageL2i.setSize(sf::Vector2f(140*WINDOW_WIDTH/800, 100*WINDOW_HEIGHT/600));
    mmStageLi.setSize(sf::Vector2f(170*WINDOW_WIDTH/800, 120*WINDOW_HEIGHT/600));
    mmStagei.setSize(sf::Vector2f(200*WINDOW_WIDTH/800, 140*WINDOW_HEIGHT/600));
    mmStageRi.setSize(sf::Vector2f(170*WINDOW_WIDTH/800, 120*WINDOW_HEIGHT/600));
    mmStageR2i.setSize(sf::Vector2f(140*WINDOW_WIDTH/800, 100*WINDOW_HEIGHT/600));
    mmStageL2i.setTexture(&stage_progression_active);
    mmStageLi.setTexture(&stage_progression_active);
    mmStagei.setTexture(&stage_progression_active);
    mmStageRi.setTexture(&stage_progression_active);
    mmStageR2i.setTexture(&stage_progression_active);
    mmStageL2i.setPosition(190*WINDOW_WIDTH/800, -25*WINDOW_HEIGHT/600);
    mmStageLi.setPosition(240*WINDOW_WIDTH/800, -30*WINDOW_HEIGHT/600);
    mmStagei.setPosition(300*WINDOW_WIDTH/800, -35*WINDOW_HEIGHT/600);
    mmStageRi.setPosition(390*WINDOW_WIDTH/800, -30*WINDOW_HEIGHT/600);
    mmStageR2i.setPosition(470*WINDOW_WIDTH/800, -25*WINDOW_HEIGHT/600);

    //draw indicator based on progression
    int progression = logic.getStageProgression();
    if (progression == -2)
        window.draw(mmStageL2i);
    else if (progression == -1)
        window.draw(mmStageLi);
    else if (progression == 0)
        window.draw(mmStagei);
    else if (progression == 1)
        window.draw(mmStageRi);
    else if (progression == 2)
        window.draw(mmStageR2i);

    //draw stage indicator veil
    /*
    stage_veil.setRadius(55);
    stage_veil.setFillColor(sf::Color(0, 0, 255, 100));
    stage_veil.setPosition(348.0f + (logic.getStageProgression() * (470 / 5.0f)), 600.0f - 95);
    window.draw(stage_veil);
    */

    // play any triggered sound effects
    if (logic.player_hit) {
        soundManager.playSound(SoundManager::SFX::bump, logic.player_hit);
        screenshake_magnitude = logic.player_hit;
        screenshake_timer = 1.0f;
        logic.player_hit = 0;
    }
    if (logic.player_bump) {
        soundManager.playSound(SoundManager::SFX::bump, logic.player_bump);
        screenshake_magnitude = logic.player_bump;
        screenshake_timer = 0.5f;
        logic.player_bump = 0;
    }
    if (logic.border_bump) {
        soundManager.playSound(SoundManager::SFX::bump, logic.player_bump);
        logic.border_bump = 0;
    }
    if(logic.splash){
      soundManager.playSound(SoundManager::SFX::splash, logic.splash);
      splash_timer = 0.5f;
      logic.splash = 0;
    }
    if(logic.powerup){
      soundManager.playSound(SoundManager::SFX::fish, logic.powerup);
      logic.powerup = 0;
    }

    //draw stamina boxes: these don't change
    sf::RectangleShape stamina_bar1 = sf::RectangleShape(sf::Vector2f(300*WINDOW_WIDTH/800, 25*WINDOW_HEIGHT/600));
    sf::RectangleShape stamina_bar2 = sf::RectangleShape(sf::Vector2f(300*WINDOW_WIDTH/800, 25*WINDOW_HEIGHT/600));
    stamina_bar1.setFillColor(sf::Color(255, 0, 0, 255));
    stamina_bar2.setFillColor(sf::Color(255, 0, 0, 255));
    stamina_bar1.setOutlineColor(sf::Color::Black);
    stamina_bar1.setOutlineThickness(2);
    stamina_bar2.setOutlineColor(sf::Color::Black);
    stamina_bar2.setOutlineThickness(2);
    stamina_bar1.setPosition(70*WINDOW_WIDTH/800, 60*WINDOW_HEIGHT/600);
    stamina_bar2.setPosition(stamina_bar1.getPosition().x + stamina_bar1.getSize().x + 70*WINDOW_WIDTH/800, 60*WINDOW_HEIGHT/600);

    //draw stamina gauges, these change
    //need to adjust dimensions to be based on walrus staminas
    //bar length is 300. Max stamina is 100
    sf::RectangleShape stamina_left1 = sf::RectangleShape(
            sf::Vector2f(stamina_bar1.getSize().x * logic.walrus1->getStamina()/100, stamina_bar1.getSize().y));
    sf::RectangleShape stamina_left2 = sf::RectangleShape(
            sf::Vector2f(stamina_bar1.getSize().x * logic.walrus2->getStamina()/100, stamina_bar2.getSize().y));
    stamina_left1.setFillColor(sf::Color(255, 255, 0, 255));
    stamina_left2.setFillColor(sf::Color(255, 255, 0, 255));
    stamina_left1.setPosition(stamina_bar1.getPosition());
    stamina_left2.setPosition(stamina_bar2.getPosition());

    // grey out bar if dead
    Player::PlayerState state1 = logic.walrus1->getState();
    Player::PlayerState state2 = logic.walrus2->getState();

    // gray out the stamina bar if dead or resting
    if (state1 == Player::PlayerState::dead) {
        stamina_bar1.setFillColor(sf::Color(50,50,50));
        stamina_left1.setFillColor(sf::Color(100,100,100));
    } else if (state1 == Player::PlayerState::resting) {
        stamina_bar1.setFillColor(sf::Color(100,100,100));
        stamina_left1.setFillColor(sf::Color(150,150,150));
    }
    if (state2 == Player::PlayerState::dead) {
        stamina_bar2.setFillColor(sf::Color(50,50,50));
        stamina_left2.setFillColor(sf::Color(100,100,100));
    } else if (state2 == Player::PlayerState::resting) {
        stamina_bar2.setFillColor(sf::Color(100,100,100));
        stamina_left2.setFillColor(sf::Color(150,150,150));
    }
    window.draw(stamina_bar1);
    window.draw(stamina_bar2);
    window.draw(stamina_left1);
    window.draw(stamina_left2);

    //draw round counter
    sf::RectangleShape roundCounterbg = sf::RectangleShape(sf::Vector2f(120,120));
    sf::RectangleShape roundCounterbg10 = sf::RectangleShape(sf::Vector2f(120,120));
    sf::RectangleShape roundCounterbg20 = sf::RectangleShape(sf::Vector2f(120,120));
    //roundCounterbg.setFillColor(sf::Color(0,0,0,255));
    roundCounterbg.setTexture(&roundCounter);
    roundCounterbg10.setTexture(&roundCounter10);
    roundCounterbg20.setTexture(&roundCounter20);
    roundCounterbg10.setTextureRect(roundCounter10_animation.uvRect);
    roundCounterbg20.setTextureRect(roundCounter20_animation.uvRect);
    roundCounterbg.setPosition((WINDOW_WIDTH/2)-52, 82);
    roundCounterbg10.setPosition((WINDOW_WIDTH/2)-52, 72);
    roundCounterbg20.setPosition((WINDOW_WIDTH/2)-52, 72);
    sf::Text round_text;
    round_text.setFont(font);
    round_text.setCharacterSize(UI_TEXT_SIZE / 3);
    round_text.setFillColor(sf::Color(255, 255, 255, 255));
    round_text.setPosition(roundCounterbg.getPosition().x+34,roundCounterbg.getPosition().y+20);
    round_text.setString("Round");
    sf::Text roundCounter_text;
    roundCounter_text.setFont(font);
    roundCounter_text.setCharacterSize(UI_TEXT_SIZE / 2);
    roundCounter_text.setFillColor(sf::Color(255, 255, 255, 255));

    // update round counter sprites
    if (logic.getRound() < 10) {
        roundCounter_text.setPosition(roundCounterbg.getPosition().x + 55, roundCounterbg.getPosition().y+30);
        window.draw(roundCounterbg);
    }else if (logic.getRound() <20) {
        roundCounter_text.setPosition(roundCounterbg.getPosition().x + 45, roundCounterbg.getPosition().y+30);
        window.draw(roundCounterbg10);
    } else{
        roundCounter_text.setPosition(roundCounterbg.getPosition().x + 45, roundCounterbg.getPosition().y+30);
        window.draw(roundCounterbg20);
    }
    roundCounter_text.setString(std::to_string(logic.getRound()));
    window.draw(roundCounter_text);
    window.draw(round_text);

    //draw walrus names above stamina bars
    walrus1_name.setString(logic.walrus1->getName());
    walrus2_name.setString(logic.walrus2->getName());
    walrus1_name.setFont(font);
    walrus2_name.setFont(font);
    walrus1_name.setCharacterSize(UI_TEXT_SIZE/3);
    walrus2_name.setCharacterSize(UI_TEXT_SIZE/3);
    walrus1_name.setFillColor(sf::Color(255,255,255,255));
    walrus1_name.setOutlineColor(sf::Color::Black);
    walrus1_name.setOutlineThickness(1);
    walrus1_name.setPosition(stamina_bar1.getPosition().x+5, stamina_bar1.getPosition().y-5);
    walrus2_name.setFillColor(sf::Color(255,255,255,255));
    walrus2_name.setOutlineColor(sf::Color::Black);
    walrus2_name.setOutlineThickness(1);
    walrus2_name.setPosition(stamina_bar2.getPosition().x+5, stamina_bar2.getPosition().y-5);
    window.draw(walrus1_name);
    window.draw(walrus2_name);

    // draw bot rays
    if (debug_mode) {
        if (!logic.walrus1->isDead()) {player1Controller->update(window, logic, 0, 1);}
        if (!logic.walrus2->isDead()) {player2Controller->update(window,logic,0,2);}
    }

}


void CameraView::menuUp(sf::RenderWindow &window, GameLogic &logic) {
    if (logic.getState() == GameLogic::GameState::mainMenu) {
        //track which menu option the player is on
        if (main_menu_selection == 'P' || main_menu_selection == 'H')
            main_menu_selection = 'P';
        else if (main_menu_selection == 'O')
            main_menu_selection = 'H';
    }
    if (logic.getState() == GameLogic::GameState::optionsMenu) {
        if (options_menu_selection == 'S' || options_menu_selection == 'M')
            options_menu_selection = 'S';
        else if (options_menu_selection == 'Q')
            options_menu_selection = 'M';
    }

    else if (logic.getState() == GameLogic::GameState::gameOverMenu) {
        if (game_over_menu_selection == 'P' || game_over_menu_selection == 'S')
            game_over_menu_selection = 'P';
        else if (game_over_menu_selection == 'Q')
            game_over_menu_selection = 'S';
    }

    else if (logic.getState() == GameLogic::GameState::playerSelectMenu) {
        if (player1_menu_selection == '1' || player1_menu_selection == '2')
            player1_menu_selection = player1_menu_selection;
        else if (player1_menu_selection == 'P')
            player1_menu_selection = '1';
        else if (player1_menu_selection == 'Q')
            player1_menu_selection = 'P';
    }
    else if (logic.getState() == GameLogic::GameState::pauseMenu) {
        if (pause_menu_selection == 'Q')
            pause_menu_selection = 'R';
    }
    soundManager.playSound(SoundManager::SFX::menuMove, logic.getSFXVolume());

}

void CameraView::menuDown(sf::RenderWindow &window, GameLogic &logic) {
    if (logic.getState() == GameLogic::GameState::mainMenu) {
        //track which menu option the player is on
        if (main_menu_selection == 'O' || main_menu_selection == 'H')
            main_menu_selection = 'O';
        else if (main_menu_selection == 'P')
            main_menu_selection = 'H';
    }

    if (logic.getState() == GameLogic::GameState::optionsMenu) {
        if (options_menu_selection == 'S')
            options_menu_selection = 'M';
        else if (options_menu_selection == 'M' || options_menu_selection == 'Q')
            options_menu_selection = 'Q';
    }

    else if (logic.getState() == GameLogic::GameState::gameOverMenu) {
        if (game_over_menu_selection == 'Q' || game_over_menu_selection == 'S')
            game_over_menu_selection = 'Q';
        else if (game_over_menu_selection == 'P')
            game_over_menu_selection = 'S';
    }

    else if (logic.getState() == GameLogic::GameState::playerSelectMenu) {
        if (player1_menu_selection == 'P')
            player1_menu_selection = 'Q';
        else if (player1_menu_selection == '1' || player1_menu_selection == '2')
            player1_menu_selection = 'P';
    }
    else if (logic.getState() == GameLogic::GameState::pauseMenu) {
        if (pause_menu_selection == 'R')
            pause_menu_selection = 'Q';
    }
    soundManager.playSound(SoundManager::SFX::menuMove, logic.getSFXVolume());
}

void CameraView::menuLeft(sf::RenderWindow &window, GameLogic &logic) {
    if (logic.getState() == GameLogic::GameState::optionsMenu) {
        if (options_menu_selection == 'S') {
            logic.setSFXVolume(logic.getSFXVolume() - 10);
        } else if (options_menu_selection == 'M') {
            logic.setMusicVolume(logic.getMusicVolume() - 10);
        }
    }
    if (logic.getState() == GameLogic::GameState::playerSelectMenu) {
        if (player1_menu_selection == '2')
            player1_menu_selection = '1';
    }
    if (logic.getState() == GameLogic::GameState::colorSelectSubMenu) {
        //std::cout << color_selection << std::endl;
        if (color_selection == '1' || color_selection == '2')
            color_selection = '1';
        if (color_selection == '3')
            color_selection = '2';
        if (color_selection == '4')
            color_selection = '3';
        if (color_selection == '5')
            color_selection = '4';
    }
    soundManager.playSound(SoundManager::SFX::menuMove, logic.getSFXVolume());
}

void CameraView::menuRight(sf::RenderWindow &window, GameLogic &logic) {
    if (logic.getState() == GameLogic::GameState::optionsMenu) {
        if (options_menu_selection == 'S') {
            logic.setSFXVolume(logic.getSFXVolume() + 10);
            //std::cout << logic.getSFXVolume();
        } else if (options_menu_selection == 'M') {
            logic.setMusicVolume(logic.getMusicVolume() + 10);
        }
    }
    if (logic.getState() == GameLogic::GameState::playerSelectMenu) {
        if (player1_menu_selection == '1')
            player1_menu_selection = '2';
    }
    if (logic.getState() == GameLogic::GameState::colorSelectSubMenu) {
        //std::cout << color_selection << std::endl;
        if (color_selection == '4' || color_selection == '5')
            color_selection = '5';
        if (color_selection == '3')
            color_selection = '4';
        if (color_selection == '2')
            color_selection = '3';
        if (color_selection == '1')
            color_selection = '2';
    }
    soundManager.playSound(SoundManager::SFX::menuMove, logic.getSFXVolume());
}

void CameraView::menuSelect(sf::RenderWindow &window, GameLogic &logic) {
    if (logic.getState() == GameLogic::GameState::mainMenu) {
        if (main_menu_selection == 'P') {
            logic.handlePlayerSelectMenu();
        } else if (main_menu_selection == 'H') {
            logic.handleHelpMenu();
        } else if (main_menu_selection == 'O') {
            logic.handleOptionsMenu();
        }
    } else if (logic.getState() == GameLogic::GameState::gameOverMenu) {
        if (game_over_menu_selection == 'P'){
            logic.handlePlayerSelectMenu();
            soundManager.playMusic(SoundManager::Music::title);
        }
        else if (game_over_menu_selection == 'S') {
            logic.handleStatsMenu();
            soundManager.playMusic(SoundManager::Music::title);
        }
        else {
            logic.returnToMenu();
            soundManager.playMusic(SoundManager::Music::title);
        }
    }
    else if (logic.getState() == GameLogic::GameState::helpMenu){
      logic.returnToMenu();
    }
    else if (logic.getState() == GameLogic::GameState::optionsMenu) {
        if (options_menu_selection == 'Q') {
            options_menu_selection = 'S';
            logic.returnToMenu();
        }
    } else if (logic.getState() == GameLogic::GameState::playerSelectMenu) {
        if (player1_menu_selection == 'P') {
            soundManager.playMusic(SoundManager::Music::battle);
            if (player1OrBot == -1 && player2OrBot == -1) {
                createControllers(0);
                logic.resetGame();
            } else if (player1OrBot == 1 && player2OrBot == -1) {
                createControllers(1);
                logic.resetGame();
            } else if (player1OrBot == -1 && player2OrBot == 1) {
                createControllers(2);
                logic.resetGame();
            } else {
                createControllers(3);
                logic.resetGame();
            }
        }
        if (player1_menu_selection == 'Q') {
            player1_menu_selection = 'P';
            logic.returnToMenu();
        }
        if (player1_menu_selection == '1') {
            player1OrBot = -player1OrBot;
            if (player1OrBot == 1) {
                //std::cout << "Entering Name" << std::endl;
                enteringNameText = true;
                logic.handleNameTextSubMenu();
                walrus1_name_str = "";
                walrus1_name.setString(walrus1_name_str);

            }
        }
        if (player1_menu_selection == '2') {
            player2OrBot = -player2OrBot;
            if (player2OrBot == 1) {
                //std::cout << "Entering Name" << std::endl;
                enteringNameText = true;
                logic.handleNameTextSubMenu();
                walrus2_name_str = "";
                walrus2_name.setString(walrus2_name_str);
            }
        }
    } else if (logic.getState() == GameLogic::GameState::pauseMenu) {
        if(pause_menu_selection == 'Q'){
          logic.returnToMenu();
          soundManager.playMusic(SoundManager::Music::title);
        } else if (pause_menu_selection == 'R') {
          logic.togglePause();
        }
    } else if (logic.getState() == GameLogic::GameState::statsMenu) {
        logic.returnToMenu();
    } else if (logic.getState() == GameLogic::GameState::nameTextSubMenu) {
        //std::cout << "Select Color" << std::endl;
        if (player1_menu_selection == '1') {
            if (walrus1_name_str.length() > 0) {
                logic.walrus1->setName(walrus1_name_str);
            } else {
                logic.walrus1->setName(DEFAULT_NAME_1);
            }
        }
        else {
            if (walrus2_name_str.length() > 0) {
                logic.walrus2->setName(walrus2_name_str);
            } else {
                logic.walrus2->setName(DEFAULT_NAME_2);
            }
        }
        enteringNameText = false;
        colorSelector = true;
        logic.handleColorSelectSubMenu();
    } else if (logic.getState() == GameLogic::GameState::colorSelectSubMenu) {
        colorSelector = false;
        logic.handlePlayerSelectMenu();
    }
    soundManager.playSound(SoundManager::SFX::menuSelect, logic.getSFXVolume());

}


void CameraView::processInput(sf::RenderWindow &window, GameLogic &logic, float dSec) {

    // update water
    water_animation.updateWater(dSec);

    // screen shake view transformation
    if (screenshake_timer > 0) {
        screenshake_timer -= dSec;
        sf::View view = window.getDefaultView();
        view.setCenter(view.getCenter() +
                       sf::Vector2f(rand() % (1 + (int) (screenshake_magnitude * screenshake_timer * 0.3)) - (int)(screenshake_magnitude * screenshake_timer * 0.3 / 2),
                                    rand() % (1 + (int) (screenshake_magnitude * screenshake_timer * 0.3)) - (int)(screenshake_magnitude * screenshake_timer * 0.3 / 2)));
        window.setView(view);
    } else {
        screenshake_timer = 0;
    }

    if (logic.getState() == GameLogic::GameState::playing) {
        // update fish animations
        for (auto anim = fish_animation_list.begin(); anim != fish_animation_list.end(); anim++) {
            (*anim)->updateFish(dSec);
        }

        // handle input in instantiated player controllers and update walrus animations
        if (!logic.walrus1->isDead()) {
            walrus1_animation.updateWalrus(logic.walrus1->getFacingDir(), logic.walrus1->getState(), dSec);
            player1Controller->update(window, logic, dSec, 1);
        }
        if (!logic.walrus2->isDead()) {
            walrus2_animation.updateWalrus(logic.walrus2->getFacingDir(), logic.walrus2->getState(), dSec);
            player2Controller->update(window, logic, dSec, 2);
        }

        //round counter flames
        roundCounter10_animation.updateRoundFire(dSec);
        roundCounter20_animation.updateRoundFire(dSec);

        //walrus splash
        walrusSplash_animation.updateWalrusSplash(dSec);
        if (splash_timer) {
            splash_timer -= dSec;
            if (splash_timer < 0) {
                splash_timer = 0;
            }
        }


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
                case sf::Event::KeyPressed:
                    // which key was pressed?
                    switch (Event.key.code) {
                        case sf::Keyboard::Up:
                            menuUp(window, logic);
                            break;
                        case sf::Keyboard::Down:
                            menuDown(window, logic);
                            break;
                        case sf::Keyboard::Right:
                            menuRight(window, logic);
                            break;
                        case sf::Keyboard::Left:
                            menuLeft(window, logic);
                            break;
                        case sf::Keyboard::Return:
                            menuSelect(window, logic);
                            break;
                        case sf::Keyboard::P:
                            logic.togglePause();
                            soundManager.playSound(SoundManager::SFX::menuSelect, logic.getSFXVolume());
                            break;
                        case sf::Keyboard::Tilde:
                            debug_mode = !debug_mode;
                            break;
                    }
                    break;
                // handle name input
                case sf::Event::TextEntered:
                    if (enteringNameText && isalnum(Event.text.unicode)) {
                        if (player1_menu_selection == '1' && walrus1_name_str.length() < MAX_INPUT_SIZE) {
                            walrus1_name_str += static_cast<char>(Event.text.unicode);
                        } else if (walrus2_name_str.length() < MAX_INPUT_SIZE) {
                            walrus2_name_str += static_cast<char>(Event.text.unicode);
                        }
                    }
                    break;
            }

        }
    }
}

void CameraView::createControllers(int players) {
    switch (players) {
        case 0:
            player1Controller = std::unique_ptr<Controller>(new BotController());
            player2Controller = std::unique_ptr<Controller>(new BotController());
            break;
        case 1:
            player1Controller = std::unique_ptr<Controller>(new PlayerController());
            player2Controller = std::unique_ptr<Controller>(new BotController());
            break;
        case 2:
            player1Controller = std::unique_ptr<Controller>(new BotController());
            player2Controller = std::unique_ptr<Controller>(new PlayerController());
            break;
        case 3:
            player1Controller = std::unique_ptr<Controller>(new PlayerController());
            player2Controller = std::unique_ptr<Controller>(new PlayerController());
            break;
    }
}
