
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Player.h"

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
	void init();
	

	sf::RenderWindow m_window; 
	sf::Font m_ArialBlackfont; 

	Player player;

	bool m_exitGame; 

};

#endif 

