#include "Player.h"


Player::Player() : m_id(-1), isTagged(false)
{
    srand(time(nullptr));
	Init();
	m_velocity = sf::Vector2f(0.0f, 0.0f);
}

Player::~Player()
{
}

void Player::update(sf::Time deltaTime)
{
	movement(deltaTime);
}

void Player::render(sf::RenderWindow& m_window)
{
	m_window.draw(m_circle);
}

void Player::Init()
{
	m_circle.setRadius(20.0f);
	m_circle.setFillColor(sf::Color::Green);
	m_circle.setPosition(rand() % 600 + 100, rand() % 400 + 100);
    m_circle.setPointCount(5);
}

void Player::movement(sf::Time deltaTime)
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_velocity.y = -playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_velocity.x = -playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_velocity.y = playerSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_velocity.x = playerSpeed;
    }

    sf::Vector2f displacement = m_velocity * deltaTime.asSeconds();
    m_circle.move(displacement);
}
