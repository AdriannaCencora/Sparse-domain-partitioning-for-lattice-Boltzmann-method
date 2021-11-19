#include "helpers.h"

bool test_neighbor(const tiling_parameters_store<coords_2d>& store, coords_2d coords, coords_2d direction)
{
    coords_2d neighbor_coords = coords;
    neighbor_coords.x += store.tile_size_ * direction.x;
    neighbor_coords.y += store.tile_size_ * direction.y;

    auto this_tile_it = store.non_empty_tiles_.find(coords);
    auto neighbor_tile_it = store.non_empty_tiles_.find(neighbor_coords);

    if (neighbor_tile_it != store.non_empty_tiles_.end())
    {
        for (auto hit_coords : this_tile_it->second.hit_coords_)
        {
            coords_2d neighbor_bit = hit_coords + direction;
            return std::find(neighbor_tile_it->second.hit_coords_.begin(),
                             neighbor_tile_it->second.hit_coords_.end(),
                             neighbor_bit) != neighbor_tile_it->second.hit_coords_.end();
        }
    }

    return false;
}

bool test_neighbor(const tiling_parameters_store<coords_3d>& store, coords_3d coords, coords_3d direction)
{
    coords_3d neighbor_coords = coords;
    neighbor_coords.x += store.tile_size_ * direction.x;
    neighbor_coords.y += store.tile_size_ * direction.y;
    neighbor_coords.z += store.tile_size_ * direction.z;

    auto this_tile_it = store.non_empty_tiles_.find(coords);
    auto neighbor_tile_it = store.non_empty_tiles_.find(neighbor_coords);

    if (neighbor_tile_it != store.non_empty_tiles_.end())
    {
        for (auto hit_coords : this_tile_it->second.hit_coords_)
        {
            coords_3d neighbor_bit = hit_coords + direction;
            return std::find(neighbor_tile_it->second.hit_coords_.begin(),
                             neighbor_tile_it->second.hit_coords_.end(),
                             neighbor_bit) != neighbor_tile_it->second.hit_coords_.end();
        }
    }

    return false;
}

void count_common_edges(tiling_parameters_store<coords_2d>& store)
{
    for (auto& tile_it : store.non_empty_tiles_)
    {
        coords_2d coords = tile_it.first;

        for (auto direction : directions_2d)
        {
            if (test_neighbor(store, coords, direction))
            {
                tile_it.second.number_of_common_sides_++;
            }
        }
    }
}

void count_common_faces(tiling_parameters_store<coords_3d>& store)
{
    for (auto& tile_it : store.non_empty_tiles_)
    {
        coords_3d coords = tile_it.first;

        for (auto direction : directions_3d)
        {
            if (test_neighbor(store, coords, direction))
            {
                tile_it.second.number_of_common_sides_++;
            }
        }
    }
}