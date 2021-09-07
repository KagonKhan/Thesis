#pragma once

#include "State.h"
#include "AnimatedSprite.h"
class Game
{
private:
	sf::RenderWindow m_Window;
	std::stack<State::ptr> m_States;
	sf::Event sfevent;

	AnimatedSprite m_Cursor;

	void initWindow();
	void initStates();
	void initCursor();
	
	void processEvents();

	void update(const sf::Time& deltaTime);
	void updateMouse(const sf::Time& deltaTime);
	void updateStates(const sf::Time& deltaTime);

	void render();
	void renderMouse();

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Game();
	~Game();
	void run(int minFPS);
};

