//Holds maze struct and directions

#include <stdbool.h>

/*
 * Enum to represent the four directions a user can look in
 * Here is an example of how to use an enum:
 *
 * Direction dir = EAST;
 * if (dir == EAST) {
 *     printf("Heading EAST!");
 * }
 */
typedef enum {
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
} Direction;

//Struct to represent a room in the maze.
struct maze_room {
  struct maze_room *next;
  int row;
  int col;
  int visited; // 1 if visited, 0 if not visited
  int connections[4]; //0=north, 1=south, 2=west, 3=east
  int connection_north; //1 if wall, 0 if no wall
  int connection_east;
  int connection_west;
  int connection_south;
};

int is_in_range(int row, int col, int num_rows, int num_cols);

struct maze_room *get_neighbor(int num_rows,
        int num_cols, struct maze_room maze[][num_cols], struct maze_room *room, Direction dir);
