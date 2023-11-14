#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:

	void Init();
	void Render(sf::RenderWindow& window);
	void Update();

	sf::CircleShape shape;
};

