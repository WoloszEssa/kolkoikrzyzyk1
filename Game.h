#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
class Game
{
	bool playerTurn;
	int gameEnded;
	Board plansza;
	bool gameStarted;
	bool pinged;
	int licznik;
	bool gameMode;
	int nextMoveX;
	int nextMoveY;

public:
	void run();
	Game();
	bool handlePlayerAttack(int mouseX, int mouseY, bool choice);
	bool isOnBoard(int x, int y);
	void handleBotAttack();
	void findNextMove();
	void drawEndGame(sf::RenderWindow& window);
	void drawStartGame(sf::RenderWindow& window, int licznikEkranow);
	bool canWin(int x, int y, int choice);
	bool playerCanWin(int x, int y, int choice);
	void botTileNextToPlayer(int x, int y, int choice);
	void Reset();
	void handleStartGame(sf::Event& event);
	bool hasNeighbor(int x, int y, int choice);
};

