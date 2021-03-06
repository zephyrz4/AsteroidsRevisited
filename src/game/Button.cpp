/*
 * Button.cpp
 *
 *  Created on: May 11, 2013
 *      Author: David Jelley
 */

#include "Button.hpp"

Button::Button(float x, float y, sf::Texture* texture, int action) {
	setPosition(x, y);
	setAction(action);

	sprite.setTexture(*texture);

}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

void Button::draw(sf::RenderWindow* window) {
	window->draw(sprite);
}

void Button::setPosition(float x, float y) {

	this->sprite.setPosition(x, y);
}

void Button::animate(int color) {

	// Don't animate if this button does nothing
	if (getAction() == BUT_NOTHING)
		return;

	switch (color) {

	case NORMAL:
		this->sprite.setColor(sf::Color(255, 255, 255));
		break;
	case OVER:
		this->sprite.setColor(sf::Color(255, 255, 0));
		break;
	case CLICK:
		this->sprite.setColor(sf::Color(0, 255, 0));
		break;

	}

}
int Button::update(sf::Vector2f mouseLoc, sf::Event event) {

	if (sprite.getGlobalBounds().contains(mouseLoc)) {

		if (event.type == sf::Event::MouseButtonReleased) {
			animate(CLICK);
			return action;
		} else {
			animate(OVER);
			return BUT_NOTHING;
		}

	} else {

		animate(NORMAL);

		return BUT_NOTHING;
	}

}

int Button::getAction() const {
	return action;
}

void Button::setAction(int action) {
	this->action = action;
}

bool Button::isPressed() const {
	return pressed;
}

void Button::setPressed(bool pressed) {
	this->pressed = pressed;
}
