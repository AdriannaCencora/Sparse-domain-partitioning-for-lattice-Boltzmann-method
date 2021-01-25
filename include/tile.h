#pragma once

#include <vector>
#include <map>
#include <cstdint>

struct tile
{
    size_t tile_size_;
    size_t number_of_hits_ = 0;
    float hit_ratio_;
    std::vector<std::pair<size_t, size_t>> hit_coords_;
};

struct tile_remainder
{
    std::pair<size_t, size_t> dimensions_;
    size_t number_of_hits_ = 0;
//    float ratio_;
    std::vector<std::pair<size_t, size_t>> hit_coords_;
};

typedef std::pair<size_t, size_t> coords_2d_t;
typedef std::map<coords_2d_t, tile> tiling_map_t;
typedef std::map<coords_2d_t, tile_remainder> tiling_remainders_map_t;

struct tiling_parameters_store
{
    std::pair<size_t, size_t> offset_;
    size_t tile_size_;

    tiling_map_t empty_tiles_;
    tiling_map_t non_empty_tiles_;
    tiling_remainders_map_t empty_remainders_;
    tiling_remainders_map_t non_empty_remainders_;

    size_t total_hits_ = 0;
    float total_hit_ratio_ = 0.0;

    size_t number_of_missed_hits_ = 0; // hits in remainder
};
