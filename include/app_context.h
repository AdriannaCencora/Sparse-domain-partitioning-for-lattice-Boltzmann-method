#pragma once

#include "geometry_data_store.h"

struct app_context
{
    bool is_ready_;
    geometry_data_store_variant_t geometry_data_store_variant_;

    //config: tile object or something (with size)
};
