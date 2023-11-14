#include "Player.h"

void Player::Init()
{
	shape.setFillColor(sf::Color::Green);
	shape.setRadius(25);
	shape.setPosition(200, 200);
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}

void Player::Update()
{
}
