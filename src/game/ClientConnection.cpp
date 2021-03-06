/*
 * Client.cpp
 *
 *  Created on: May 9, 2013
 *      Author: cam
 */

#include "ClientConnection.hpp"
#include "../shared/GameGlobals.hpp"
#include <iostream>

ClientConnection::ClientConnection() :
		connection(PROTOCOL, TIMEOUT) {

	Reset();
}

ClientConnection::~ClientConnection() {
	// TODO Auto-generated destructor stub
}

bool ClientConnection::send(sf::Packet data) {

	return connection.SendPacket(data);

}

void ClientConnection::Connect(sf::IpAddress ip) {

	if (!connection.isRunning()) {

		connection.Connect(ip);

	}

}

int ClientConnection::receive(sf::Packet *data) {

	return connection.ReceivePacket(data);
}

void ClientConnection::update(float delta) {
	connection.Update(delta);
}

void ClientConnection::printStats() {

	std::cout << connection.getReliabilitySystem().toString() << std::endl;

}

float ClientConnection::getRTT() {
	return connection.getReliabilitySystem().GetRoundTripTime() * 1000;
}

void ClientConnection::Reset() {
	connection.Stop();
	connection.setRemotePort(SERVER_PORT);
	connection.setProtocolId(PROTOCOL);
	connection.setTimeout(TIMEOUT);
	connection.Start();
}
