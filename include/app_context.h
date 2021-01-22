#pragma once

#include "geometry_data_store.h"
#include "tile.h"

struct app_context
{
    bool is_ready_;
    geometry_data_store_variant_t geometry_data_store_variant_;
    tiling_parameters_store tiling_parameters_store_ = {};
    std::vector<uint16_t> tile_sizes_collection_;  // some config to be refactored
};
