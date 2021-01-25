#include "geometry_partitioner.h"
#include "tiling_parameters_store_modifiers.h"

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            all_tiling_parameters_store_.push_back(
                    collect_tiling_parameters(geometry,
                                              std::make_pair(offset_x, offset_y),
                                              tile_size_));
        }
    }
}

void geometry_partitioner::operator()(const geometry_3d_data_store& geometry)
{
}

tiling_parameters_store collect_tiling_parameters(const geometry_2d_data_store& geometry,
                                                  const std::pair<size_t, size_t> offset,
                                                  const size_t tile_size)
{
    tiling_parameters_store tiling_parameters_store = {};
    tiling_parameters_store.offset_ = offset;
    tiling_parameters_store.tile_size_ = tile_size;

    size_t max_width = geometry.width_ - tile_size;
    size_t max_length = geometry.length_ - tile_size;

    size_t remainder_width = (geometry.width_ - offset.first) % tile_size;
    size_t remainder_length = (geometry.length_ - offset.second) % tile_size;

    size_t last_width = geometry.width_ - remainder_width - tile_size;
    size_t last_length = geometry.length_ - remainder_length - tile_size;

    std::cout << "!!!!!!!!!!!!" << std::endl;
    std::cout << "remainder_width: " << remainder_width << std::endl;
    std::cout << "remainder_length: " << remainder_length << std::endl;
    std::cout << "last_width: " << last_width << std::endl;
    std::cout << "last_length: " << last_length << std::endl;


    for (size_t length = offset.second; length <= max_length; length += tile_size)
    {
        for (size_t width = offset.first; width <= max_width; width += tile_size)
        {
            coords_2d_t starting_coords;

            starting_coords = std::make_pair(width, length);
            tile tile = apply_tiling(geometry, starting_coords, tile_size);
            add_tile(tiling_parameters_store, starting_coords, tile);

            //Handle omitted rows and columns (4 cases) - 'tiling remainders':
            //(TODO: Consider extraction of below code to separe function.)

            // Case 1: y = 0 = const, x+= tile size; the lowest rows.
            if (offset.second and length == offset.second)
            {
                starting_coords = std::make_pair(width, 0);
                tile_remainder tile_remainder = handle_remainders(geometry,
                                                        starting_coords,
                                                        std::make_pair(tile_size, offset.second));

                add_remainder(tiling_parameters_store, starting_coords, tile_remainder);
            }

            // Case 2: x = 0 = const, y+= tile_size; the left most columns.
            if (offset.first and width == offset.first)
            {
                starting_coords = std::make_pair(0, length);
                tile_remainder tile_remainder = handle_remainders(geometry,
                                                      starting_coords,
                                                      std::make_pair(offset.first, tile_size));

                add_remainder(tiling_parameters_store, starting_coords, tile_remainder);
            }

            // Case 3: x = remainder = const, y+= tile size; the right most columns.
            if (remainder_width and width == last_width)
            {
                starting_coords = std::make_pair((last_width + tile_size), length);
                tile_remainder tile_remainder = handle_remainders(geometry,
                                                      starting_coords,
                                                      std::make_pair(remainder_width, tile_size));

                add_remainder(tiling_parameters_store, starting_coords, tile_remainder);
            }

            // Case 4: y = remainder = const, x += tile size; the highest rows.
            if (remainder_length and length == last_length)
            {
                starting_coords = std::make_pair(width, (last_length + tile_size));
                tile_remainder tile_remainder = handle_remainders(geometry,
                                                        starting_coords,
                                                        std::make_pair(tile_size, remainder_length));

                add_remainder(tiling_parameters_store, starting_coords, tile_remainder);
            }
        }
    }

    size_t total_geometry_area = geometry.width_ * geometry.length_;
    calculate_total_hits_and_misses(tiling_parameters_store, total_geometry_area);

    return tiling_parameters_store;
}


tile apply_tiling(const geometry_2d_data_store& geometry,
                  const std::pair<size_t, size_t> starting_coords,
                  const size_t tile_size)
{
    tile tile = {};
    tile.tile_size_ = tile_size;

    //std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + tile_size; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + tile_size; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                tile.number_of_hits_++;
                tile.hit_coords_.push_back(std::make_pair(x, y));
  //              std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
            }
        }
    }

//    std::cout << std::endl;
    return tile;
}

tile_remainder handle_remainders(const geometry_2d_data_store& geometry,
                                 const std::pair<size_t, size_t> starting_coords,
                                 const std::pair<size_t, size_t> distance)
{
    tile_remainder tile_remainder = {};
    tile_remainder.dimensions_ = distance;

  //  std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + distance.second; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + distance.first; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                tile_remainder.number_of_hits_++;
                tile_remainder.hit_coords_.push_back(std::make_pair(x, y));
//                std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
            }
        }
    }

//    std::cout << std::endl;
    return tile_remainder;
}
