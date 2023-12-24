#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{800U,600U,32U}, "Tag Network Game"},
	m_exitGame{false}
{
	port = 45000;
	ipAddress = "localhost";
	client = std::make_unique<Client>(ipAddress, port);
	
	Init();

	socket.setBlocking(false);
}

Game::~Game()
{
	client->disconnectFromGame(&m_player);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type)
		{
			processKeys(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	sf::Vector2f newPosition = m_player.getPosition();
	if (newPosition != lastSentPosition)
	{
		lastSentPosition = newPosition;
		client->sendPlayer(&m_player);
	}

	client->receivePlayer(enemies, &m_player);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setPosition(enemies[i]->m_circle.getPosition());
		if (&m_player.isTagged && checkCollision(m_player, *enemies[i]))
		{
			if (!enemies[i]->isTagged && enemies[i]->collisionCooldown.getElapsedTime() > enemies[i]->cooldownDuration)
			{
				enemies[i]->isTagged = true;
				m_player.isTagged = false;

				enemies[i]->cooldownDuration = sf::seconds(3.0f);
				enemies[i]->collisionCooldown.restart();
			}
		}
		if (enemies[i]->isTagged && enemies[i]->collisionCooldown.getElapsedTime() < enemies[i]->cooldownDuration)
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Yellow);
			int remainingTime = static_cast<int>(enemies[i]->cooldownDuration.asSeconds() - enemies[i]->collisionCooldown.getElapsedTime().asSeconds());
			static int previousTime = -1;

			if (remainingTime != previousTime)
			{
				std::cout << "Can't catch player, wait: '" << remainingTime << "' seconds." << std::endl;
				previousTime = remainingTime;
			}
		}
	}

	if (m_window.hasFocus())
	{
		m_player.update(t_deltaTime);
	}
	checkTags();
	wrapAround(m_player);
}

void Game::render()
{
	m_window.clear(sf::Color::White);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw(m_window);

		if (enemies[i]->isTagged)
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Red);
		}
		else
		{
			enemies[i]->m_circle.setFillColor(sf::Color::Green);
		}
	}

	if (m_player.isTagged)
	{
		m_player.m_circle.setFillColor(sf::Color::Red);
	}
	else
	{
		m_player.m_circle.setFillColor(sf::Color::Green);
	}
	m_player.render(m_window);
	m_window.display();
}

void Game::Init()
{
	std::cout << "Enter your name: ";
	std::cin >> m_name;
	m_player.setName(m_name);
}

void Game::checkTags()
{
	m_player.isTagged = true;

	for (const auto& enemy : enemies)
	{
		if (enemy->isTagged)
		{
			m_player.isTagged = false;
			return;
		}
	}
}

bool Game::checkCollision(Player& player, Enemy& enemy)
{
	sf::FloatRect playerBounds = player.m_circle.getGlobalBounds();
	sf::FloatRect enemyBounds = enemy.m_circle.getGlobalBounds();

	if (playerBounds.intersects(enemyBounds))
	{
		//std::cout << "Enemy Tagged" << std::endl;
		return true;
	}
	return false;
}

void Game::wrapAround(Player& player)
{
	sf::Vector2f playerPos = player.getPosition();
	float playerRadius = player.getRadius();

	sf::Vector2u windowSize = m_window.getSize();

	if (playerPos.x + playerRadius < 0.0f)
	{
		player.m_circle.setPosition(windowSize.x + playerRadius, playerPos.y);
	}
	else if (playerPos.x - playerRadius > windowSize.x)
	{
		player.m_circle.setPosition(-playerRadius, playerPos.y);
	}

	if (playerPos.y + playerRadius < 0.0f)
	{
		player.m_circle.setPosition(playerPos.x, windowSize.y + playerRadius);
	}
	else if (playerPos.y - playerRadius > windowSize.y)
	{
		player.m_circle.setPosition(playerPos.x, -playerRadius);
	}
}
