#pragma once

#include "geometry_data_store.h"
#include "tile.h"


struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(const std::size_t tile_size)
    : tile_size_(tile_size)
    {}

    void operator()(const geometry_2d_data_store& geometry);
    void operator()(const geometry_3d_data_store& geometry);

private:
    const std::size_t tile_size_;
    tiling_parameters_store_variant_t data_store_variant_;
};

tiling_parameters_store<coords_2d> apply_tiling(const geometry_2d_data_store& geometry,
                                                const coords_2d& offset,
                                                const std::size_t tile_size);

tiling_parameters_store<coords_3d> apply_tiling(const geometry_3d_data_store& geometry,
                                                const coords_3d& offset,
                                                const std::size_t tile_size);

tile<coords_2d>& get_tile(tiling_parameters_store<coords_2d>& store,
                          const coords_2d current_coord);

tile<coords_3d>& get_tile(tiling_parameters_store<coords_3d>& store,
                          const coords_3d current_coord);

void prepare_tiles(const geometry_2d_data_store& geometry,
                   tiling_parameters_store<coords_2d>& store);

void prepare_tiles(const geometry_3d_data_store& geometry,
                   tiling_parameters_store<coords_3d>& store);
