#pragma once

#include <boost/variant.hpp>
#include <vector>
#include <map>
#include <cstdint>
#include <iostream>

struct coords_2d
{
    int x;
    int y;

    coords_2d operator+(const coords_2d& other) const
    {
        coords_2d coords = {};
        coords.x = x + other.x;
        coords.y = y + other.y;

        return coords;
    }

    bool operator==(const coords_2d& other) const
    {
        return x == other.x and y == other.y;
    }

    bool operator<(const coords_2d& other) const
    {
        return x < other.x or (x == other.x and y < other.y);
    }
};

struct coords_3d
{
    int x;
    int y;
    int z;

    coords_3d operator+(const coords_3d& other) const
    {
        coords_3d coords = {};
        coords.x = x + other.x;
        coords.y = y + other.y;
        coords.z = z + other.z;

        return coords;
    }

    bool operator==(const coords_3d& other) const
    {
        return x == other.x and y == other.y and z == other.z;
    }

    bool operator<(const coords_3d& other) const
    {
        return x < other.x or (x == other.x and y < other.y) or (x == other.x and y == other.y and z < other.z);
    }
};

template<typename CoordsType>
struct tile
{
    size_t number_of_hits_ = 0;
    float hit_ratio_;
    std::vector<CoordsType> hit_coords_;
    size_t number_of_common_sides_ = 0;
};

template<typename CoordsType>
using tiling_map_t = std::map<CoordsType, tile<CoordsType>>;

template<typename CoordsType>
struct tiling_parameters_store
{
    CoordsType offset_;
    size_t tile_size_;

    tiling_map_t<CoordsType> empty_tiles_;
    tiling_map_t<CoordsType> non_empty_tiles_;

    size_t total_hits_ = 0;
    float total_hit_ratio_ = 0.0;
};

typedef boost::variant<tiling_parameters_store<coords_2d>,
                       tiling_parameters_store<coords_3d>> tiling_parameters_store_variant_t;
