#pragma once

#include "geometry_data_store.h"
#include "tile.h"

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(const size_t tile_size)
    : tile_size_(tile_size)
    {}

    void operator()(const geometry_2d_data_store& geometry);
    void operator()(const geometry_3d_data_store& geometry);

private:
    const size_t tile_size_;
    tiling_parameters_store_variant_t data_store_variant_;
};

tiling_2d_parameters_store collect_tiling_parameters(const geometry_2d_data_store& geometry,
                                                     const coords_2d_t current_offset,
                                                     const size_t tile_size);

tiling_3d_parameters_store collect_tiling_parameters(const geometry_3d_data_store& geometry,
                                                     const coords_3d_t current_offset,
                                                     const size_t tile_size);

tile_2d apply_tiling(const geometry_2d_data_store& geometry,
                     const coords_2d_t starting_coords,
                     const size_t tile_size);

tile_3d apply_tiling(const geometry_3d_data_store& geometry,
                     const coords_3d_t starting_coords,
                     const size_t tile_size);

tile_2d_remainder handle_remainders(const geometry_2d_data_store& geometry,
                                    const coords_2d_t starting_coords,
                                    const coords_2d_t distance);

tile_3d_remainder handle_remainders(const geometry_3d_data_store& geometry,
                                    const coords_3d_t starting_coords,
                                    const coords_3d_t distance);
