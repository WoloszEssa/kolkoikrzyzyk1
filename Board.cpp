#include "Board.h"
#include <SFML/Graphics.hpp>
Board::Board()
{
	grid.resize(gridSize);
	for (int i = 0; i < gridSize; ++i) 
    {
		grid[i].resize(gridSize);
		for (int j = 0; j < gridSize; ++j)
		{
			grid[i][j] = 0;
		}
	}
}
Board::Board(Board& board) //konstruktor kopiujacy (mial byc uzywany przy logice bota)
{
	gridSize = board.gridSize;
	cellSize = board.cellSize;
	howManyToWin = board.howManyToWin;
	grid = board.grid;
	playerSymbol = board.playerSymbol;
}
Board::~Board()
{
	grid.resize(gridSize);
	for (int i = 0; i < gridSize; ++i)
	{
		grid[i].resize(gridSize);
		for (int j = 0; j < gridSize; ++j)
		{
			grid[i][j] = 0;
		}
	}
}
void Board::clear() //czyszczenie planszy
{
	grid.clear();
	grid.resize(gridSize);
	for (int i = 0; i < gridSize; ++i)
	{
		grid[i].resize(gridSize);
		for (int j = 0; j < gridSize; ++j)
		{
			grid[i][j] = 0;
		}
	}

}

void Board::boardDraw(sf::RenderWindow& window)
{
	float cellOffset = 0.13 * cellSize;//odstep od krawedzi komorki
    for (int x = 0; x < gridSize; ++x)
    {
        for (int y = 0; y < gridSize; ++y) 
        {
            sf::RectangleShape cell(sf::Vector2f(cellSize- 2, cellSize - 2));
            cell.setPosition(offSet + x * cellSize, offSet + y * cellSize);
			sf::Texture texture;
			sf::Sprite sprite;
			if(playerSymbol) //gracz wybral kolko
			{
				if (grid[x][y] == 1)
				{
					texture.loadFromFile("kolko.png");
					sprite.scale(0.007 * cellSize, 0.007 * cellSize);
					sprite.setTexture(texture);
					sprite.setPosition(offSet + cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
				}
				else if (grid[x][y] == 2)
				{
					texture.loadFromFile("krzyzyk.png");
					sprite.scale(0.007 * cellSize, 0.007 * cellSize);
					sprite.setTexture(texture);
					sprite.setPosition(offSet + cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
				}

			}
			else //gracz wybral krzyzyk
			{
				if (grid[x][y] == 1)
				{
					texture.loadFromFile("krzyzyk.png");
					sprite.scale(0.007 * cellSize, 0.007 * cellSize);
					sprite.setTexture(texture);
					sprite.setPosition(offSet + cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
				}
				else if (grid[x][y] == 2)
				{
					texture.loadFromFile("kolko.png");
					sprite.scale(0.007 * cellSize, 0.007 * cellSize);
					sprite.setTexture(texture);
					sprite.setPosition(offSet + cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
				}

			}
			if (grid[x][y] == 11) //czyszczenie tej jednej komorki zeby sie wyswietlila plansza
			{
			texture.loadFromFile("puste.png");
			sprite.scale(0.007 * cellSize, 0.007 * cellSize);
			sprite.setTexture(texture);
			sprite.setPosition(offSet + cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
			}

           
            window.draw(cell);
			window.draw(sprite);
        }
    }

}
void Board::attack(int x, int y, bool choice) //postawienie pionka
{
	if (getTileStatus(x, y) == 0)
	{
		if (choice)
		{
			grid[x][y] = 1;
		}
		else
		{
			grid[x][y] = 2;
		}
	}
}
void Board::boardPing() 
{
	grid[0][0] = 11;
	
}

void Board::playerSymbolChoose(bool choice)
{
	playerSymbol = choice;
}

int Board::offset()
{
	return offSet;
}

int Board::cellsize()
{
	return cellSize;
}

int Board::getTileStatus(int x, int y) //sprawdzenie statusu komorki
{
	return grid[x][y];
}
bool Board::getSymbol()
{
	return playerSymbol;
}
int Board::gridsize()
{
	return gridSize;
}	
int Board::getHowManyToWin()
{
	return howManyToWin;
}
void Board::clearTile(int x, int y) //czyszczenie komorki (mialo byc uzywane przy logice bota)
{
	grid[x][y] = 0;
}


void Board::setGrid(int size, int hM) //ustawienie planszy
{
    gridSize = size;
    cellSize = (windowSize / gridSize) / 2;
	howManyToWin = hM;
    grid.clear();
    grid.resize(gridSize);
    for (int i = 0; i < gridSize; ++i) {
        grid[i].resize(gridSize);
    }

}

std::vector<std::tuple<int, int>> Board::getAttackableTiles() //zwraca wolne komorki
{
	std::vector<std::tuple<int, int>> attackableTiles;
	for (int x = 0; x < gridSize; ++x)
	{
		for (int y = 0; y < gridSize; ++y)
		{
			if (getTileStatus(x,y) == 0)
			{
				attackableTiles.push_back(std::make_tuple(x, y));
			}
		}
	}
	return attackableTiles;
}

int Board::WinCheck() //sprawdzenie czy ktos wygral
{
	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			if (getTileStatus(i, j) != 0)
			{
				if (checkAround(i, j, getTileStatus(i, j)))
				{
					return getTileStatus(i, j); 
				}
				
			}
			
		}
	}
	if (getAttackableTiles().size() == 0) //remis
	{
		return 3;
	}
	
}

bool Board::isOnBoard(int x, int y) //sprawdza czy komorka jest na planszy
{
	return (x >= 0 && x < gridsize() && y >= 0 && y < gridsize());
}


bool Board::checkAround(int x, int y, int choice) //sprawdza czy ktos wygral
{
	//wystarczy ze sprawdza czy jest howManyToWin komórek w prawo, dó³, skos w prawo, skos w lewo 
	int j=howManyToWin-1;
	//w prawo
	int counter = 0;
	if (isOnBoard(x + j, y))  
	{
		for (int i = 0; i <= j; i++)
		{
			if ( getTileStatus(x + i, y) == choice)
			{
				counter++;
			}
		}

		if (counter == howManyToWin)
		{
			return true;
		}
	}
	//w dol
	counter = 0;
	if (isOnBoard(x , y - j))
	{
		for (int i = 0; i <= j; i++)
		{
			if (getTileStatus(x, y - i) == choice)
			{
				counter++;
			}
		}
		if (counter == howManyToWin)
		{
			return true;
		}
	}
	//skos w prawo
	counter = 0;
	if (isOnBoard(x + j, y - j))
	{
		for (int i = 0; i <= j; i++)
		{
			if (getTileStatus(x + i, y - i) == choice)
			{
				counter++;
			}
		}
		if (counter == howManyToWin)
		{
			return true;
		}
	}
	//skos w lewo
	counter = 0;
	if (isOnBoard(x - j, y - j))
	{
		for (int i = 0; i <= j; i++)
		{
			if (getTileStatus(x - i, y - i) == choice)
			{
				counter++;
			}
		}
		if (counter == howManyToWin)
		{
			return true;
		}
	}
	return false;
}





