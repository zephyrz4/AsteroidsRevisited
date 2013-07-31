/*
 * CGame.cpp
 *
 *  Created on: May 11, 2013
 *      Author: Cam
 */

#include "CGame.hpp"
#include <iostream>
#include <string.h>
#include "MD5.h"
#include <SFML/Network.hpp>
#include "../server/Login.hpp"

CGame::CGame() {
	setState(INIT);
}

CGame::~CGame() {
	// TODO Auto-generated destructor stub
}

void CGame::init() {

	if (!icon.loadFromFile("images/icon.png"))
		return;

	// Init the background.
	background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	if (!backgroundImage.loadFromFile("images/background.png"))
		return; // Error message here.

	background.setTexture(&backgroundImage);

	// Init the Main Menu
	gameMenu.addButton(50, 0, "images/title.gif", BUT_NOTHING);
	gameMenu.addButton(283.5, 100, "images/sub2.gif", BUT_NOTHING);
	gameMenu.addButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "images/test.png",
			BUT_PLAY);

	// Init login menu
	loginMenu.addButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "images/login.png",
			BUT_LOGIN);
	loginMenu.addTextBox(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 60, 150);
	loginMenu.addTextBox(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 30, 150);

	setState(MENU);

}

void CGame::run() {

	// Init window
	window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids",
			sf::Style::Titlebar | sf::Style::Close);
	window.setIcon(96, 96, icon.getPixelsPtr());
	window.setFramerateLimit(FRAMES_PER_SECOND);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		switch (getState()) {
		case INIT:
			// Error message here.
			break;
		case MENU:

			// Draw the background.
			window.clear();
			window.draw(background);
			// Draw menu buttons.
			handleButton(gameMenu.update(sf::Mouse::getPosition(window),event));

			gameMenu.draw(&window);
			break;

		case LOGIN:
			window.clear();
			window.draw(background);
			handleButton(loginMenu.update(sf::Mouse::getPosition(window),event));

			loginMenu.draw(&window);
			break;

		case NETCONNECT:
			window.clear();
			netConnect();


			break;
		case PLAYING:
			window.clear();
			window.draw(background);
			break;
		case PAUSE:
			break;
		case QUIT:
			return;
			break;
		default:
			// Error message here.
			break;
		}

		window.display();

	}

}

void CGame::update() {
}

int CGame::getState() const {
	return state;
}

void CGame::showMenu() {
}

void CGame::setState(int state) {
	this->state = state;
}

void CGame::handleButton(int action) {

	switch (action) {

	case BUT_PLAY:
		setState(LOGIN);
		break;
	case BUT_LOGIN:
		setState(NETCONNECT);
		break;
	case BUT_QUIT:
		setState(QUIT);
		break;

	}

	sf::RectangleShape test;
}

void CGame::netConnect() {

	sf::Packet packet;
	loginPacket info;
	std::string tmp ;
	short packetType;
	short response;

	std::cout << "Sending login..." << std::endl;

	// FIXME get this from a config or something
	gameClient.setServerAddress("127.0.0.1");
	gameClient.setServerPort(SERVER_PORT);

	// Setup login packet
	info.packetType = GAMELOGIN;
	strcpy(info.username,loginMenu.getTextBoxes().at(0)->toString().c_str());
	tmp = loginMenu.getTextBoxes().at(1)->toString() + "coolsalt";
	strcpy(info.password,md5(tmp).c_str());
	packet << info;

	// Send login packet to server
	if( gameClient.send(packet) ) {
		std::cout << "failed to login" << std::endl;
		setState(QUIT);
	}

	// Wait from response
	packet.clear();
	std::cout << "Waiting for server response...";

	while( gameClient.receive(&packet) != sf::Socket::Done ) {
	}

	packet >> packetType;

	if( packetType != GAMELOGINRESPONSE ) {

		setState(QUIT);
		return;
	}

	packet >> response;

	switch( response ) {

	case AUTH_VALID:
		std::cout << "Logged in!" << std::endl;
		setState(PLAYING);
		return;
		break;
	case AUTH_UNKNOWN_USER:
		setState(LOGIN);
		return;
		break;
	case AUTH_INVALID_PASSWORD:
		setState(LOGIN);
		return;
		break;
	}

	setState(LOGIN);




}
