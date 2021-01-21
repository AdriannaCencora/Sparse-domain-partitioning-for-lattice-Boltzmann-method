#pragma once

#include <map>
#include <vector>

struct params_per_tile
{
    uint8_t tile_size_;
    uint8_t number_of_hits_ = 0; // no more than tile_size_^dimension
    //uint8_t non_empty_neighbours_amount_;
    std::vector<std::pair<uint16_t, uint16_t>> relative_hit_coords_; // counted from current coords, which are treated as start point - 0,0,0. To get absoluth location add relative_hit_coords_ to collected_parameters map key value
};

typedef std::pair<uint16_t, uint16_t> tile_2d_starting_coords_t;
//std::map<tile_2d_starting_coords_t, params_per_tile> collected_parameters;
