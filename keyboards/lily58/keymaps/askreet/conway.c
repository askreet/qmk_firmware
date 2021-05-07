#include <stdlib.h>
#include <stdio.h>

#include "conway.h"

#define CELL_HANDLER(NAME) void NAME(conway_grid* grid, conway_pos x, conway_pos y)

typedef void (*cell_handler)(conway_grid*, conway_pos, conway_pos);

conway_cell* cell_at(conway_grid* grid, conway_pos x, conway_pos y) {
  return &grid->cells[(y*grid->width)+x];
}

static inline conway_cell* wrapped_cell_at(conway_grid* grid, conway_pos x, conway_pos y) {
  if (x >= 0 && x >= grid->width) x -= grid->width;
  if (x < 0) x += grid->width;

  if (y >= 0 && y >= grid->height) y -= grid->height;
  if (y < 0) y += grid->height;

  return cell_at(grid, x, y);
}

static inline void for_each_cell(conway_grid* grid, cell_handler func) {
  for (conway_pos x = 0; x < grid->width; x++) {
    for (conway_pos y = 0; y < grid->height; y++) {
      func(grid, x, y);
    }
  }
}

void _conway_set_cell_random(conway_grid* grid, conway_pos x, conway_pos y) {
  cell_at(grid, x, y)->neighbors = 0;
  cell_at(grid, x, y)->is_alive = rand() % 2;
}

void conway_seed_random(conway_grid* grid, int seed) {
  srand(seed);

  for_each_cell(grid, _conway_set_cell_random);
}

CELL_HANDLER(_conway_advance_reset_neighbor_calculation) {
  cell_at(grid, x, y)->neighbors = 0;
}

CELL_HANDLER(_conway_advance_determine_neighbor_counts) {
  if (cell_at(grid, x, y)->is_alive) {

    wrapped_cell_at(grid, x-1, y-1)->neighbors++;
    wrapped_cell_at(grid, x  , y-1)->neighbors++;
    wrapped_cell_at(grid, x+1, y-1)->neighbors++;
    wrapped_cell_at(grid, x-1, y  )->neighbors++;
    wrapped_cell_at(grid, x+1, y  )->neighbors++;
    wrapped_cell_at(grid, x-1, y+1)->neighbors++;
    wrapped_cell_at(grid, x  , y+1)->neighbors++;
    wrapped_cell_at(grid, x+1, y+1)->neighbors++;
  }
}

CELL_HANDLER(_conway_advance_compute_future_state) {
  conway_cell* cell = cell_at(grid, x, y);

  if (cell->is_alive) {
    if (cell->neighbors < 2) {
      cell->future_alive = 0;
    } else if (cell->neighbors == 2 || cell->neighbors == 3) {
      cell->future_alive = 1;
    } else if (cell->neighbors > 3) {
      cell->future_alive = 0;
    }
  } else if (cell->neighbors == 3) {
    cell->future_alive = 1;
  }
}

CELL_HANDLER(_conway_promote_future_state) {
  conway_cell* cell = cell_at(grid, x, y);

  cell->is_alive = cell->future_alive;
}

void conway_advance(conway_grid* grid) {
  for_each_cell(grid, _conway_advance_reset_neighbor_calculation);
  for_each_cell(grid, _conway_advance_determine_neighbor_counts);
  for_each_cell(grid, _conway_advance_compute_future_state);
  for_each_cell(grid, _conway_promote_future_state);
}

