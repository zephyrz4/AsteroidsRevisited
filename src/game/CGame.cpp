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

CGame::CGame() {
	setState(INIT);
}

CGame::~CGame() {
	// TODO Auto-generated destructor stub
}

void CGame::init() {

	// Init the background.
	background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	background.setTexture(ResourceHandler.loadTexture("images/background.png"));

	// Init the Main Menu
	gameMenu.addButton(50, 0, ResourceHandler.loadTexture("images/title.gif"),
			BUT_NOTHING);
	gameMenu.addButton(283.5, 100,
			ResourceHandler.loadTexture("images/sub2.gif"), BUT_NOTHING);
	gameMenu.addButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			ResourceHandler.loadTexture("images/test.png"), BUT_PLAY);
	gameMenu.addButton(SCREEN_WIDTH-50,SCREEN_HEIGHT-50,ResourceHandler.loadTexture("images/quit.png"),BUT_QUIT);

	// Init login menu
	loginMenu.addButton(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			ResourceHandler.loadTexture("images/login.png"), BUT_LOGIN);
	loginMenu.addButton(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2,
			ResourceHandler.loadTexture("images/register.png"), BUT_REGISTER);
	loginMenu.addTextBox(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 60, ResourceHandler.loadFont("fonts/TitilliumWeb-Regular.ttf"), 150);
	loginMenu.addTextBox(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 30, ResourceHandler.loadFont("fonts/TitilliumWeb-Regular.ttf"), 150);
	loginMenu.addButton(SCREEN_WIDTH-50,SCREEN_HEIGHT-50,ResourceHandler.loadTexture("images/quit.png"),BUT_QUIT);
	// Init networking
	gameClient.setServerAddress("127.0.0.1");
	gameClient.setServerPort(SERVER_PORT);

	setState(MENU);

}

void CGame::run() {

	// Init window
	window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids",
			sf::Style::Titlebar | sf::Style::Close);
	window.setIcon(96, 96, ResourceHandler.loadImage("images/icon.png")->getPixelsPtr());
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
			handleButton(
					gameMenu.update(sf::Mouse::getPosition(window), event));

			gameMenu.draw(&window);
			break;

		case LOGIN:
			window.clear();
			window.draw(background);
			handleButton(
					loginMenu.update(sf::Mouse::getPosition(window), event));

			loginMenu.draw(&window);
			break;

		case PLAYING:
			receiveServerUpdate();
			gameWorld.update(event);
			window.clear();
			window.draw(background);
			gameWorld.draw(&window);
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

void CGame::receiveServerUpdate() {

	sf::Packet serverPacket;

	gameClient.receive(&serverPacket);

	gameWorld.packetToWorld(serverPacket);
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
		login();
		break;
	case BUT_REGISTER:
		gameRegister();
		break;
	case BUT_QUIT:
		setState(QUIT);
		break;

	}

	sf::RectangleShape test;
}

void CGame::login() {

	sf::Packet packet;
	loginPacket info;
	std::string tmp;
	short packetType;

	std::cout << "Sending login..." << std::endl;

	// Setup login packet
	info.packetType = GAMELOGIN;
	strcpy(info.username, loginMenu.getTextBoxes().at(0)->toString().c_str());
	tmp = loginMenu.getTextBoxes().at(1)->toString() + "coolsalt";
	strcpy(info.password, md5(tmp).c_str());
	packet << info;

	// Send login packet to server
	if (gameClient.send(packet)) {
		std::cout << "failed to login" << std::endl;
		setState(QUIT);
	}

	// Wait from response
	packet.clear();
	std::cout << "Waiting for server response...";

	while (gameClient.receive(&packet) != sf::Socket::Done) {
	}

	packet >> packetType;

	switch (packetType) {

	case LOGIN_AUTH_VALID:
		std::cout << "Logged in!" << std::endl;
		setState(PLAYING);
		return;
		break;
	case LOGIN_AUTH_UNKNOWN_USER:
		std::cout << "Invalid user!" << std::endl;
		setState(LOGIN);
		return;
		break;
	case LOGIN_AUTH_INVALID_PASSWORD:
		std::cout << "Unknown password!" << std::endl;
		setState(LOGIN);
		return;
		break;
	}

	setState(LOGIN);

}

void CGame::draw(sf::RenderWindow* window) {
}

void CGame::gameRegister() {

	sf::Packet packet;
	loginPacket info;
	std::string tmp;
	short packetType;

	std::cout << "Sending register..." << std::endl;

	// Setup login packet
	info.packetType = GAMEREGISTER;
	strcpy(info.username, loginMenu.getTextBoxes().at(0)->toString().c_str());
	tmp = loginMenu.getTextBoxes().at(1)->toString() + "coolsalt";
	strcpy(info.password, md5(tmp).c_str());
	packet << info;

	// Send login packet to server
	if (gameClient.send(packet)) {
		std::cout << "failed to login" << std::endl;
		setState(QUIT);
	}

	// Wait from response
	packet.clear();
	std::cout << "Waiting for server response...";

	while (gameClient.receive(&packet) != sf::Socket::Done) {
	}

	packet >> packetType;

	switch (packetType) {

	case LOGIN_REG_SUCCESS:
		std::cout << "Registered & Logged in!" << std::endl;
		setState(PLAYING);
		return;
		break;
	case LOGIN_REG_INUSE:
		std::cout << "User already in use!" << std::endl;
		setState(LOGIN);
		return;
		break;
	}

	setState(LOGIN);

}
