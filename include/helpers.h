#pragma once

#include "tile.h"
#include "geometry_data_store.h"

void count_common_edges(tiling_parameters_store<coords_2d>& store);

void count_common_edges(tiling_parameters_store<coords_3d>& store);

std::size_t calculate_tile_area(const std::size_t tile_size,
                                const std::size_t dimension);
