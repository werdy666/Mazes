#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include "MazeGenerator.h"



class Game
{
public:
	Game(int ScreenWidth, int ScreenHeight, int TileSize);
	void Run(); // main game loop
private: // methods
	void ProcessEvents();
	void Update();
	void Render();
	void HandlePlayerInput(sf::Keyboard::Key Key, bool IsPressed);
	void SetStartandExitTiles(); // returns location of exit tile
	void TileCollision(int DirectionOfTravel);

public:  // variables

private: // variables

	sf::RenderWindow m_Window;
	std::vector<int> m_MazeData;
	MazeGenerator Maze;
	sf::Vector2i m_TileSize;
	sf::Vector2i m_MazeSize;
	sf::Clock m_FPS;
	sf::Vector2i m_ScreenSize;
	sf::Texture m_WallTexture;
	sf::Texture m_FloorTexture;
	sf::Texture m_PlayerTexture;
	sf::Texture m_ExitTexture;

	sf::Sprite m_Wall;
	sf::Sprite m_Floor;
	sf::Sprite m_Player;
	sf::Sprite m_Exit;

	float m_timesincelastupdate;
	const float m_TimePerFrame; // updates per frame.
	
	float m_LevelTime;
	sf::Vector2i m_ExitPos;
	float m_PlayerSpeed;
	bool m_PlayerExitedLevel;
	bool m_IsMovingLeft;
	bool m_IsMovingRight;
	bool m_IsMovingUp;
	bool m_IsMovingDown;

	//sf::view
	sf::View m_GameView;
	sf::View m_GameInfoView;

};

