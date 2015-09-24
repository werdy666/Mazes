#include "Game.h"


Game::Game(int ScreenWidth, int ScreenHeight, int TSize)
	:m_Window(sf::VideoMode(ScreenWidth, ScreenHeight), "Mazes V0.8"),
	m_TileSize(TSize, TSize),
	m_MazeSize(51, 51),
	m_TimePerFrame(1.f / 120.f),
	m_IsMovingLeft(false),
	m_IsMovingRight(false),
	m_IsMovingUp(false),
	m_IsMovingDown(false),
	m_PlayerSpeed(4.0f),
	m_ScreenSize(ScreenWidth, ScreenHeight)
{
	m_MazeData.resize(m_MazeSize.x * m_MazeSize.y);
	Maze.GenerateNewMaze(m_MazeData, m_MazeSize.x, m_MazeSize.y);
	SetStartandExitTiles();
	m_LevelTime = 0;

	m_FloorTexture.loadFromFile("./Res/Floor.png", sf::IntRect(0, 0, m_TileSize.x, m_TileSize.y));
	m_WallTexture.loadFromFile("./Res/Wall.png", sf::IntRect(0, 0, m_TileSize.x, m_TileSize.y));
	m_PlayerTexture.loadFromFile("./Res/Player.png", sf::IntRect(0, 0, m_TileSize.x, m_TileSize.y));
	m_ExitTexture.loadFromFile("./Res/Exit.png", sf::IntRect(0, 0, m_TileSize.x, m_TileSize.y));

	m_Wall.setTexture(m_WallTexture);
	m_Floor.setTexture(m_FloorTexture);
	m_Player.setTexture(m_PlayerTexture);
	m_Exit.setTexture(m_ExitTexture);

	m_PlayerExitedLevel = false;
	m_GameView.setCenter(m_Player.getPosition().x, m_Player.getPosition().y);
	m_GameView.setSize(m_ScreenSize.x,m_ScreenSize.y); // change to variable screenwidth and screenheight

}
void Game::Run()
{
	sf::Clock Clock;
	float TimeSinceLastUpdate = 0;
	while (m_Window.isOpen())
	{
		float elapsedtime = Clock.restart().asSeconds();
		TimeSinceLastUpdate += elapsedtime;
		m_LevelTime += elapsedtime;
		while (TimeSinceLastUpdate > m_TimePerFrame)
		{
			TimeSinceLastUpdate = 0;
			ProcessEvents();
			Update();
		}
		Render();
	}
}
void Game::ProcessEvents()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_Window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			HandlePlayerInput(event.key.code, true);
		}
		if (event.type == sf::Event::KeyReleased)
		{
			HandlePlayerInput(event.key.code, false);
		}
	}

}
void Game::Update()
{
	// move player based on IsmovingLeft etc variables and check collisions

	if (m_IsMovingLeft)
	{
		m_Player.move(-m_PlayerSpeed, 0);
		TileCollision(Direction::Left);
	}
	if (m_IsMovingRight)
	{
		m_Player.move(m_PlayerSpeed, 0);
		TileCollision(Direction::Right);
	}
	if (m_IsMovingUp)
	{
		m_Player.move(0, -m_PlayerSpeed);
		TileCollision(Direction::Up);
	}
	if (m_IsMovingDown)
	{
		m_Player.move(0,m_PlayerSpeed);
		TileCollision(Direction::Down);
	}
	if (!m_PlayerExitedLevel)
	{
		if (m_Player.getPosition().x == m_ExitPos.x * m_TileSize.x
			&& m_Player.getPosition().y == m_ExitPos.y * m_TileSize.y)
			// player has exited
		{
			m_PlayerExitedLevel = true;
		}
	}
	if (m_PlayerExitedLevel)
	{
		m_Window.close();
	}
}
void Game::Render()
{
	m_Window.clear(sf::Color::Blue);
	m_GameView.setCenter(m_Player.getPosition().x, m_Player.getPosition().y);
	m_Window.setView(m_GameView);

	sf::Vector2f centrescreen;
	centrescreen = m_GameView.getCenter(); // view centre of player x and y
	int minx, miny, maxx, maxy;
	int renderx = centrescreen.x;
	int rendery = centrescreen.y;
	int a = (m_ScreenSize.x / m_TileSize.x + 3) / 2; // formula to draw addition tiles outside screen to allow smooth scrolling
	int xsize = a * m_TileSize.x;
	int ysize = a * m_TileSize.y;
	minx = renderx - xsize;
	miny = rendery - ysize;
	maxx = renderx + xsize;
	maxy = rendery + ysize;

	if (minx < 0)
	{
		minx = 0;
	}
	if (miny < 0)
	{
		miny = 0;
	}
	if (maxx > m_MazeSize.x * m_TileSize.x)
	{
		maxx = m_MazeSize.x * m_TileSize.x;
	}
	if (maxy > m_MazeSize.y * m_TileSize.y)
	{
		maxy = m_MazeSize.y * m_TileSize.y;
	}
	// setup ready for drawing method, 
	minx /= m_TileSize.x;
	maxx /= m_TileSize.x;
	miny /= m_TileSize.y;
	maxy /= m_TileSize.y;


	// DRAW TILES
	for (int x = minx; x < maxx; x++)
	{
		for (int y = miny; y < maxy; y++)
		{
			int a = m_MazeData[x + y * m_MazeSize.x];
			if (a == 0)//draw wall
			{
				m_Wall.setPosition(x * m_TileSize.x, y * m_TileSize.y);
				m_Window.draw(m_Wall);
			}
			else // draw floor
			{
				m_Floor.setPosition(x * m_TileSize.x, y * m_TileSize.y);
				m_Window.draw(m_Floor);
			}
		}
	}

	// Draw Exit
	m_Window.draw(m_Exit);
	// Draw Player
	m_Window.draw(m_Player);
	m_Window.display();
	m_Window.setView(m_GameInfoView); // status bar, time, lives etc
	m_Window.setView(m_Window.getDefaultView());
}
void Game::HandlePlayerInput(sf::Keyboard::Key Key, bool IsPressed )
{
	if (Key == sf::Keyboard::Escape)
	{
		m_Window.close();
	}
	if (Key == sf::Keyboard::Space && IsPressed == false) // Generate new maze when space is released
	{
		Maze.GenerateNewMaze(m_MazeData, m_MazeSize.x, m_MazeSize.y);
		SetStartandExitTiles(); 
		m_LevelTime = 0; // reset leveltime
	}

	if (Key == sf::Keyboard::Left)
	{
		m_IsMovingLeft = IsPressed;
	}
	else if (Key == sf::Keyboard::Right)
	{
		m_IsMovingRight = IsPressed;
	}
	if (Key == sf::Keyboard::Up)
	{
		m_IsMovingUp = IsPressed;
	}
	else if (Key == sf::Keyboard::Down)
	{
		m_IsMovingDown = IsPressed;
	}
}
void Game::SetStartandExitTiles() // returns location of exit tile
{
	int Pstartx = 1; int Pstarty = 1;
	bool playerstart = false;

	while (!playerstart)
	{
		int x = rand() % 3 + 1;
		int y = rand() % 3 + 1;
		int pstarttile = x + (y * m_MazeSize.x);
		Pstartx = x;
		Pstarty = y;
		if (m_MazeData[pstarttile] == 1)
		{
			playerstart = true;
		}
		m_Player.setPosition(Pstartx * m_TileSize.x, Pstarty * m_TileSize.x);
	}
	bool IsExitValid = false;
	while (!IsExitValid)
	{
		int percentx = (m_MazeSize.x / 10) + 4;
		int percenty = (m_MazeSize.y / 10) + 4; // use when ready!!!!!!
		int Exitx = rand() % percentx + (m_MazeSize.x - percentx);
		int Exity = rand() % percenty + (m_MazeSize.y - percenty); // change to percenty when ready!!!!!!
		int exit = Exitx + (Exity * m_MazeSize.x);
		if (m_MazeData[exit] == 1)
		{
			IsExitValid = true;
			m_ExitPos.x = Exitx;
			m_ExitPos.y = Exity;
			m_Exit.setPosition(m_ExitPos.x * m_TileSize.x, m_ExitPos.y * m_TileSize.y);
		}
	}

	return;

}
void Game::TileCollision(int DirectionOfTravel)
{
	int xtile = -1;
	int ytile = -1;
	int CheckTileA = -1;
	int CheckTileB = -1;
	switch (DirectionOfTravel)
	{
	case Direction::Down:
		//Bottom Left Corner
		xtile = (m_Player.getPosition().x) / m_TileSize.x;
		ytile = (m_Player.getPosition().y + m_TileSize.y-1)/ m_TileSize.x;
		CheckTileA = xtile + (ytile * m_MazeSize.x);
		//Bottom Right Corner
		xtile = (m_Player.getPosition().x + m_TileSize.x-1) / m_TileSize.x;
		ytile = (m_Player.getPosition().y + m_TileSize.y-1) / m_TileSize.x;
		CheckTileB = xtile + (ytile * m_MazeSize.x);
		break;
	case Direction::Left:
		//Top Left Corner
		xtile = m_Player.getPosition().x / m_TileSize.x;
		ytile = m_Player.getPosition().y / m_TileSize.y;
		CheckTileA = xtile + (ytile * m_MazeSize.x);
		//Bottom Left
		xtile = m_Player.getPosition().x / m_TileSize.x;
		ytile = (m_Player.getPosition().y + m_TileSize.y-1) / m_TileSize.y;
		CheckTileB = xtile + (ytile * m_MazeSize.x);
		break;
	case Direction::Up:
		// Top Left Corner
		xtile = m_Player.getPosition().x / m_TileSize.x;
		ytile = m_Player.getPosition().y / m_TileSize.y;
		CheckTileA = xtile + (ytile * m_MazeSize.x);
		//TopRight Corner
		xtile = (m_Player.getPosition().x + m_TileSize.x-1) / m_TileSize.x;
		ytile = m_Player.getPosition().y / m_TileSize.y;
		CheckTileB = xtile + (ytile * m_MazeSize.x);
		break;
	case Direction::Right:
		// Top Right Corner
		xtile = (m_Player.getPosition().x + m_TileSize.x-1) / m_TileSize.x;
		ytile = m_Player.getPosition().y / m_TileSize.y;
		CheckTileA = xtile + (ytile * m_MazeSize.x);
		//Bottom Right Corner
		xtile = (m_Player.getPosition().x + m_TileSize.x-1) / m_TileSize.x;
		ytile = (m_Player.getPosition().y + m_TileSize.y-1) / m_TileSize.y;
		CheckTileB = xtile + (ytile * m_MazeSize.x);
		break;

	}

	if (m_MazeData[CheckTileA] == 0 || m_MazeData[CheckTileB] == 0) // Check both points of players direction
	{																// IE Down Check Bottom Left & Bottom Right Corners
		switch (DirectionOfTravel)
		{
		case Direction::Up:
			m_Player.setPosition(m_Player.getPosition().x,(ytile + 1) * m_TileSize.y);
			break;
		case Direction::Down:
			m_Player.setPosition(m_Player.getPosition().x, (ytile - 1) * m_TileSize.y);
			break;
		case Direction::Left:
			m_Player.setPosition((xtile + 1) * m_TileSize.x, m_Player.getPosition().y);
			break;
		case Direction::Right:
			m_Player.setPosition((xtile - 1) * m_TileSize.x, m_Player.getPosition().y);
			break;
		}
	}

	return;
}
