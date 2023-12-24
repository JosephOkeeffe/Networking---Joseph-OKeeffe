#include "Client.h"

Client::Client(sf::IpAddress& ip, unsigned short& port)  : m_connected(false)
{
	if (connection.connect(ip, port, sf::seconds(5)) != sf::Socket::Done)
	{
		_Output_Text("Error: Cant connect to server")
	}
	else
	{
		connection.setBlocking(false);
		_Output_Text("Connected to server")
	}
}

void Client::disconnectFromGame(Player* p)
{
	sf::Packet temp;
	temp << 1;
	temp << p->getID();

	if (connection.send(temp) != sf::Socket::Done)
	{
		_Output_Text("Error: Cant send disconnect command to server")
	}
	else
	{
		_Output_Text("Disconnected from the game")
	}
}

void Client::sendPlayer(Player* p)
{
	sf::Packet temp;
	temp << 3;
	temp << p->getID();
	temp << p->getPosition().x;
	temp << p->getPosition().y;
	temp << p->getPlayerTag();

	if (connection.send(temp) != sf::Socket::Done)
	{
		_Output_Text("Error: Cant send player data to the server")
	}
}

void Client::sendMessage(Player* p, std::string& text)
{
	sf::Packet temp;
	temp << 4;
	temp << p->getID();
	temp << text;

	if (text.length() > 2)
	{
		if (connection.send(temp) != sf::Socket::Done)
		{
			_Output_Text("Error: Cant send text message to the server")
		}
	}
}

void Client::sendMyName(Player* p)
{
	sf::Packet temp;
	temp << 5;
	temp << p->getID();
	temp << p->getName();

	if (connection.send(temp) != sf::Socket::Done)
	{
		_Output_Text("Error: Cant send name to the server")
	}

}

void Client::getPlayerList(Player* p)
{
	sf::Packet temp;
	temp << 6;
	temp << p->getID();

	if (connection.send(temp) != sf::Socket::Done)
	{
		_Output_Text("Error: Cant send Player List to the server")
	}
}

void Client::receivePlayer(std::vector<std::unique_ptr<Enemy>>& enemies, Player* p)
{
	sf::Packet receivePacket;
	int type, id;

	if (connection.receive(receivePacket) == sf::Socket::Done)
	{
		receivePacket >> type;
		receivePacket >> id;

		if (type == 0) 
		{
			if (p->getID() == -1)
			{
				p->setID(id);
				std::cout << "ID: " << p->getID() << std::endl;
				this->sendMyName(p);
				sf::sleep(sf::milliseconds(50));
				this->getPlayerList(p);
			}
			m_connected = true;
		}
		else if (type == 1) 
		{
			for (unsigned int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->getID() == id)
				{
					m_textMessage = "Player " + enemies[i]->getName() + " has disconnected from the game";
					std::cout << "Enemy: " << enemies[i]->getID() << " has been deleted" << std::endl;
					enemies.erase(enemies.begin() + i);
				}
			}
		}
		else if (type == 2)
		{
			_Output_Text("The server is full")
		}
		else if (type == 3) 
		{
			for (unsigned int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->getID() == id)
				{
					sf::Vector2f pos;
					bool isNewTagged;
					receivePacket >> pos.x;
					receivePacket >> pos.y;
					receivePacket >> isNewTagged;
					enemies[i]->setPosition(pos);
					enemies[i]->setTagged(isNewTagged);
					break;
				}
			}
		}
		else if (type == 4) 
		{
			std::string receivedMessage;
			receivePacket >> receivedMessage;

			if (!receivedMessage.empty())
			{
				std::string senderName;
				for (unsigned int k = 0; k < enemies.size(); k++)
				{
					if (enemies[k]->getID() == id)
					{
						senderName = enemies[k]->getName();
					}
				}
				if (id == p->getID())
				{
					senderName = p->getName();
				}
				std::string testMessage(receivedMessage);
				std::string newString = testMessage.substr(0, testMessage.length() - 1);
				m_textMessage = senderName + ":" + newString;
			}
		}

		else if (type == 6)
		{
			int playerNumber;
			std::vector<std::string> playersName;
			std::vector<int> playersId;

			receivePacket >> playerNumber;
			std::cout << "Current players on the server: " << playerNumber << std::endl;

			for (int i = 0; i < playerNumber; ++i)
			{
				std::string tempName;
				int tempId;
				receivePacket >> tempId;
				receivePacket >> tempName;
				playersName.push_back(tempName);
				playersId.push_back(tempId);
			}

			for (unsigned int i = 0; i < playersId.size(); ++i) 
			{
				bool haveThatEnemy = false;
				for (unsigned int v = 0; v < enemies.size(); v++) 
				{
					if (enemies[v]->getID() == playersId[i])
					{
						haveThatEnemy = true;
					}
				}
				if (playersId[i] != p->getID() && !haveThatEnemy) 
				{
					enemies.push_back(std::make_unique<Enemy>(playersId[i], sf::Vector2f(100, 100), playersName[i]));
					m_textMessage = "A new player connected: " + playersName[i];
					std::cout << "Created a new enemy with id: " << playersId[i] << std::endl;
				}
			}
			playersName.clear();
			playersId.clear();
		}
		else if (type == 7)
		{
			int playerID;
			bool isTagged;
			receivePacket >> playerID >> isTagged;

			for (const auto& enemy : enemies)
			{
				if (enemy->getID() == playerID)
				{
					enemy->setTagged(isTagged);
					break;
				}
			}
		}
	}
}
