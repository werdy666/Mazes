/////////////////////////////////////////////////////////////////////////////////////////////
///  MazeClasses V0.60 (Based on Mazes V0.6)
///  - Scrolling maze using sf::View [X]
///  - Also draw only what is in view, not entire map. [X]
///
/// MazeClasses V0.5 (Based on Mazes V0.5)
/// Start and Exit tiles Randomized
/// Maze Generator starts from random tile.
///
/// MazeClasses V0.4 
/// - Same Version as non classes maze
/// - Player pixel perfect moving and collision detection
///
/// MazeClasses V0.1 Based on Mazes V0.2
///
/// Mazes V0.2
/// Implement non recursive method for Depth First Search. [X]
///
///
/// Mazes V0.1
/// Implement non square mazes [X], changable sized mazes [X]. vector size of maze [X]
///
/// Bug - Recursion causes stack overflow error with larger sized maps 150+ - Fixed via V0.2
///////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"

int main()
{
	unsigned int Seed = time(0);
	srand(Seed);

	Game NewGame(896,896,32);
	NewGame.Run();
	return 0;
}