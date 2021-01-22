#pragma once

#include "geometry_data_store.h"
#include "tile.h"

struct app_context
{
    bool is_ready_;
    geometry_data_store_variant_t geometry_data_store_variant_;
    std::vector<tiling_parameters_store> all_tiling_parameters_store_;
    std::vector<size_t> tile_sizes_collection_;  // some config to be refactored
};
