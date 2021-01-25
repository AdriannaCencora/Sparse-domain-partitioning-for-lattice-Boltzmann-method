#pragma once

#include "tile.h"

#include <numeric>

template <typename Store, typename Coords, typename Tile>
void add_tile(Store& tiling_parameters_store,
              Coords starting_coords,
              Tile& tile,
              size_t total_tile_area)
{
    tile.hit_ratio_ = static_cast<float>(tile.number_of_hits_) / total_tile_area;

    if (tile.number_of_hits_)
    {
        tiling_parameters_store.non_empty_tiles_.emplace(
                std::make_pair(starting_coords, tile));
    }
    else
    {
        tiling_parameters_store.empty_tiles_.emplace(
                std::make_pair(starting_coords, tile));
    }

}

template <typename Store, typename Coords, typename Tile>
void add_remainder(Store& tiling_parameters_store,
                   Coords starting_coords,
                   Tile& tile_remainder)
{
    if (tile_remainder.number_of_hits_)
    {
        tiling_parameters_store.non_empty_remainders_.emplace(
                std::make_pair(starting_coords, tile_remainder));
    }
    else
    {
        tiling_parameters_store.empty_remainders_.emplace(
                std::make_pair(starting_coords, tile_remainder));
    }
}

template <typename Store>
void calculate_total_hits_and_misses(Store& tiling_parameters_store,
                                     size_t total_geometry_area)
{
    size_t result = std::accumulate(std::begin(tiling_parameters_store.non_empty_tiles_),
                                    std::end(tiling_parameters_store.non_empty_tiles_),
                                    0,
                                    [](size_t current_result, const auto& elem)
                                    {
                                        return current_result + elem.second.number_of_hits_;
                                    });

    tiling_parameters_store.total_hits_ = result;
    tiling_parameters_store.total_hit_ratio_ = static_cast<float>(result) / total_geometry_area;

    result = std::accumulate(std::begin(tiling_parameters_store.non_empty_remainders_),
                             std::end(tiling_parameters_store.non_empty_remainders_),
                             0,
                             [](size_t current_result, const auto& elem)
                             {
                                 return current_result + elem.second.number_of_hits_;
                             });

    tiling_parameters_store.number_of_missed_hits_ = result;
}
