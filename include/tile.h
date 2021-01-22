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
    single_tile_parameters(size_t tile_size, tile_type tile_type)
        : tile_size_(tile_size), tile_type_(tile_type), number_of_hits_(0)
    {}

    size_t tile_size_;
    tile_type tile_type_;
    size_t number_of_hits_;
    float ratio_;
    std::vector<std::pair<size_t, size_t>> hit_coords_;
};

typedef std::pair<size_t, size_t> starting_coords_2d_t;
typedef std::map<starting_coords_2d_t, single_tile_parameters> tiling_parameters_map_t;

struct tiling_parameters_store
{
    std::pair<size_t, size_t> offset_;
    tiling_parameters_map_t empty_tiles_;
    tiling_parameters_map_t non_empty_tiles_;
    size_t total_hits_ = 0;
    size_t total_tiles_with_hits_ = 0;
};
