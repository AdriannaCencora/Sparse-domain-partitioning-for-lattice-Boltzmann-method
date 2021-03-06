#pragma once

#include <boost/variant.hpp>
#include <vector>
#include <map>
#include <tuple>
#include <cstdint>
#include <iostream>

// TODO: Refactor me heavily!
// Consider templates to reduce number of structs with the same fields
// Use dedicated structures instead of tuple and pair for coords representaton
// variant is unnecessry here

typedef std::pair<size_t, size_t> coords_2d_t;
typedef std::tuple<size_t, size_t, size_t> coords_3d_t;

struct tile_2d
{
    size_t tile_size_;
    size_t number_of_hits_ = 0;
    float hit_ratio_;
    std::vector<coords_2d_t> hit_coords_;
};

struct tile_2d_remainder
{
    coords_2d_t dimensions_;
    size_t number_of_hits_ = 0;
    std::vector<coords_2d_t> hit_coords_;
};
typedef std::map<coords_2d_t, tile_2d> tiling_2d_map_t;
typedef std::map<coords_2d_t, tile_2d_remainder> tiling_2d_remainders_map_t;

struct tile_3d
{
    size_t tile_size_;
    size_t number_of_hits_ = 0;
    float hit_ratio_;
    std::vector<coords_3d_t> hit_coords_;
};

struct tile_3d_remainder
{
    coords_3d_t dimensions_;
    size_t number_of_hits_ = 0;
    std::vector<coords_3d_t> hit_coords_;
};

typedef std::map<coords_3d_t, tile_3d> tiling_3d_map_t;
typedef std::map<coords_3d_t, tile_3d_remainder> tiling_3d_remainders_map_t;

struct tiling_2d_parameters_store
{
    coords_2d_t offset_;
    size_t tile_size_;

    tiling_2d_map_t empty_tiles_;
    tiling_2d_map_t non_empty_tiles_;
    tiling_2d_remainders_map_t empty_remainders_;
    tiling_2d_remainders_map_t non_empty_remainders_;

    size_t total_hits_ = 0;
    float total_hit_ratio_ = 0.0;

    size_t number_of_missed_hits_ = 0; // hits in remainder
};

struct tiling_3d_parameters_store
{
    coords_3d_t offset_;
    size_t tile_size_;

    tiling_3d_map_t empty_tiles_;
    tiling_3d_map_t non_empty_tiles_;
    tiling_3d_remainders_map_t empty_remainders_;
    tiling_3d_remainders_map_t non_empty_remainders_;

    size_t total_hits_ = 0;
    float total_hit_ratio_ = 0.0;

    size_t number_of_missed_hits_ = 0; // hits in remainder
};

typedef boost::variant<tiling_2d_parameters_store,
                       tiling_3d_parameters_store> tiling_parameters_store_variant_t;
