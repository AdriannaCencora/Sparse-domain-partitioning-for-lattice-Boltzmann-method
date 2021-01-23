#pragma once

#include "geometry_data_store.h"
#include "tile.h"

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(const size_t tile_size,
                         std::vector<tiling_parameters_store>& all_tiling_parameters_store)
    : tile_size_(tile_size), all_tiling_parameters_store_(all_tiling_parameters_store)
    {}

    void operator()(const geometry_2d_data_store& geometry);
    void operator()(const geometry_3d_data_store& geometry);

private:
    const size_t tile_size_;
    std::vector<tiling_parameters_store>& all_tiling_parameters_store_;
};

tiling_parameters_store collect_tiling_parameters_per_config(
                                            const geometry_2d_data_store& geometry,
                                            const std::pair<size_t, size_t> current_offset,
                                            const size_t tile_size);

single_tile_parameters apply_tiling(const geometry_2d_data_store& geometry,
                                    const std::pair<size_t, size_t> starting_coords,
                                    const std::pair<size_t, size_t> distance);
