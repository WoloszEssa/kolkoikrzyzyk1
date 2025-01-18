#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
class Game
{
	bool playerTurn;
	int gameEnded;
	Board plansza;
	bool gameStarted;

public:
	void run();
	Game();
	bool handlePlayerAttack(int mouseX, int mouseY);
	bool isOnBoard(int x, int y);
	void handleBotAttack();
	void drawEndGame(sf::RenderWindow& window);
	void drawStartGame(sf::RenderWindow& window);
};

