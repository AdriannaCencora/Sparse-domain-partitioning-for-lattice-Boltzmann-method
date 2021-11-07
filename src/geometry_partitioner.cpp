#include "geometry_partitioner.h"
#include "tiling_parameters_store_modifiers.h"
#include "generate_data_output.h"

#include <tuple>

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            data_store_variant_ = apply_tiling(geometry,
                                               std::make_pair(offset_x, offset_y),
                                               tile_size_);
            generate_data_output(data_store_variant_);
        }
    }
}

void geometry_partitioner::operator()(const geometry_3d_data_store& geometry)
{
    for (size_t offset_z = 0; offset_z < tile_size_; ++offset_z)
    {
        for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
        {
            for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
            {
                data_store_variant_ = apply_tiling(geometry,
                                                   std::make_tuple(offset_x, offset_y, offset_z),
                                                   tile_size_);
                generate_data_output(data_store_variant_);
            }
        }
    }
}

tile_2d& get_tile(tiling_2d_parameters_store& store, const coords_2d_t offset,
                  const size_t tile_size, const coords_2d_t point)
{
    size_t x = std::get<0>(point);
    size_t y = std::get<1>(point);

    int offset_x = std::get<0>(offset);
    int offset_y = std::get<1>(offset);

    int tile_offset_x = ((tile_size - offset_x) % tile_size);
    int tile_offset_y = ((tile_size - offset_y) % tile_size);

    int tile_x_index  = (x + tile_offset_x) / tile_size;
    int tile_y_index  = (y + tile_offset_y) / tile_size;

    int tile_x_coord = -tile_offset_x + tile_x_index * tile_size;
    int tile_y_coord = -tile_offset_y + tile_y_index * tile_size;

    coords_2d_t tile_coords = std::make_pair(tile_x_coord, tile_y_coord);

    return store.non_empty_tiles_[tile_coords];
}

tile_3d& get_tile(tiling_3d_parameters_store& store, const coords_3d_t offset,
                  const size_t tile_size, const coords_3d_t point)
{
    size_t x = std::get<0>(point);
    size_t y = std::get<1>(point);
    size_t z = std::get<2>(point);

    int offset_x = std::get<0>(offset);
    int offset_y = std::get<1>(offset);
    int offset_z = std::get<2>(offset);

    // helper for handling 'reminders'
    int tile_offset_x = ((tile_size - offset_x) % tile_size);
    int tile_offset_y = ((tile_size - offset_y) % tile_size);
    int tile_offset_z = ((tile_size - offset_z) % tile_size);

    // relative start coords
    int tile_x_index  = (x + tile_offset_x) / tile_size;
    int tile_y_index  = (y + tile_offset_y) / tile_size;
    int tile_z_index  = (z + tile_offset_z) / tile_size;

    // start coords in geometry
    int tile_x_coord = -tile_offset_x + tile_x_index * tile_size;
    int tile_y_coord = -tile_offset_y + tile_y_index * tile_size;
    int tile_z_coord = -tile_offset_z + tile_z_index * tile_size;

    coords_3d_t tile_coords = std::make_tuple(tile_x_coord, tile_y_coord, tile_z_coord);

    return store.non_empty_tiles_[tile_coords];
}


tiling_2d_parameters_store apply_tiling(const geometry_2d_data_store& geometry,
                                        const coords_2d_t offset,
                                        const size_t tile_size)
{
    tiling_2d_parameters_store store = {};
    store.offset_ = offset;

    for (size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
    {
        for (size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
        {
            coords_2d_t cords = std::make_pair(pos_x, pos_y);

            tile_2d& tile = get_tile(store, offset, tile_size, cords);

            if (geometry.bitset2d_[pos_y].test(pos_x))
            {
                tile.hit_coords_.push_back(std::make_pair(pos_x, pos_y));
                tile.number_of_hits_ += 1;
            }
        }
    }

    // filtering empty tiles and calculate some stats
    auto tile_it = store.non_empty_tiles_.begin();
    while (tile_it != store.non_empty_tiles_.end())
    {
        tile_it->second.tile_size_ = tile_size;
        if (tile_it->second.number_of_hits_ == 0)
        {
            store.empty_tiles_[tile_it->first] = tile_it->second;
            tile_it = store.non_empty_tiles_.erase(tile_it);
        }
        else
        {
            tile_it->second.hit_ratio_ = tile_it->second.number_of_hits_/ (tile_size*tile_size);
            store.total_hits_ += tile_it->second.number_of_hits_;
            ++tile_it;
        }
    }

    store.tile_size_ = tile_size;
    store.total_hit_ratio_ = store.total_hits_;
    store.total_hit_ratio_ /= (geometry.width_*geometry.length_);
    return store;
}

tiling_3d_parameters_store apply_tiling(const geometry_3d_data_store& geometry,
                                        const coords_3d_t offset,
                                        const size_t tile_size)
{
    tiling_3d_parameters_store store = {};
    store.offset_ = offset;

    for (size_t pos_z = 0; pos_z < geometry.height_; ++pos_z)
    {
        for (size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
        {
            for (size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
            {
                coords_3d_t cords = std::make_tuple(pos_x, pos_y, pos_z);
                tile_3d& tile = get_tile(store, offset, tile_size, cords);

                if (geometry.bitset3d_[pos_z][pos_y].test(pos_x))
                {
                    tile.hit_coords_.push_back(std::make_tuple(pos_x, pos_y, pos_z));
                    tile.number_of_hits_ += 1;
                }
            }
        }
    }

    // filtering empty tiles and calculate some stats
    auto tile_it = store.non_empty_tiles_.begin();
    while (tile_it != store.non_empty_tiles_.end())
    {
        tile_it->second.tile_size_ = tile_size;
        if (tile_it->second.number_of_hits_ == 0)
        {
            store.empty_tiles_[tile_it->first] = tile_it->second;
            tile_it = store.non_empty_tiles_.erase(tile_it);
        }
        else
        {
            tile_it->second.hit_ratio_ = tile_it->second.number_of_hits_/ (tile_size*tile_size);
            store.total_hits_ += tile_it->second.number_of_hits_;
            ++tile_it;
        }
    }

    store.tile_size_ = tile_size;
    store.total_hit_ratio_ = store.total_hits_;
    store.total_hit_ratio_ /= (geometry.width_*geometry.length_);
    return store;
}

