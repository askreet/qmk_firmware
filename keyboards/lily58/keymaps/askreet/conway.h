#include <stdint.h>

typedef struct {
  uint8_t neighbors : 4;
  uint8_t future_alive : 1;
  uint8_t is_alive : 1;
} conway_cell;
typedef int8_t conway_pos; // grid max 128x128

typedef struct {
	conway_cell* cells;
	size_t width;
	size_t height;
} conway_grid;

conway_cell* cell_at(conway_grid* grid, conway_pos x, conway_pos y);
uint8_t conway_neighbor_count(conway_grid* grid, conway_pos x, conway_pos y);
void conway_seed_random(conway_grid* grid, int seed);
void conway_advance(conway_grid* grid);

