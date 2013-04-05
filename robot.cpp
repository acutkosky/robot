
#include "robot.h"

Robot::Robot(void) {
  orientation = EAST;
  x = y = 0;
}

void Robot::Advance(int squares) {
  driver.Forward(squares*Square_Length);

  /*update position*/
  switch(orientation) {
  case NORTH:
    y-=squares;
    break;
  case EAST:
    x+=squares;
    break;
  case SOUTH:
    y+=squares;
    break;
  case WEST:
    x-=squares;
    break;
  }

}

void Robot::Turn(int degrees) {
  driver.Turn(degrees*STEPS_PER_DEGREE);
}

void Robot::Go(int squares,char direction) {

  if(orientation == direction) {
    Advance(squares);
    return;
  }

  int start,end;

  switch(orientation) {
  case NORTH:
    start = 0;
    break;
  case EAST:
    start = 90;
    break;
  case SOUTH:
    start = 180;
    break;
  case WEST:
    start = 270;
  }

  switch(direction) {
  case NORTH:
    end = 0;
    break;
  case EAST:
    end = 90;
    break;
  case SOUTH:
    end = 180;
    break;
  case WEST:
    end = 270;
    break;
  }

  int turn = end-start;

  if(turn == 270)
    turn = -90;

  if(turn == -270)
    turn = 90;

  Turn(turn);

  orientation = direction;

  Advance(squares);
}


char Robot::Walls(void) {
  char walls = 0;

  if(driver.forward_sensor.read() > FORWARD_OPEN_THRESHOLD) {
    //there is a wall in front!
    ADD_WALL(walls,orientation);
  }

  if(driver.right_sensor.read() > RIGHT_OPEN_THRESHOLD) {
    //there is a wall to the right!
    if(orientation == WEST) {
      ADD_WALL(walls,NORTH);
    } else {
      ADD_WALL(walls,orientation>>1);
    }
  }

  if(driver.left_sensor.read() > LEFT_OPEN_THRESHOLD) {
    //there is a wall to the left!
    if(orientation == NORTH) {
      ADD_WALL(walls,WEST);
    } else {
      ADD_WALL(walls,orientation<<1);
    }
  }

  return walls;
}


void Robot::Update_Maze(void) {
  char cur_walls = Walls();
  maze.Visit(x,y,cur_walls);
  maze.Flood_Fill();
}

int Robot::Maze_Step(void) {
  char direction = maze.Get_Direction(x,y);
  if(direction == 0)
    return 0;

  Go(1,direction);
  Update_Maze();
  return 1;
}

void Robot::Solve(void) {

  //make sure everything is set up.
  Update_Maze();

  while(Maze_Step());

  //done!
}

  
