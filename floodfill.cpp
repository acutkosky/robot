

#include "floodfill.h"



Maze::Maze(void) {

  for(int x=0;x<GRID_SIZE;x++) {
    for(int y=0;y<GRID_SIZE;y++) {
      grid[x][y].walls_visits = 0;
      grid[x][y].distance = MIN(ABS(x-GRID_SIZE/2),ABS(x-(GRID_SIZE-1)/2))+MIN(ABS(y-GRID_SIZE/2),ABS(y-(GRID_SIZE-1)/2));
    }
  }

  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL(grid[i,0].walls_visits,NORTH);
    ADD_WALL(grid[i,GRID_SIZE-1].walls_visits,SOUTH);
  }

  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL(grid[0,i].walls_visits,EAST);
    ADD_WALL(grid[GRID_SIZE-1,i].walls_visits,WEST);
  }

}

char Maze::Update_Distance(char x, char y) {
  
  
  char old_distance = grid[x][y].distance;
  
  /*are we at the center yet?*/
  if(grid[x][y].old_distance == 0) {
    return 0;
  }
  
  char best_distance = 255;
  char walls = grid[x][y].walls_visits;
  
  if(!NORTH_WALL(walls)) {
    if(best_distance > grid[x][y-1].distance)
      best_distance = grid[x][y-1].distance;
  }


  if(!SOUTH_WALL(walls)) {
    if(best_distance > grid[x][y+1].distance)
      best_distance = grid[x][y+1].distance;
  }



  if(!EAST_WALL(walls)) {
    if(best_distance > grid[x+1][y].distance)
      best_distance = grid[x+1][y].distance;
  }


  if(!WEST_WALL(walls)) {
    if(best_distance > grid[x-1][y].distance)
      best_distance = grid[x-1][y].distance;
  }

  grid[x][y].distance = best_distance+1;
  return (distance+1 == old_distance);

}

char Maze::Get_Direction(char x, char y) {
  char walls = grid[x][y].walls_visits;

  char target_distance = grid[x][y].distance+1;
  char best_visits = 15;
  char best_direction;

  /*check if we've already solved the maze*/
  if(target_distance == 1) 
    return 0;

  if(!NORTH_WALL(walls)) {
    if(grid[x][y-1].distance == target_distance && VISITS(grid[x][y-1].walls_visits) <= best_visits) {
      best_direction = NORTH;
      best_visits = VISITS(grid[x][y-1].walls_visits);
    }
  }


  if(!SOUTH_WALL(walls)) {
    if(grid[x][y+1].distance == target_distance && VISITS(grid[x][y+1].walls_visits) <= best_visits) {
      best_direction = NORTH;
      best_visits = VISITS(grid[x][y+1].walls_visits);
    }
  }


  if(!EAST_WALL(walls)) {
    if(grid[x+1][y].distance == target_distance && VISITS(grid[x][y].walls_visits) <= best_visits) {
      best_direction = NORTH;
      best_visits = VISITS(grid[x+1][y].walls_visits);
    }
  }


  if(!WEST_WALL(walls)) {
    if(grid[x-1][y].distance == target_distance && VISITS(grid[x-1][y].walls_visits) <= best_visits) {
      best_direction = NORTH;
      best_visits = VISITS(grid[x-1][y].walls_visits);
    }
  }

  return best_direction;
}

void Maze::Visit(char x,char y, char walls) {

  /*
   *increment visit count.
   *since visits are stored in lower bits, this is just a normal increment.
   */
  if(VISITS(grid[x][y].walls_visits)<15)
    grid[x][y].walls_visits++;

  if(WALLS(grid[x][y].walls)!=walls)
    Add_Walls(x,y,walls);
}
  

void Maze::Add_Walls(char x, char y, char wall) {
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


char Maze::Flood_Fill(char x, char y) {
  char stable = 0;
  while(!stable) {
    stable = 1;
    for(char x=0;x<GRID_SIZE;x++) {
      for(char y=0;y<GRID_SIZE;y++) {
	if(Update_Distance(x,y) == 1) //it was changed
	  stable = 0;
      }
    }
  }
}
