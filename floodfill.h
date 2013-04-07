
/*
 *floodfill algorithm. We will do a full floodfill for each step for now. Possibly later by not completely floodfilling we can speed thigns up without having too inefficient a maze-solver in practice. 
 *Basically all the numbers here are stored in chars (or subsets of chars).
 *This is because I'm afraid we'll run out of memory.
 */

#pragma once

//#define ABS(A) (A<0?-A:A)
//#define MIN(A,B) (A<B?A:B)

#define GRID_SIZE 4
#define NORTH ((char)(1<<7))
#define EAST ((char)(1<<6))
#define SOUTH ((char)(1<<5))
#define WEST ((char)(1<<4))

#define VISITS_MASK (15)
#define WALLS_MASK (15<<4)

#define VISITS(val) (val&VISITS_MASK)
#define WALLS(val) (val&WALLS_MASK)

#define NORTH_WALL(val) (val&NORTH)
#define SOUTH_WALL(val) (val&SOUTH)
#define EAST_WALL(val) (val&EAST)
#define WEST_WALL(val) (val&WEST)

#define ADD_WALL(val,wall) val|=wall


struct Cell {
  /*
   *first 4 bits = NSEW walls (1 = there is a wall)
   *second 4 bits = number of visits to this cell (caps at 15)
   */
  char walls_visits;

  /*
   *current best estimate of distance from this cell to the center
   */
  char distance;
};

class Maze {
 public:
  Cell grid[GRID_SIZE][GRID_SIZE];

  /*
   *constructor - sets up the grid with no walls and no visits, initializes distances
   */
  Maze(void);


  /*
   *Visit:
   *"Visits" the specified cell given observed walls -
   *increment the visit count
   *if observed walls are not equal to previously guessed walls, update walls
   */
  void Visit(char x, char y,char Walls);

  /*
   *Add_Walls:
   *adds walls into the maze
   *increments the visits count for the current cell
   *x,y = coordinates of current cell
   *wall = OR of walls to add in (e.g. walls = NORTH|SOUTH will add two walls).
   *will also add the other side of the walls from the adjacent cells.
   */
  void Add_Walls(char x, char y, char wall);

  /*
   *Flood_Fill:
   *Floodfills the maze grid starting from the current cell, updating all the distances to their final values.
   *returns the direction of the fastest path to the center from current cell. Ties are given to less-explored cells.
   */
  char Flood_Fill(char x,char y);

  /*
   *Get_Direction:
   *returns the direction of the fastest path to the center from the given cell coordinates
   *returns 0 if the current coordinates are already at the goal
   */
  char Get_Direction(char x, char y);

  /*
   *Reset_Goal:
   *sets the given position to the goal of the maze, reflood-fills it
   */
  void Reset_Goal(char x, char y);
 private:

  /*
   *Update_Distance:
   *updates distance value for cell at position x,y
   *distance updates to minimum distance of a neighbor plus 1
   *returns 1 if the cell's distance value was changed, 0 otherwise
   */
  char Update_Distance(char x, char y);



};
