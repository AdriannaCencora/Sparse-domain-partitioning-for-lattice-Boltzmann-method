#pragma once

#include "tile.h"


void add_tile(tiling_parameters_store& tiling_parameters_store,
              coords_2d_t starting_coords,
              tile tile);

void add_remainder(tiling_parameters_store& tiling_parameters_store,
                   coords_2d_t starting_coords,
                   tile_remainder& tile_remainder);

void calculate_total_hits_and_misses(tiling_parameters_store& tiling_parameters_store,
                                     size_t total_geometry_area);
