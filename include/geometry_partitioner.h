#pragma once

#include "geometry_data_store.h"
#include "tile.h"

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(const uint16_t tile_size,
                         tiling_parameters_store& tiling_parameters_store)
    : tile_size_(tile_size), tiling_parameters_store_(tiling_parameters_store)
    {}

    void operator()(const geometry_2d_data_store& geometry);
    void operator()(const geometry_3d_data_store& geometry);

private:
    const uint16_t tile_size_;
    tiling_parameters_store& tiling_parameters_store_;
};
