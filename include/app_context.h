#pragma once

#include "geometry_data_store.h"

struct app_context
{
    geometry_data_store geometry_data_store_ = {};
    uint8_t dimensions;
    //config: tile object or something (with size)
} ctx = {};
