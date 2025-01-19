#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Board
{
	const int windowSize = 800;
	int gridSize;
	int cellSize;
	const int offSet = 100;
	int howManyToWin;	
	std::vector<std::vector<int>> grid;

public:
	void boardDraw(sf::RenderWindow& window);
	Board();
	void attack(int x, int y, bool choice);

	int offset();
	int cellsize();
	int getTileStatus(int x, int y);
	int gridsize();
	std::vector<std::tuple<int, int>> getAttackableTiles();
	int WinCheck();
	bool checkAround(int x, int y, int choice);
	bool isOnBoard(int x, int y);
	int getHowManyToWin();
	void clearTile(int x, int y);
	void setGrid(int size, int hM);
	void boardPing();
	void clear();

	
};


