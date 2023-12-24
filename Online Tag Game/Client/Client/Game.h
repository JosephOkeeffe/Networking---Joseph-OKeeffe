#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Client.h"
#include "Player.h"
#include "Enemy.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void Init();

	void checkTags();
	bool checkCollision(Player& player, Enemy& enemy);
	void wrapAround(Player& player);

	unsigned short port;
	sf::IpAddress ipAddress;
	sf::TcpSocket socket;

	sf::Clock m_clock; 
	sf::Time m_positionTimer;

	Player m_player;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::unique_ptr<Client> client;

	std::string m_name;
	sf::Vector2f lastSentPosition;

	sf::RenderWindow m_window;

	bool m_exitGame;
};

#endif