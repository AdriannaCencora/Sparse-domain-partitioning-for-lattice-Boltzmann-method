#pragma once

#include <vector>
#include <map>
#include <cstdint>

struct tile
{
    size_t tile_size_;
    size_t number_of_hits_ = 0;
    float ratio_;
    std::vector<std::pair<size_t, size_t>> hit_coords_;
};

struct tile_remainder
{
    std::pair<size_t, size_t> dimensions_;
    size_t number_of_hits_ = 0;
    float ratio_;
    std::vector<std::pair<size_t, size_t>> hit_coords_;
};

typedef std::pair<size_t, size_t> starting_coords_2d_t;
typedef std::map<starting_coords_2d_t, tile> tiling_map_t;
typedef std::map<starting_coords_2d_t, tile_remainder> tiling_remainders_map_t;

struct tiling_parameters_store
{
    std::pair<size_t, size_t> offset_;
    tiling_map_t empty_tiles_;
    tiling_map_t non_empty_tiles_;
    tiling_remainders_map_t empty_remainders_;
    tiling_remainders_map_t non_empty_remainders_;
    size_t total_hits_ = 0;
    size_t total_tiles_with_hits_ = 0;
};
