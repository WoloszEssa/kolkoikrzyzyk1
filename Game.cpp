#include "Game.h"

Game::Game()
{
    licznik = 0;
    playerTurn = true;
    gameEnded = 0;
    gameStarted = false;
	pinged = true;
}

void Game::Reset()
{
	licznik = 0;
    playerTurn = true;
    gameEnded = 0;
    gameStarted = false;
    pinged = true;
	plansza.clear();
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "KolkoKrzyzyk By Mikolaj Wolos", sf::Style::Titlebar | sf::Style::Close);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle start screen
            if (!gameStarted)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
					handleStartGame(event);
                }
            }
            else if (gameStarted && gameEnded == 0)
            {  
                if (pinged) plansza.clearTile(0, 0); pinged = false;
                if (!playerTurn)
                {
                    
                    handleBotAttack();
                    playerTurn = true;
                    gameEnded = plansza.WinCheck();
                }

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (handlePlayerAttack(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameEnded = plansza.WinCheck();
                        playerTurn = false;
                    }
                }
            }

            if (gameEnded && event.type == sf::Event::KeyPressed)
            {
				Reset();
				licznik = 0;
            }
        }

        window.clear();

        if (!gameStarted)
        {
            drawStartGame(window, licznik);
            
        }
        else
        {
            plansza.boardDraw(window);
            if (gameEnded != 0)
            {
                drawEndGame(window);
            }
        }
        window.display();
    }
}

bool Game::handlePlayerAttack(int mouseX, int mouseY)
{
    int x = (mouseX - plansza.offset()) / plansza.cellsize();
    int y = (mouseY - plansza.offset()) / plansza.cellsize();

    if (isOnBoard(x, y) && plansza.getTileStatus(x, y) == 0)
    {
        plansza.attack(x, y, true);
        return true;
    }
    return false;
}

bool Game::isOnBoard(int x, int y)
{
    return (x >= 0 && x < plansza.gridsize() && y >= 0 && y < plansza.gridsize());
}

void Game::handleBotAttack()
{
        //// Check if the bot can win in the next move
    //for (int x = 0; x < plansza.gridsize(); x++)
    //{
    //    for (int y = 0; y < plansza.gridsize(); y++)
    //    {
    //        botCanWin(x, y, 2);
    //    }
    //}


        //// Take the center if available
        int center = plansza.gridsize() / 2;
        if (plansza.getTileStatus(center, center) == 0)
        {
            plansza.attack(center, center, false);
            return;
        }

        std::vector<std::tuple<int, int>> attackableTiles = plansza.getAttackableTiles();
        std::random_shuffle(attackableTiles.begin(), attackableTiles.end());
        if (attackableTiles.size() > 0)
        {
            plansza.attack(std::get<0>(attackableTiles[0]), std::get<1>(attackableTiles[0]), false);
        }
       
}

bool Game::botCanWin(int x, int y, int choice)
{
	plansza.attack(x, y, choice);
	if (plansza.WinCheck() == choice)
	{
		plansza.clearTile(x, y);
		return true;
	}
	plansza.clearTile(x, y);
	return false;
}

//bool Game::playerCanWin(int x, int y, int choice)
//{
//    for (int x = 0; x < plansza.gridsize(); x++)
//    {
//        for (int y = 0; y < plansza.gridsize(); y++)
//        {
//            if (plansza.getTileStatus(x, y) == 0)
//            {
//                plansza.attack(x, y, true);
//                if (plansza.WinCheck() == 1) // Player wins
//                {
//                    plansza.attack(x, y, false); // Block player
//                    return;
//                }
//                plansza.clearTile(x, y); // Undo move
//            }
//        }
//    }
//}



void Game::botTileNextToPlayer(int x, int y, int choice)
{
    //ruch bota kolo ruchu gracza
	if (plansza.getTileStatus(x, y) == 0)
	{
		plansza.attack(x, y, choice);
		return;
	}
	else
	{
		std::vector<std::pair<int, int>> neighbors = {
			{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1},
			{x - 1, y - 1}, {x + 1, y + 1}, {x - 1, y + 1}, {x + 1, y - 1}
		};
		for (auto& neighbor : neighbors)
		{
			int nx = neighbor.first;
			int ny = neighbor.second;
			if (plansza.isOnBoard(nx, ny) && plansza.getTileStatus(nx, ny) == 0)
			{
				plansza.attack(nx, ny, choice);
				return;
			}
		}
	}
}

void Game::drawEndGame(sf::RenderWindow& window)
{
    //kwadrat za tekstem
    sf::RectangleShape rectangle(sf::Vector2f(200, 50));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineThickness(5);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setPosition(100, 100);
    window.draw(rectangle);
    //tekst
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    if (gameEnded == 1)
    {
        text.setString("Wygralo O");
    }
    else if (gameEnded == 2)
    {
        text.setString("Wygralo X");
    }
    else if (gameEnded == 3)
    {
        text.setString("Remis");
    }
    text.setPosition(100, 100);
    window.draw(text);
}

void Game::drawStartGame(sf::RenderWindow& window, int licznikEkranow)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Handle error
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    //narysuj dwa guziki, jeden do gry z botem, drugi do gry z graczem
	if (licznikEkranow == 0 || licznikEkranow == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            sf::RectangleShape button(sf::Vector2f(200, 50));
            button.setFillColor(sf::Color::Black);
            button.setOutlineThickness(5);
			button.setOrigin(100, 25);
            button.setOutlineColor(sf::Color::Red);
            button.setPosition(200, 275 + i * 100); // Centered in the window
            window.draw(button);
        }
    }
	if (licznikEkranow == 3)
	{
		for (int i = 0; i < 3; i++)
		{
			sf::RectangleShape button(sf::Vector2f(200, 50));
			button.setFillColor(sf::Color::Black);
			button.setOutlineThickness(5);
			button.setOutlineColor(sf::Color::Red);
			button.setPosition(200, 275 + i * 100); // Centered in the window
			window.draw(button);
		}
	}
    // Draw button text
	if (licznikEkranow == 0)
    {
        text.setString("Wybierz tryb gry");
        text.setPosition(205, 185); // Centered in the button
        window.draw(text);

        text.setString("Gra z botem");
        text.setPosition(225, 285); // Centered in the button
        window.draw(text);

        text.setString("Gra z graczem");
        text.setPosition(225, 385); // Centered in the button
        window.draw(text);
    }
	else if (licznikEkranow == 1)
	{
        text.setString("Wybierz czym chcesz grac");
        text.setPosition(205, 185); // Centered in the button
        window.draw(text);

		text.setString("Kólko");
		text.setPosition(225, 285); // Centered in the button
		window.draw(text);

		text.setString("Krzyzyk");
		text.setPosition(225, 385); // Centered in the button
		window.draw(text);
	}
    else if (licznikEkranow == 2)
    {
        text.setString("Wybierz rozmiar planszy");
        text.setPosition(205, 185); // Centered in the button
        window.draw(text);

        text.setString("3x3 - 3 aby wygrac");
        text.setPosition(225, 285); // Centered in the button
        window.draw(text);

        text.setString("5x5 - 4 aby wygrac");
        text.setPosition(225, 385); // Centered in the button
        window.draw(text);

        text.setString("10x10 - 5 aby wygrac");
        text.setPosition(225, 485); // Centered in the button
        window.draw(text);
        
    }


}

void Game::handleStartGame(sf::Event& event)
{
    int mouseX = event.mouseButton.x;
    int mouseY = event.mouseButton.y;
	//Stage 1: Choose game mode
    if (licznik == 0)
    {
        if (mouseX >= 200 && mouseX <= 400 && mouseY >= 275 && mouseY <= 325)//gra z botem
        {
            licznik++;
        }
        else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 375 && mouseY <= 425)//gra z graczem
        {
            licznik++;
        }
    }
    // Stage 2: Choose symbol
    else if (licznik == 1)
    {
        if (mouseX >= 200 && mouseX <= 400 && mouseY >= 275 && mouseY <= 325)//kolko    
        {
            licznik++;
        }
        else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 375 && mouseY <= 425)//krzyzyk
        {
            licznik++;
        }
    }
    // Stage 3: Choose board size
    else if (licznik == 2)
    {

        if (mouseX >= 200 && mouseX <= 400 && mouseY >= 275 && mouseY <= 325)//3x3
        {
            plansza.setGrid(3,3);
            gameStarted = true;
            plansza.boardPing();
        }
        else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 375 && mouseY <= 425)//5x5
        {
            plansza.setGrid(5,4);
            gameStarted = true;
            plansza.boardPing();
        }
        else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 475 && mouseY <= 525)//10x10
        {
            plansza.setGrid(10,5);
            gameStarted = true;
            plansza.boardPing();
        }

    }
}

