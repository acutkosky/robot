
/*
 *robot.h
 *interfaces the robot with the driver and the maze systems
 */

#include "driver.h"
#include "floodfill.h"

#define STEPS_PER_DEGREE (1000.0/1040.0)
#define SQUARE_LENGTH 100 //length of a square in steps of the motors.

class Robot {

  Driver driver;
  Maze maze;
  char orientation;
  char x;
  char y;



  /*
   *Advance:
   *tells the robot to go a specified number of grid squares forward
   */
  void Advance(int squares);

  /*
   *Turn:
   *tells the robot to turn a specified number of degrees
   *DOES NOT SET ORIENTATION
   */
  void Turn(int degrees);

  /*
   *Go:
   *tells the robot to move a specified number of squares in the specified direction
   *calls advance and turn as appropriate.
   *sets orientation.
   */
  void Go(int squares,char direction);

  /*
   *Walls:
   *returns the walls visible from the current cell in an ORed char.
   */
  char Walls(void);

  /*
   *Update_Maze:
   *takes sensor readings to update the current location's
   *walls in the maze, then recalculates flood-fill
   */
  void Update_Maze(void);

  /*
   *Maze_Step:
   *Moves the robot in the best maze-solving direction and updates the maze data.
   *blocks until the robot has finished moving.
   *returns 0 if the robot has already solved the maze
   */
  int Maze_Step(void);


  /*
   *Solve:
   *Solves the maze by calling Maze_Step repeatedly
   *blocks until the maze is solved
   */
  void Solve(void);

  /*
   *constructor
   *just initializes everything to default parameters (orientation is east, starting in the upper left corner
   */
  Robot(void);
}
