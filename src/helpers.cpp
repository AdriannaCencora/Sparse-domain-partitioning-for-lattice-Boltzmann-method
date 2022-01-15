#include "helpers.h"


void count_common(tiling_parameters_store<coords_2d>& store)
{
    for (auto& tile_it : store.non_empty_tiles_)
    {
        // right neighbor
        coords_2d neighbor_coords = tile_it.first;
        neighbor_coords.x += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_edges_;
        }

        // down neighbor
        neighbor_coords.x = tile_it.first.x;
        neighbor_coords.y += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_edges_;
        }

        // vertex neighbor
        neighbor_coords.x += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_vertices_;
        }
    }
}

void count_common(tiling_parameters_store<coords_3d>& store)
{
    for (auto& tile_it : store.non_empty_tiles_)
    {
        // right neighbor
        coords_3d neighbor_coords = tile_it.first;
        neighbor_coords.x += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_edges_;
        }

        // up neighbor
        neighbor_coords.x = tile_it.first.x;
        neighbor_coords.y += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_edges_;
        }

        // back neighbor
        neighbor_coords.y = tile_it.first.y;
        neighbor_coords.z += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_edges_;
        }

        // vertex neighbor
        neighbor_coords.x += store.tile_size_;
        neighbor_coords.y += store.tile_size_;
        if (store.non_empty_tiles_.find(neighbor_coords) != store.non_empty_tiles_.end())
        {
            ++store.number_of_common_vertices_;
        }
    }
}

std::size_t calculate_tile_area(const std::size_t tile_size,
                                const std::size_t dimension)
{
    std::size_t area = 1;

    for (std::size_t i = 1; i <= dimension; ++i)
    {
        area *= tile_size;
    }
    return area;
}
