
#include "robot.h"
/*
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
*/
Robot::Robot(void) {
  driver = Driver();
  orientation = EAST;
  x = y = 0;
  explored_new = 0;
}


void Robot::Advance(float squares) {
  //  Serial.println("advancing");
  float dist = driver.Forward(squares*SQUARE_LENGTH);
  
  //Serial.println("advanced!");
  if(driver.forward_sensor.read(4)>FORWARD_OPEN_THRESHOLD) {
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
  if(degrees == 180) {
    // hack for turning to the further wall if turning around
    if(driver.right_sensor.read()  > driver.right_sensor.middle_distance) {
      driver.Turn(-180*STEPS_PER_DEGREE);
      return;
    }
  }

  driver.Turn(degrees*STEPS_PER_DEGREE);
}

void Robot::Go(float squares,unsigned char direction) {

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

  if(turn == 180 || turn == -180) {
    squares -= REVERSE_COMPENSATION;
  }
  Turn(turn);

  orientation = direction;

  Advance(squares);
}


unsigned char Robot::Read_Walls(void) {
  unsigned char walls = 0;
  //  delay(10);
  float fs = driver.forward_sensor.read();
  float rs = driver.right_sensor.read();
  float ls = driver.left_sensor.read();
#ifdef DEBUG
  Serial.print("\n\r");
#endif
  if(fs> FORWARD_OPEN_THRESHOLD) {
    //there is a wall in front!
#ifdef WALL_DEBUG
    Serial.print("wall in front! reading: ");
    Serial.print(fs);
    Serial.print("\n\r");
#endif
    ADD_WALL(walls,orientation);
  }

  if(rs > RIGHT_OPEN_THRESHOLD) {
    //there is a wall to the right!
#ifdef WALL_DEBUG
    Serial.print("wall to right! reading: ");
    Serial.print(rs);
    Serial.print("\n\r");
#endif
    if(orientation == WEST) {
      ADD_WALL(walls,NORTH);
    } else {
      ADD_WALL(walls,(orientation>>1));
    }
  }

  if(ls > LEFT_OPEN_THRESHOLD) {
    //there is a wall to the left!
#ifdef WALL_DEBUG
    Serial.print("wall to left! reading: ");
    Serial.print(ls);
    Serial.print("\n\r");
#endif
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
      //Serial.println("oh no! inconsistent wall readings...");
      walls = Read_Walls();
      if(walls == WALLS(maze.grid[x][y].walls_visits)) {
	//Serial.println("inconsistencies resolved to old reading");
      } else {
	//Serial.println("old reading discarded for a new one");
      }
    } else {
      //Serial.println("consistent wall readings!");
    }
  } else {
    //Serial.println("visiting a new square");
  }

  
  return walls;
}

unsigned char Robot::Update_Maze(void) {
  unsigned char cur_walls = 0;//Walls();
  unsigned char visits = VISITS(maze.grid[x][y].walls_visits);
  unsigned char ret;

  float rs = driver.right_sensor.read();
  float ls = driver.left_sensor.read();
  float fs = driver.forward_sensor.read();
 
  char leftoffsetx, leftoffsety;

  unsigned char changed_maze = 0;

  switch(orientation) {
  case NORTH:
    leftoffsetx = -1;
    leftoffsety = 0;
    break;
  case SOUTH:
    leftoffsetx = 1;
    leftoffsety = 0;
    break;
  case EAST:
    leftoffsetx = 0;
    leftoffsety = -1;
    break;
  case WEST:
    leftoffsetx = 0;
    leftoffsety = 1;
    break;
  }


  if(fs > FORWARD_FAR_THRESHOLD) {
    if(fs>FORWARD_OPEN_THRESHOLD) {
      cur_walls |= orientation;
    } else {
      changed_maze |= maze.Add_Walls(x-leftoffsety,y+leftoffsetx,orientation);
    }
  }

  if(rs > RIGHT_FAR_THRESHOLD) {
    if(rs >RIGHT_OPEN_THRESHOLD) {
      if(orientation == WEST) {
	cur_walls |= NORTH;
      } else {
	cur_walls |= (orientation >> 1);
      }
    } else {

      //wall two to the right!

      if(orientation == WEST) {
	changed_maze |= maze.Add_Walls(x-leftoffsetx,y-leftoffsety,NORTH);
      } else {
	changed_maze |= maze.Add_Walls(x-leftoffsetx,y-leftoffsety,(orientation >> 1));
      }
    }
  }
  

  if(ls > LEFT_FAR_THRESHOLD) {
    if(ls >LEFT_OPEN_THRESHOLD) {
      if(orientation == NORTH) {
	cur_walls |= WEST;
      } else {
	cur_walls |= (orientation << 1);
      }
    } else {
      //wall two to the left!
      if(orientation == NORTH) {
	changed_maze |= maze.Add_Walls(x+leftoffsetx,y+leftoffsety,WEST);
      } else {
	changed_maze |= maze.Add_Walls(x+leftoffsetx,y+leftoffsety,(orientation << 1));
      }
    }
  }

  //are we in a corridor?
  switch(orientation) {
  case NORTH:
  case SOUTH:
    ret = (cur_walls == (EAST|WEST));
    break;
  case EAST:
  case WEST:
    ret = (cur_walls == (NORTH|SOUTH));
    break;
  }
  if(visits == 0) {//
    explored_new = 1;
    }

  changed_maze |= maze.Visit(x,y,cur_walls);
  //printwalls(cur_walls);
  //we only want to floodfill if we're about to turn

  return ret;//maze.Flood_Fill(x,y);

}

/*
void printmaze(Maze& maze) {
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++) {
      Serial.print((uint)maze.grid[j][i].distance);
      Serial.print(" ");
    }
    Serial.print("\n\r");
  }
}
*/
unsigned char Robot::Best_Direction(void) {
  unsigned char target_distance = maze.grid[x][y].distance;
  if(target_distance == 0) {
    return 0;
  } else {
    target_distance--;
  }

  if(!(orientation&maze.grid[x][y].walls_visits)) {
    switch(orientation) {
    case NORTH:
      if(maze.grid[x][y-1].distance <= target_distance)
	return NORTH;
      break;
    case EAST:
      if(maze.grid[x+1][y].distance <= target_distance)
	return EAST;
      break;
    case SOUTH:
      if(maze.grid[x][y+1].distance <= target_distance)
	return SOUTH;
      break;
    case WEST:
      if(maze.grid[x-1][y].distance <= target_distance)
	return WEST;
      break;
    }
  }

  return maze.Get_Direction(x,y);
}

int Robot::Maze_Step(void) {

  //if we are in a corridor then go straight. otherwise floodfill.
  unsigned char corridor = Update_Maze();  
  unsigned char visits = VISITS(maze.grid[x][y].walls_visits);



  unsigned char direction;


  if(visits == 1) {
    if(corridor) {

      //Serial.println("corridor!");
      direction = orientation;
    } else {
      //Serial.println("floodfilling!");
      maze.Flood_Fill(x,y);
      driver.current_speed = 0;
      direction = Best_Direction();
    }
  } else {
    direction = Best_Direction();
    //Serial.println("visited square!");
  }
  //printmaze(maze);
    


  /*
  if(direction != orientation && VISITS(maze.grid[x][y].walls_visits)==0) {
    maze.Flood_Fill(x,y);
    direction = maze.Get_Direction(x,y);
  }
  */
  if(direction == 0) {
    //Serial.println("direction = 0!");

    return 0;
  }

#ifdef DEBUG
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
#endif

  /*ok, if we want to go straight through n explored squares
   *we should do it as one Go command*/

  float numsquares = 1.0;
  
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
    //oh noes


  }

  unsigned char tempx = x+delx;
  unsigned char tempy = y+dely;

  while(VISITS(maze.grid[tempx][tempy].walls_visits)!=0 && maze.Get_Direction(tempx,tempy)==direction) {
    numsquares+=1.0;
    tempx += delx;
    tempy += dely;
  }
  /*  Serial.println("calced numsquares. direction:");
  printwalls(direction);
  Serial.println("numsquares:");
  Serial.println(numsquares);
  */
  Go(numsquares,direction);
  //Serial.println("went!");


  return 1;

}



unsigned char Robot::Solve(void) {

  //make sure everything is set up.
  Update_Maze();
  explored_new = 0;
  while(Maze_Step());

  //done!

  return explored_new;
}

  
