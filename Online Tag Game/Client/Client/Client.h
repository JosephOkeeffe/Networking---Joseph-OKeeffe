#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <memory>

class Client
{
public:
	Client(sf::IpAddress& ip, unsigned short& port);
	void disconnectFromGame(Player* p);

	void sendPlayer(Player* p);
	void receivePlayer(std::vector<std::unique_ptr<Enemy>>& enemies, Player* p);
	void sendMessage(Player* p, std::string& text);
	void sendMyName(Player* p);
	void getPlayerList(Player* p);

	std::string getMessage(){ return m_textMessage; }

private:
	sf::TcpSocket connection;
	bool m_connected;

	std::string m_textMessage;
	int packetCounter = 0;
	sf::Clock packetClock;
};

