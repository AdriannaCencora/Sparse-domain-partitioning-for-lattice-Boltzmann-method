#pragma once

#include <vector>
#include <map>
#include <cstdint>

enum tile_type
{
    full_area, //area = tile_size^2
    partial_area // area = tile_size x reminder - edge
};

struct single_tile_parameters
{
    single_tile_parameters(uint16_t tile_size, tile_type tile_type)
        : tile_size_(tile_size), tile_type_(tile_type), number_of_hits_(0)
    {}

    uint16_t tile_size_;
    tile_type tile_type_;
    uint16_t number_of_hits_;
    float ratio_;
    std::vector<std::pair<uint16_t, uint16_t>> hit_coords_;
//    std::pair<uint16_t, uint16_t> starting_tile_offset_x_y_;
//    std::pair<uint16_t, uint16_t> starting_coords_;
};

typedef std::pair<uint16_t, uint16_t> starting_coords_2d_t;

struct tiling_parameters_store
{
    std::pair<uint16_t, uint16_t> offset_x_y_;
    std::map<starting_coords_2d_t, single_tile_parameters> empty_tiles_;
    std::map<starting_coords_2d_t, single_tile_parameters> non_empty_tiles_;
};
