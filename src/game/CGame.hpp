/*
 * CGame.hpp
 *
 *  Created on: May 11, 2013
 *      Author: Cam
 */

#ifndef CGAME_HPP_
#define CGAME_HPP_

#include "Menu.hpp"
#include "CGame_shared.hpp"

enum gameState {

	INIT, MENU, LOGIN, CONNECT, PLAYING, PAUSE, QUIT

};

#include <SFML/Graphics.hpp>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAMES_PER_SECOND 60

class CGame {
public:
	CGame();
	virtual ~CGame();

	virtual void init();
	virtual void run();
	virtual void update();

	void showMenu();

	int getState() const;
	void setState(int state);

private:

	// Background drawable object
	sf::RectangleShape background;
	// Background texture
	sf::Texture backgroundImage;
	// State of the game
	int state;
	// Window of the game
	sf::RenderWindow window;
	// Game menu
	Menu gameMenu;
	// Window icon :D
	sf::Image icon;

};

#endif /* CGAME_HPP_ */
