#include "Game.h"

Game::Game()
{
    playerTurn = true;
    gameEnded = 0;
    gameStarted = false;
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
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    // Check if the button is clicked
                    if (mouseX >= 200 && mouseX <= 400 && mouseY >= 275 && mouseY <= 325)
                    {
                        gameStarted = true;
                        goto draw;
						break;
                    }
                    else if (mouseX >= 200 && mouseX <= 400 && mouseY >= 375 && mouseY <= 425)
                    {
                        gameStarted = true;
                        goto draw;
                        break;
                    }


                }
            }
            else if (gameStarted && gameEnded == 0)
            {
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
        }
        draw:
        window.clear();
        if (!gameStarted)
        {
            drawStartGame(window);
        }
        else
        {
            window.clear();
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
    std::vector<std::tuple<int, int>> attackableTiles = plansza.getAttackableTiles();
    std::random_shuffle(attackableTiles.begin(), attackableTiles.end());
    if (attackableTiles.size() > 0)
    {
        plansza.attack(std::get<0>(attackableTiles[0]), std::get<1>(attackableTiles[0]), false);
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

void Game::drawStartGame(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Handle error
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setString("Wybierz tryb gry");
    text.setPosition(205, 185); // Centered in the button
    window.draw(text);
    //narysuj dwa guziki, jeden do gry z botem, drugi do gry z graczem
    for (int i = 0; i < 2; i++)
    {
        sf::RectangleShape button(sf::Vector2f(200, 50));
        button.setFillColor(sf::Color::Black);
        button.setOutlineThickness(5);
        button.setOutlineColor(sf::Color::Red);
        button.setPosition(200, 275 + i * 100); // Centered in the window
        window.draw(button);
    }
    // Draw button text

    text.setString("Gra z botem");
    text.setPosition(225, 285); // Centered in the button
    window.draw(text);

    text.setString("Gra z graczem");
    text.setPosition(225, 385); // Centered in the button
    window.draw(text);
}
