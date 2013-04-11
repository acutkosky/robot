

#include "floodfill.h"
//#include<iostream>
//#include<cstdlib>
//using std::cout;

typedef unsigned int uint;

Maze::Maze(void) {

  for(int x=0;x<GRID_SIZE;x++) {
    for(int y=0;y<GRID_SIZE;y++) {
      grid[x][y].walls_visits = 0;
      grid[x][y].distance = 127;
      grid[x][y].turns = 255;
      grid[x][y].valid_directions = NORTH|SOUTH|EAST|WEST;
    }
  }
  grid[GRID_SIZE/2][GRID_SIZE/2].distance = 0;
  grid[GRID_SIZE/2][(GRID_SIZE-1)/2].distance = 0;
  grid[(GRID_SIZE-1)/2][(GRID_SIZE-1)/2].distance = 0;
  grid[(GRID_SIZE-1)/2][(GRID_SIZE)/2].distance = 0;
  /*
      unsigned char xdist,ydist;
      if(x>=GRID_SIZE/2) {
	xdist = x-GRID_SIZE/2;
      } else {
	xdist = (GRID_SIZE-1)/2 - x;
      }

      if(y>=GRID_SIZE/2) {
	ydist = y-GRID_SIZE/2;
      } else {
	ydist = (GRID_SIZE-1)/2 - y;
      }

      grid[x][y].distance = xdist+ydist;//MIN(ABS(x-GRID_SIZE/2),ABS(x-(GRID_SIZE-1)/2))+MIN(ABS(y-GRID_SIZE/2),ABS(y-(GRID_SIZE-1)/2))+'a';
    }
  }
  */
  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL((grid[i][0].walls_visits),NORTH);
    ADD_WALL((grid[i][GRID_SIZE-1].walls_visits),SOUTH);
  }

  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL((grid[GRID_SIZE-1][i].walls_visits),EAST);
    ADD_WALL((grid[0][i].walls_visits),WEST);
  }
  Flood_Fill(0,0);

}

void Maze::Reset_Goal(unsigned char x, unsigned char y) {
  for(int i=0;i<GRID_SIZE;i++) {
    for(int j=0;j<GRID_SIZE;j++) {
      grid[i][j].distance = 127;
      //set visits to 0
      //grid[i][j].walls_visits &= WALLS_MASK;
    }
  }
  grid[x][y].distance = 0;
  grid[x][y].turns = 0;
  grid[x][y].valid_directions = NORTH|SOUTH|EAST|WEST;
  Flood_Fill(0,0);
}

unsigned char Maze::Update_Distance(unsigned char x, unsigned char y) {
  
  
  unsigned char old_distance = grid[x][y].distance;
  
  /*are we at the center yet?*/
  if(old_distance == 0) {
    return 1;
  }
  
  unsigned char best_distance = 127;
  unsigned char walls = grid[x][y].walls_visits;
  //  cout<<"x: "<<(int)x<<"y: "<<(int)y<<"\n";  
  if(!NORTH_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x][y-1].distance)
      best_distance = grid[x][y-1].distance;
  }// else {cout<<"there is a north wall\n";}

  //cout<<"after NORTH: "<<(uint) best_distance<<"\n";

  if(!SOUTH_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x][y+1].distance)
      best_distance = grid[x][y+1].distance;
  }// else {cout<<"there is a south wall\n";}
  //cout<<"after SOUTH: "<<(uint) best_distance<<"\n";




  if(!EAST_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x+1][y].distance)
      best_distance = grid[x+1][y].distance;
  }// else {cout<<"there is a east wall\n";}
  //cout<<"after EAST: "<<(uint) best_distance<<"\n";



  if(!WEST_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x-1][y].distance)
      best_distance = grid[x-1][y].distance;
  }// else {cout<<"there is a west wall\n";}
  //cout<<"after WEST: "<<(uint) best_distance<<"\n";


  grid[x][y].distance = best_distance+1;
  return (best_distance+1 == old_distance);

}

unsigned char Maze::Get_Best_Direction(unsigned char x,unsigned char y, unsigned char orientation) {
  unsigned char walls = grid[x][y].walls_visits;
  unsigned char best_visits = 15;
  unsigned char best_distance = 255;
  unsigned char best_direction;
  unsigned char best_turns = 255;

  unsigned char distance,visits,turns;

  if(grid[x][y].distance == 0)
    return 0;
  
  unsigned char valids = grid[x][y].valid_directions;
  if((orientation&valids)!=0) {
    return orientation;
  }

  if(NORTH_WALL(valids))
    return NORTH;
  if(SOUTH_WALL(valids))
    return SOUTH;
  if(EAST_WALL(valids))
    return EAST;
  if(WEST_WALL(valids))
    return WEST;
  return 0;
  
  if(!NORTH_WALL(walls)) {
    distance = grid[x][y-1].distance;
    visits = VISITS(grid[x][y-1].walls_visits);
    turns = grid[x][y-1].turns+(orientation!=NORTH)+(NORTH&(grid[x][y-1].valid_directions)==0);

    if(distance < best_distance) {
      best_direction = NORTH;
      best_distance = distance;
      best_visits = visits;
      best_turns = turns;
    } else if(distance == best_distance) {
      if(turns<best_turns) {
	best_direction = NORTH;
	best_distance = distance;
	best_visits = visits;
	best_turns = turns;
      } else if(turns == best_turns) {
	if(visits< best_visits) {
	  best_direction = NORTH;
	  best_distance = distance;
	  best_visits = visits;
	  best_turns = turns;
	}
      }
    }
  }

  if(!SOUTH_WALL(walls)) {
    distance = grid[x][y+1].distance;
    visits = VISITS(grid[x][y+1].walls_visits);
    turns = grid[x][y+1].turns+(orientation!=SOUTH)+(SOUTH&(grid[x][y+1].valid_directions)==0);

    if(distance < best_distance) {
      best_direction = SOUTH;
      best_distance = distance;
      best_visits = visits;
      best_turns = turns;
    } else if(distance == best_distance) {
      if(turns<best_turns) {
	best_direction = SOUTH;
	best_distance = distance;
	best_visits = visits;
	best_turns = turns;
      } else if(turns == best_turns) {
	if(visits< best_visits) {
	  best_direction = SOUTH;
	  best_distance = distance;
	  best_visits = visits;
	  best_turns = turns;
	}
      }
    }
  }

  if(!EAST_WALL(walls)) {
    distance = grid[x+1][y].distance;
    visits = VISITS(grid[x+1][y].walls_visits);
    turns = grid[x+1][y].turns+(orientation!=EAST)+(EAST&(grid[x+1][y].valid_directions)==0);

    if(distance < best_distance) {
      best_direction = EAST;
      best_distance = distance;
      best_visits = visits;
      best_turns = turns;
    } else if(distance == best_distance) {
      if(turns<best_turns) {
	best_direction = EAST;
	best_distance = distance;
	best_visits = visits;
	best_turns = turns;
      } else if(turns == best_turns) {
	if(visits< best_visits) {
	  best_direction = EAST;
	  best_distance = distance;
	  best_visits = visits;
	  best_turns = turns;
	}
      }
    }
  }

  if(!WEST_WALL(walls)) {
    distance = grid[x-1][y].distance;
    visits = VISITS(grid[x-1][y].walls_visits);
    turns = grid[x-1][y].turns+(orientation!=WEST)+(WEST&(grid[x-1][y].valid_directions)==0);

    if(distance < best_distance) {
      best_direction = WEST;
      best_distance = distance;
      best_visits = visits;
      best_turns = turns;
    } else if(distance == best_distance) {
      if(turns<best_turns) {
	best_direction = WEST;
	best_distance = distance;
	best_visits = visits;
	best_turns = turns;
      } else if(turns == best_turns) {
	if(visits< best_visits) {
	  best_direction = WEST;
	  best_distance = distance;
	  best_visits = visits;
	  best_turns = turns;
	}
      }
    }
  }
  return best_direction;
}


unsigned char Maze::Get_Direction(unsigned char x, unsigned char y) {
  unsigned char walls = grid[x][y].walls_visits;

  unsigned char best_distance = 255;
  unsigned char best_visits = 15;
  unsigned char best_direction;

  /*check if we've already solved the maze*/
  if(grid[x][y].distance == 0) 
    return 0;
  //cout<<"target distance: "<<(int)target_distance<<"\n";

  if(!NORTH_WALL(walls)) {
    if(grid[x][y-1].distance < best_distance || (grid[x][y-1].distance==best_distance && VISITS(grid[x][y-1].walls_visits) <= best_visits)) {
      best_direction = NORTH;
      //cout<<"it's north!\n";
      best_visits = VISITS(grid[x][y-1].walls_visits);
      best_distance = grid[x][y-1].distance;
   
    }
  }


  if(!SOUTH_WALL(walls)) {
    if(grid[x][y+1].distance < best_distance || (grid[x][y+1].distance == best_distance && VISITS(grid[x][y+1].walls_visits) <= best_visits)) {
      best_direction = SOUTH;
      //cout<<"it's south!\n";
      best_visits = VISITS(grid[x][y+1].walls_visits);
      best_distance = grid[x][y+1].distance;
    }
  }


  if(!EAST_WALL(walls)) {
    if(grid[x+1][y].distance < best_distance || (grid[x+1][y].distance == best_distance && VISITS(grid[x+1][y].walls_visits) <= best_visits)) {
      best_direction = EAST;
      //cout<<"it's east!\n";
      best_visits = VISITS(grid[x+1][y].walls_visits);
      best_distance = grid[x+1][y].distance;
    }
  }


  if(!WEST_WALL(walls)) {
    if(grid[x-1][y].distance < best_distance || (grid[x-1][y].distance == best_distance && VISITS(grid[x-1][y].walls_visits) <= best_visits)) {
      best_direction = WEST;
      //cout<<"it's west!\n";
      best_visits = VISITS(grid[x-1][y].walls_visits);
      best_distance = grid[x-1][y].distance;
    }
  }

  return best_direction;
}

void Maze::Visit(unsigned char x,unsigned char y, unsigned char walls) {

  /*
   *increment visit count.
   *since visits are stored in lower bits, this is just a normal increment.
   */
  if(VISITS(grid[x][y].walls_visits)<15)
    grid[x][y].walls_visits++;

  if(WALLS(grid[x][y].walls_visits)!=walls)
    Add_Walls(x,y,walls);
}
  

void Maze::Add_Walls(unsigned char x, unsigned char y, unsigned char wall) {
  ADD_WALL(grid[x][y].walls_visits,wall);



  /*add in the other sides of the walls*/
  if(NORTH_WALL(wall) && y>0)
    ADD_WALL(grid[x][y-1].walls_visits,SOUTH);

  if(SOUTH_WALL(wall) && y<GRID_SIZE-1)
    ADD_WALL(grid[x][y+1].walls_visits,NORTH);

  if(EAST_WALL(wall) && x<GRID_SIZE-1)
    ADD_WALL(grid[x+1][y].walls_visits,WEST);

  if(WEST_WALL(wall) && x>0) 
    ADD_WALL(grid[x-1][y].walls_visits,EAST);

}

unsigned char Maze::Update_Turns(unsigned char x, unsigned char y) {
  if(grid[x][y].distance == 0) {
    return 1;
  }
  unsigned char old_turns = grid[x][y].turns;
  unsigned char old_valids = grid[x][y].valid_directions;

  unsigned char walls = WALLS(grid[x][y].walls_visits);
  unsigned char bestturns = 255;
  unsigned char directions = 0;
  
  unsigned char target_distance = grid[x][y].distance-1;
  unsigned char turns;
  if(!NORTH_WALL(walls)) {
    if(grid[x][y-1].distance == target_distance) {
      turns =grid[x][y-1].turns+((NORTH&grid[x][y-1].valid_directions) == 0);
	if(turns <bestturns) {
	  bestturns = turns;
	  directions = 0;
	}
	if(turns==bestturns) {
	  directions |= NORTH;
	}
    }
  }

  if(!SOUTH_WALL(walls)) {
    if(grid[x][y+1].distance == target_distance) {
      turns =grid[x][y+1].turns+((SOUTH&grid[x][y+1].valid_directions) == 0);
	if(turns <bestturns) {
	  bestturns = turns;
	  directions = 0;
	}
	if(turns==bestturns) {
	  directions |= SOUTH;
	}
    }
  }

  if(!EAST_WALL(walls)) {
    if(grid[x][y-1].distance == target_distance) {
      turns =grid[x+1][y].turns+((EAST&grid[x+1][y].valid_directions) == 0);
	if(turns <bestturns) {
	  bestturns = turns;
	  directions = 0;
	}
	if(turns==bestturns) {
	  directions |= EAST;
	}
    }
  }

  if(!WEST_WALL(walls)) {
    if(grid[x][y-1].distance == target_distance) {
      turns =grid[x-1][y].turns+((WEST&grid[x-1][y].valid_directions) == 0);
	if(turns <bestturns) {
	  bestturns = turns;
	  directions = 0;
	}
	if(turns==bestturns) {
	  directions |= WEST;
	}
    }
  }

  grid[x][y].turns = bestturns;
  grid[x][y].valid_directions = directions;
  return (bestturns  == old_turns)&&(directions == old_valids);
}

unsigned char Maze::Flood_Fill(unsigned char x, unsigned char y) {
  unsigned char stable = 0;
  unsigned char changed = 0; //set to one if any part of the maze was changed
  while(!stable) {
    stable = 1;
    for(unsigned char x=0;x<GRID_SIZE;x++) {
      for(unsigned char y=0;y<GRID_SIZE;y++) {
	if((Update_Distance(x,y)!=1)&&(Update_Turns(x,y) != 1)) {//it was changed
	  stable = 0;
	  changed = 1;
	}
      }
    }


  }
  return changed;
}
