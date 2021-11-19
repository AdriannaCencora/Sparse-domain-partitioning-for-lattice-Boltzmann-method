#pragma once

#include "tile.h"

bool test_neighbor(const tiling_parameters_store<coords_2d>& store,
                   coords_2d coords, coords_2d direction);

bool test_neighbor(const tiling_parameters_store<coords_3d>& store,
                   coords_3d coords, coords_3d direction);

void count_common_edges(tiling_parameters_store<coords_2d>& store);

void count_common_faces(tiling_parameters_store<coords_3d>& store);
