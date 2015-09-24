#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>

enum Direction{
	Left,
	Right,
	Up,
	Down
};


class MazeGenerator
{
private:

	bool Generate(std::vector<int> &MazeArray, int x, int y);
	bool ClearMaze(std::vector<int> &MazeArray);
	void ProcessPath(std::vector<int> &MazeArray, int NewTileA, int NewTileB); // new tile a is 2 tiles from start point
																	// TileB is the inbetween tile
	int MazeSizeX;
	int MazeSizeY;
	std::vector<bool> Visited;
	std::stack <int> MyStack;
	int StartPoint;

public:
	void GenerateNewMaze(std::vector<int> &MazeArray, int sizeX, int sizeY);
};