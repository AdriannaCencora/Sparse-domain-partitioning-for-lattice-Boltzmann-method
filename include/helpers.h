#pragma once

#include "tile.h"
#include "geometry_data_store.h"

bool test_neighbor(const tiling_parameters_store<coords_2d>& store,
                   coords_2d coords, coords_2d direction);

bool test_neighbor(const tiling_parameters_store<coords_3d>& store,
                   coords_3d coords, coords_3d direction);

void count_common_edges(tiling_parameters_store<coords_2d>& store);

void count_common_edges(tiling_parameters_store<coords_3d>& store);

std::size_t calculate_tile_area(const std::size_t tile_size,
                                const std::size_t dimension);

std::size_t calculate_geometry_area(const geometry_2d_data_store& geometry);

std::size_t calculate_geometry_area(const geometry_3d_data_store& geometry);
