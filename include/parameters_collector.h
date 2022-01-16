#pragma once

#include "tile.h"
#include "geometry_data_store.h"

#include <numeric>

void count_common(tiling_parameters_store<coords_2d>& store);

void count_common(tiling_parameters_store<coords_3d>& store);

std::size_t calculate_tile_area(const std::size_t tile_size,
                                const std::size_t dimension);

template <typename CoordsType>
void collect_tiling_parameters(tiling_parameters_store<CoordsType>& store)
{

    store.fill_ratio_ = std::accumulate(store.tiles_.begin(),
                                        store.tiles_.end(),
                                        0.0,
                                        [](float result, const auto& elem)
                                        {
                                            return result + elem.second.fill_ratio_;
                                        }
                                        ) / store.tiles_.size();

    count_common(store);

    store.common_edges_ratio_ =
        static_cast<float>(store.number_of_common_edges_) / store.tiles_.size();
    store.common_vertices_ratio_ =
        static_cast<float>(store.number_of_common_vertices_) / store.tiles_.size();
}
