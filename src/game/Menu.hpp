/*
 * Menu.hpp
 *
 *  Created on: May 11, 2013
 *      Author: Cam
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include "Button.hpp"
#include "TextBox.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#define CLICK_DELAY 0.0001

using namespace std;

class Menu {
public:
	Menu();
	virtual ~Menu();

	virtual void draw(sf::RenderWindow* window);
	virtual int update(sf::Vector2i mouseLoc, sf::Event event);
	virtual void addButton(float x, float y, const char* path, int action);
	virtual void addTextBox(float x, float y, float pWidth);

private:
	// Holds all the buttons
	vector<Button*> buttons;

	// Holds all the textboxes
	vector<TextBox*> TextBoxes;

	// Timer
	static sf::Clock reset;
};

#endif /* MENU_HPP_ */
