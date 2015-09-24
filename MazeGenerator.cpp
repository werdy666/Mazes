#include "MazeGenerator.h"

void MazeGenerator::GenerateNewMaze(std::vector<int> &mazearray, int mazexsize, int mazeysize)
{
	// maze maker magic V0.2 - using std::stack for extra large mazes, Eg.. 2001 x 2001

	MazeSizeX = mazexsize;
	MazeSizeY = mazeysize;
	Visited.resize(MazeSizeX*MazeSizeY);
	for (auto c : Visited)
	{
		Visited[c] = false;
	}
	ClearMaze(mazearray);
	bool odd = false;
	while (!odd) // Randomize start position on grid for Maze Creation 
	{
		int startx = rand() % (MazeSizeX - 1) + 1;
		int starty = rand() % (MazeSizeY - 1) + 1;
		if (startx % 2 == 1 && starty % 2 == 1) // check to see if we have 2 odd numbers
		{
			odd = true;
			StartPoint = startx + (starty * MazeSizeX); // set new start point
		}
	}

	Visited[StartPoint] = true;
	mazearray[StartPoint] = 1;

	int xposcell = StartPoint % MazeSizeX; // x pos in tiles
	int yposcell = StartPoint / MazeSizeX; // y pos in tiles
	
	// call Map Generation function( Non Recursive )
	Generate(mazearray, xposcell, yposcell);
	return;
}
bool MazeGenerator::Generate(std::vector<int> &mazearray, int x, int y)
{
	int TileX = x;
	int TileY = y;
	int VisitedCells = 1; // start point has been visited
	int CurrentCell = StartPoint;
	MyStack.push(CurrentCell);
	std::vector <int> AvailableDirections;

	while (!MyStack.empty())
	{   //check available directions. if one is found push it onto availabledirections vector
		if (TileX - 2 > 0 && !Visited[CurrentCell - 2] &&
			TileX - 1 > 0 && !Visited[CurrentCell - 1]) // If both new tile and between tile is not visited and not outside map
		{
			// tiles are 1 option to visit.
			AvailableDirections.push_back(Direction::Left);
		}
		if (TileX + 2 < MazeSizeX && !Visited[CurrentCell + 2] &&
			TileX + 1 < MazeSizeX && !Visited[CurrentCell + 1]) // If both new tile and between tile is not visited and not outside map
		{
			// tiles are 1 option to visit.
			AvailableDirections.push_back(Direction::Right);
		}
		if (TileY - 2 > 0 && !Visited[CurrentCell - (MazeSizeX*2)] &&
			TileY - 1 > 0 && !Visited[CurrentCell - MazeSizeX]) // If both new tile and between tile is not visited and not outside map
		{
			// tiles are 1 option to visit.
			AvailableDirections.push_back(Direction::Up);
		}
		if (TileY + 2 < MazeSizeY && !Visited[CurrentCell + (MazeSizeX * 2)] &&
			TileY + 1 < MazeSizeY && !Visited[CurrentCell + MazeSizeX]) // If both new tile and between tile is not visited and not outside map
		{
			// tiles are 1 option to visit.
			AvailableDirections.push_back(Direction::Down);
		}
		if (AvailableDirections.size() > 0)
		{
			std::random_shuffle(AvailableDirections.begin(), AvailableDirections.end()); // randomizes new direction for DFS

			switch (AvailableDirections[0])
			{
			case Direction::Up:
				ProcessPath(mazearray, CurrentCell - (MazeSizeX * 2), CurrentCell - MazeSizeX);
				MyStack.push(CurrentCell);
				CurrentCell -= MazeSizeX*2; 
				VisitedCells += 2;
				TileY -= 2;
				AvailableDirections.clear();
				break;
			case Direction::Down:
				ProcessPath(mazearray, CurrentCell + (MazeSizeX * 2), CurrentCell + MazeSizeX);
				MyStack.push(CurrentCell);
				CurrentCell += MazeSizeX * 2; 
				VisitedCells += 2;
				TileY += 2; 
				AvailableDirections.clear();
				break;
			case Direction::Left:
				ProcessPath(mazearray, CurrentCell - 2, CurrentCell - 1);
				MyStack.push(CurrentCell);
				CurrentCell -= 2; 
				VisitedCells += 2;
				TileX -= 2;
				AvailableDirections.clear();
				break;
			case Direction::Right:
				ProcessPath(mazearray, CurrentCell + 2, CurrentCell + 1);
				MyStack.push(CurrentCell);
				CurrentCell += 2; 
				VisitedCells += 2;
				TileX += 2;
				AvailableDirections.clear();
				break;
			}
		} // if available directions is not 0;
		else // no avialable direction to follow
		{
			// pop one from the stack to retry
			CurrentCell = MyStack.top();
			TileX = CurrentCell % MazeSizeX; // x pos in tiles
			TileY = CurrentCell / MazeSizeX; // y pos in tiles
			MyStack.pop();
		}
	}// while totalcells !=0
	return true;

}
bool MazeGenerator::ClearMaze(std::vector<int> &mazearray)
{
	for (auto &a : Visited)
	{
		a = false;
	}
	for (unsigned int x = 0; x < mazearray.size(); x++)
	{
		mazearray[x] = 0;
	}
	return true;
}

void MazeGenerator::ProcessPath(std::vector<int> &mazearray, int NewTile, int BetweenTile)
{
	Visited[NewTile] = true;
	Visited[BetweenTile] = true;
	mazearray[NewTile] = 1;
	mazearray[BetweenTile] = 1;
}