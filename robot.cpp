
#include "robot.h"

Robot::Robot(void) {
  driver = Driver();
  orientation = EAST;
  x = y = 0;
}


void Robot::Advance(int squares) {
  float dist = driver.Forward(squares*SQUARE_LENGTH);

  if(driver.forward_sensor.delay_read(10,5)>FORWARD_OPEN_THRESHOLD) {
    //there is a wall in front of us - let's go forward until we hit it
    dist+=driver.Forward(0.25*SQUARE_LENGTH);
  }

  squares = (int)(dist/SQUARE_LENGTH+0.5);

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

void Robot::Go(int squares,unsigned char direction) {

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


unsigned char Robot::Read_Walls(void) {
  unsigned char walls = 0;
  float fs = driver.forward_sensor.read(10);
  float rs = driver.right_sensor.read(10);
  float ls = driver.left_sensor.read(10);
  Serial.print("\n\r");
  if(fs> FORWARD_OPEN_THRESHOLD) {
    //there is a wall in front!
    Serial.print("wall in front! reading: ");
    Serial.print(fs);
    Serial.print("\n\r");
    ADD_WALL(walls,orientation);
  }

  if(driver.right_sensor.read(10) > RIGHT_OPEN_THRESHOLD) {
    //there is a wall to the right!
    Serial.print("wall to right! reading: ");
    Serial.print(rs);
    Serial.print("\n\r");
    if(orientation == WEST) {
      ADD_WALL(walls,NORTH);
    } else {
      ADD_WALL(walls,(orientation>>1));
    }
  }

  if(driver.left_sensor.read(10) > LEFT_OPEN_THRESHOLD) {
    //there is a wall to the left!
    Serial.print("wall to left! reading: ");
    Serial.print(ls);
    Serial.print("\n\r");
    if(orientation == NORTH) {
      ADD_WALL(walls,WEST);
    } else {
      ADD_WALL(walls,(orientation<<1));
    }
  }

  return walls;
}

unsigned char Robot::Walls(void) {

  unsigned char walls = Read_Walls();
  if(VISITS(maze.grid[x][y].walls_visits)!=0) {
    if(walls != WALLS(maze.grid[x][y].walls_visits)) {
      //something changed? better check again...
      Serial.println("oh no! inconsistent wall readings...");
      walls = Read_Walls();
      if(walls == WALLS(maze.grid[x][y].walls_visits)) {
	Serial.println("inconsistencies resolved to old reading");
      } else {
	Serial.println("old reading discarded for a new one");
      }
    } else {
      Serial.println("consistent wall readings!");
    }
  } else {
    Serial.println("visiting a new square");
  }


  return walls;
}

void Robot::Update_Maze(void) {
  unsigned char cur_walls = Walls();
  maze.Visit(x,y,cur_walls);
  maze.Flood_Fill(x,y);
}


void printmaze(Maze& maze) {
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++) {
      Serial.print((uint)maze.grid[j][i].distance);
      Serial.print(" ");
    }
    Serial.print("\n\r");
  }
}

void printwalls(unsigned char walls) {
  if(NORTH_WALL(walls))
    Serial.println("North");
  if(EAST_WALL(walls))
    Serial.println("East");
  if(SOUTH_WALL(walls))
    Serial.println("South");
  if(WEST_WALL(walls))
    Serial.println("West");
}

int Robot::Maze_Step(void) {
  unsigned char direction = maze.Get_Direction(x,y);
  if(direction == 0) {
    Serial.println("direction = 0!");
    return 0;
  }
  printmaze(maze);
  Serial.print("position: ");
  Serial.print((uint)x);
  Serial.print(" ");
  Serial.print((uint)y);
  Serial.print("\n\r");
  Serial.println("Walls here:");
  printwalls(maze.grid[x][y].walls_visits);

  switch(direction) {
  case NORTH:
    Serial.println("direction = NORTH!");
    break;
  case WEST:
    Serial.println("direction = WEST!");
    break;
  case EAST:
    Serial.println("direction = EAST!");
    break;
  case SOUTH:
    Serial.println("direction = SOUTH!");
    break;
  default:
    Serial.println("direction is strange!");
  }

  /*ok, if we want to go straight through n explored squares
   *we should do it as one Go command*/

  int numsquares = 1;
  
  char delx;
  char dely;
  
  switch(direction) {
  case SOUTH:
    delx = 0;
    dely = 1;
    break;
  case NORTH:
    delx = 0;
    dely = -1;
    break;
  case EAST:
    dely=0;
    delx = 1;
    break;
  case WEST:
    dely=0;
    delx = -1;
    break;
  }

  unsigned char tempx = x+delx;
  unsigned char tempy = y+dely;

  while(VISITS(maze.grid[tempx][tempy].walls_visits)!=0 && maze.Get_Direction(tempx,tempy)==direction) {
    numsquares++;
    tempx += delx;
    tempy += dely;
  }

  Go(numsquares,direction);
  Update_Maze();
  return 1;
}



void Robot::Solve(void) {

  //make sure everything is set up.
  Update_Maze();

  while(Maze_Step());

  //done!
}

  
