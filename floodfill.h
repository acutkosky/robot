
/*
 *floodfill algorithm. We will do a full floodfill for each step for now. Possibly later by not completely floodfilling we can speed thigns up without having too inefficient a maze-solver in practice. 
 *Basically all the numbers here are stored in unsigned chars (or subsets of unsigned chars).
 *This is because I'm afraid we'll run out of memory.
 */

#pragma once

//#define ABS(A) (A<0?-A:A)
//#define MIN(A,B) (A<B?A:B)

#define GRID_SIZE 16
#define NORTH ((unsigned char)(128))
#define EAST ((unsigned char)(64))
#define SOUTH ((unsigned char)(32))
#define WEST ((unsigned char)(16))

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
  unsigned char walls_visits;

  /*
   *current best estimate of distance from this cell to the center
   */
  unsigned char distance;

  unsigned char turns;
  //wasting 4 bits here... can anything be done about that? 
  unsigned char valid_directions;
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
  void Visit(unsigned char x, unsigned char y,unsigned char Walls);

  /*
   *Add_Walls:
   *adds walls into the maze
   *increments the visits count for the current cell
   *x,y = coordinates of current cell
   *wall = OR of walls to add in (e.g. walls = NORTH|SOUTH will add two walls).
   *will also add the other side of the walls from the adjacent cells.
   */
  void Add_Walls(unsigned char x, unsigned char y, unsigned char wall);

  /*
   *Flood_Fill:
   *Floodfills the maze grid starting from the current cell, updating all the distances to their final values.
   *returns the direction of the fastest path to the center from current cell. Ties are given to less-explored cells.
   */
  unsigned char Flood_Fill(unsigned char x,unsigned char y);

  /*
   *Get_Best_Direction:
   *returns direction of fastest path with least turns visited least option.
   */
  unsigned char Get_Best_Direction(unsigned char,unsigned char,unsigned char);


  /*
   *Get_Direction:
   *returns the direction of the fastest path to the center from the given cell coordinates
   *returns 0 if the current coordinates are already at the goal
   */
  unsigned char Get_Direction(unsigned char x, unsigned char y);

  /*
   *Reset_Goal:
   *sets the given position to the goal of the maze, reflood-fills it
   */
  void Reset_Goal(unsigned char x, unsigned char y);
 private:

  /*
   *Update_Distance:
   *updates distance value for cell at position x,y
   *distance updates to minimum distance of a neighbor plus 1
   *returns 1 if the cell's distance value was changed, 0 otherwise
   */
  unsigned char Update_Distance(unsigned char x, unsigned char y);

  /*
   *Update_Turns:
   *updates the best number of turns from a given node
   */
  unsigned char Update_Turns(unsigned char x,unsigned char y);

};
