#pragma once

#include "tile.h"

const coords_2d directions_2d[]= {{1, 0},  // right
                                  {-1, 0},  // left
                                  {0, 1},   // up
                                  {0, -1}}; // down

const coords_3d directions_3d[] = {{1, 0, 0},  // right
                                   {-1, 0, 0},  // left
                                   {0, 1, 0},   // up
                                   {0, -1, 0},  // down
                                   {0, 0, 1},   // front
                                   {0, 0, -1}}; // back

bool test_neighbor(const tiling_parameters_store<coords_2d>& store, coords_2d coords, coords_2d direction);

bool test_neighbor(const tiling_parameters_store<coords_3d>& store, coords_3d coords, coords_3d direction);

void count_common_edges(tiling_parameters_store<coords_2d>& store);

void count_common_faces(tiling_parameters_store<coords_3d>& store);
