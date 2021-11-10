#include "geometry_partitioner.h"
#include "generate_data_output.h"
#include "helpers.h"

#include <tuple>

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            data_store_variant_ = apply_tiling(geometry,
                                               {offset_x, offset_y},
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
                std::cout << "operator()\n";
                data_store_variant_ = apply_tiling(geometry,
                                                  {offset_x, offset_y, offset_z},
                                                  tile_size_);
                generate_data_output(data_store_variant_);
            }
        }
    }
}

tile<coords_2d>& get_tile(tiling_parameters_store<coords_2d>& store,
                          const coords_2d offset, const size_t tile_size,
                          const coords_2d point)
{
    size_t x = point.x;
    size_t y = point.y;

    int offset_x = offset.x;
    int offset_y = offset.y;

    int tile_offset_x = ((tile_size - offset_x) % tile_size);
    int tile_offset_y = ((tile_size - offset_y) % tile_size);

    int tile_x_index  = (x + tile_offset_x) / tile_size;
    int tile_y_index  = (y + tile_offset_y) / tile_size;

    int tile_x_coord = -tile_offset_x + tile_x_index * tile_size;
    int tile_y_coord = -tile_offset_y + tile_y_index * tile_size;

    coords_2d tile_coords = {tile_x_coord, tile_y_coord};

    return store.non_empty_tiles_[tile_coords];
}

tile<coords_3d>& get_tile(tiling_parameters_store<coords_3d>& store,
                          const coords_3d offset, const size_t tile_size,
                          const coords_3d point)
{
    size_t x = point.x;
    size_t y = point.y;
    size_t z = point.z;

    // offset in gemetry
    int offset_x = offset.x;
    int offset_y = offset.y;
    int offset_z = offset.z;

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

    coords_3d tile_coords = {tile_x_coord, tile_y_coord, tile_z_coord};

    return store.non_empty_tiles_[tile_coords];
}

tiling_parameters_store<coords_2d> apply_tiling(const geometry_2d_data_store& geometry,
                                                const coords_2d offset,
                                                const size_t tile_size)
{
    tiling_parameters_store<coords_2d> store = {};
    store.offset_ = offset;

    for (size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
    {
        for (size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
        {
            coords_2d cords = {pos_x, pos_y};
            tile<coords_2d>& tile = get_tile(store, offset, tile_size, cords);

            if (geometry.bitset2d_[pos_y].test(pos_x))
            {
                tile.hit_coords_.push_back({pos_x, pos_y});
                tile.number_of_hits_ += 1;
            }
        }
    }

    //TODO: filtering empty tiles and calculate some stats, extract to separate function
    auto tile_it = store.non_empty_tiles_.begin();
    while (tile_it != store.non_empty_tiles_.end())
    {
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

    count_common_edges(store);

    return store;
}

tiling_parameters_store<coords_3d> apply_tiling(const geometry_3d_data_store& geometry,
                                                const coords_3d offset,
                                                const size_t tile_size)
{
    tiling_parameters_store<coords_3d> store = {};
    store.offset_ = offset;

    for (size_t pos_z = 0; pos_z < geometry.height_; ++pos_z)
    {
        for (size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
        {
            for (size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
            {
                coords_3d cords = {pos_x, pos_y, pos_z};
                tile<coords_3d>& tile = get_tile(store, offset, tile_size, cords);

                if (geometry.bitset3d_[pos_z][pos_y].test(pos_x))
                {
                    tile.hit_coords_.push_back({pos_x, pos_y, pos_z});
                    tile.number_of_hits_ += 1;
                }
            }
        }
    }
    // TODO: filtering empty tiles and calculate some stats, extract to separate function
    auto tile_it = store.non_empty_tiles_.begin();
    while (tile_it != store.non_empty_tiles_.end())
    {
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

    count_common_faces(store);

    return store;
}
