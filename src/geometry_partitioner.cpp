#include "geometry_partitioner.h"
#include "generate_data_output.h"
#include "helpers.h"

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{
    for (std::size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (std::size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            coords_2d coords = {offset_x, offset_y};
            data_store_variant_ = apply_tiling(geometry, coords, tile_size_);
            generate_data_output(data_store_variant_);
        }
    }
}

void geometry_partitioner::operator()(const geometry_3d_data_store& geometry)
{
    for (std::size_t offset_z = 0; offset_z < tile_size_; ++offset_z)
    {
        for (std::size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
        {
            for (std::size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
            {
                coords_3d coords = {offset_x, offset_y, offset_z};
                data_store_variant_ = apply_tiling(geometry, coords, tile_size_);
                generate_data_output(data_store_variant_);
            }
        }
    }
}

tile<coords_2d>& get_tile(tiling_parameters_store<coords_2d>& store,
                          const coords_2d current_coord)
{
    // Out of border tile complement, the same for whole iteration
    // Ex. for tile_size = 3: for offset.x=0 -> 0
    //                        for offset.x=1 -> 2
    //                        for offset.x=2 -> 1
    int out_of_border_complement_x = ((store.tile_size_ - store.offset_.x) % store.tile_size_);
    int out_of_border_complement_y = ((store.tile_size_ - store.offset_.y) % store.tile_size_);

    // Tile starting coordinates without alignement (offset and jumping by tile size)
    int starting_x = (current_coord.x + out_of_border_complement_x) / store.tile_size_;
    int starting_y = (current_coord.y + out_of_border_complement_y) / store.tile_size_;

    // Real tile starting coordinates in geometry (from 'the most left' side)
    int displaced_starting_x = -out_of_border_complement_x + starting_x * store.tile_size_;
    int displaced_starting_y = -out_of_border_complement_y + starting_y * store.tile_size_;

    coords_2d tile_start_coords = {displaced_starting_x, displaced_starting_y};

    // Create new key - tile starting coordinates - with empty tile or get already existing
    return store.non_empty_tiles_[tile_start_coords];
}

tile<coords_3d>& get_tile(tiling_parameters_store<coords_3d>& store,
                          const coords_3d current_coord)
{
    // Out of border tile complement, the same for whole iteration
    int out_of_border_complement_x = ((store.tile_size_ - store.offset_.x) % store.tile_size_);
    int out_of_border_complement_y = ((store.tile_size_ - store.offset_.y) % store.tile_size_);
    int out_of_border_complement_z = ((store.tile_size_ - store.offset_.z) % store.tile_size_);

    // Tile starting coordinates without alignement (offset and jumping by tile size)
    int starting_x = (current_coord.x + out_of_border_complement_x) / store.tile_size_;
    int starting_y = (current_coord.y + out_of_border_complement_y) / store.tile_size_;
    int starting_z = (current_coord.z + out_of_border_complement_z) / store.tile_size_;

    // Real tile starting coordinates in geometry (from 'the most left' side)
    int displaced_starting_x = -out_of_border_complement_x + starting_x * store.tile_size_;
    int displaced_starting_y = -out_of_border_complement_y + starting_y * store.tile_size_;
    int displaced_starting_z = -out_of_border_complement_z + starting_z * store.tile_size_;

    coords_3d tile_starting_coords = {displaced_starting_x, displaced_starting_y,
                                      displaced_starting_z};

    // Create new key - tile starting coordinates - with empty tile or get already existing
    return store.non_empty_tiles_[tile_starting_coords];
}

void prepare_tiles(const geometry_2d_data_store& geometry,
                   tiling_parameters_store<coords_2d>& store)
{
    // Test coordinates one by one, each is belonging to specific tile and
    // save collected information inside tile structure (info storage)
    for (std::size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
    {
        for (std::size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
        {
            tile<coords_2d>& tile = get_tile(store, {pos_x, pos_y});

            if (geometry.bitset2d_[pos_y].test(pos_x))
            {
                tile.hit_coords_.push_back({pos_x, pos_y});
                tile.number_of_hits_ += 1;
            }
        }
    }
}

void prepare_tiles(const geometry_3d_data_store& geometry,
                   tiling_parameters_store<coords_3d>& store)
{
    for (std::size_t pos_z = 0; pos_z < geometry.height_; ++pos_z)
    {
        for (std::size_t pos_y = 0; pos_y < geometry.length_; ++pos_y)
        {
            for (std::size_t pos_x = 0; pos_x < geometry.width_; ++pos_x)
            {
                tile<coords_3d>& tile = get_tile(store, {pos_x, pos_y, pos_z});

                if (geometry.bitset3d_[pos_z][pos_y].test(pos_x))
                {
                    tile.hit_coords_.push_back({pos_x, pos_y, pos_z});
                    tile.number_of_hits_ += 1;
                }
            }
        }
    }
}
