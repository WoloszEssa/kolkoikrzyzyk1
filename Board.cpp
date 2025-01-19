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

void Board::boardDraw(sf::RenderWindow& window)
{
	float cellOffset = 0.13*cellSize;
    for (int x = 0; x < gridSize; ++x)
    {
        for (int y = 0; y < gridSize; ++y) 
        {
            sf::RectangleShape cell(sf::Vector2f(cellSize- 2, cellSize - 2));
            cell.setPosition(offSet + x * cellSize, offSet + y * cellSize);
			sf::Texture texture;
			sf::Sprite sprite;
			if (grid[x][y] == 1)
			{
				texture.loadFromFile("kolko.png");
				sprite.scale(0.007*cellSize, 0.007*cellSize);
				sprite.setTexture(texture);
				sprite.setPosition(offSet+ cellOffset + x * cellSize, offSet + cellOffset + y * cellSize);
			}
			else if (grid[x][y] == 2)
			{
				texture.loadFromFile("krzyzyk.png");
				sprite.scale(0.007*cellSize, 0.007*cellSize);
				sprite.setTexture(texture);
				sprite.setPosition(offSet + cellOffset +x * cellSize, offSet + cellOffset + y * cellSize);
			}
           
            window.draw(cell);
			window.draw(sprite);
        }
    }

}
void Board::attack(int x, int y, bool choice)
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

int Board::offset()
{
	return offSet;
}

int Board::cellsize()
{
	return cellSize;
}

int Board::getTileStatus(int x, int y)
{
	return grid[x][y];
}
int Board::gridsize()
{
	return gridSize;
}	
int Board::getHowManyToWin()
{
	return howManyToWin;
}
void Board::clearTile(int x, int y)
{
	grid[x][y] = 0;
}

void Board::setGridSize(int size)
{
    gridSize = size;
    cellSize = (windowSize / gridSize) / 2;
    grid.clear();
    grid.resize(gridSize);
    for (int i = 0; i < gridSize; ++i) {
        grid[i].resize(gridSize);
    }
}

std::vector<std::tuple<int, int>> Board::getAttackableTiles()
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

int Board::WinCheck()
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
	if (getAttackableTiles().size() == 0)
	{
		return 3;
	}
	
}

bool Board::isOnBoard(int x, int y)
{
	return (x >= 0 && x < gridsize() && y >= 0 && y < gridsize());
}


bool Board::checkAround(int x, int y, int choice)
{
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




