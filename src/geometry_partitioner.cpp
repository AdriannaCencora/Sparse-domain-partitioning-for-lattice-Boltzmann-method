#include "geometry_partitioner.h"
#include "tiling_parameters_store_modifiers.h"
#include "generate_data_output.h"

#include <tuple>

//TODO: Refactor file. Extract functions, refactor types and naming
void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            data_store_variant_ = collect_tiling_parameters(geometry,
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
        std::cout << "Iteration for z offset: " << offset_z << std::endl;

        for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
        {
            for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
            {
                data_store_variant_ = collect_tiling_parameters(geometry,
                                                  std::make_tuple(offset_x, offset_y, offset_z),
                                                  tile_size_);
                generate_data_output(data_store_variant_);
            }
        }
    }
}

tiling_2d_parameters_store collect_tiling_parameters(const geometry_2d_data_store& geometry,
                                                     const coords_2d_t offset,
                                                     const size_t tile_size)
{
    tiling_2d_parameters_store tiling_2d_parameters_store = {};
    tiling_2d_parameters_store.offset_ = offset;
    tiling_2d_parameters_store.tile_size_ = tile_size;

    size_t max_width = geometry.width_ - tile_size;
    size_t max_length = geometry.length_ - tile_size;

    size_t remainder_width = (geometry.width_ - offset.first) % tile_size;
    size_t remainder_length = (geometry.length_ - offset.second) % tile_size;

    size_t last_width = geometry.width_ - remainder_width - tile_size;
    size_t last_length = geometry.length_ - remainder_length - tile_size;

    for (size_t length = offset.second; length <= max_length; length += tile_size)
    {
        for (size_t width = offset.first; width <= max_width; width += tile_size)
        {
            coords_2d_t starting_coords;

            starting_coords = std::make_pair(width, length);
            tile_2d tile = apply_tiling(geometry, starting_coords, tile_size);
            add_tile(tiling_2d_parameters_store, starting_coords, tile, tile_size*tile_size);

            //Handle omitted rows and columns (4 cases) - 'tiling remainders':
            //(TODO: Consider extraction of below code to separe function.)

            // Case 1: y = 0 = const, x+= tile size; the lowest rows.
            if (offset.second and length == offset.second)
            {
                starting_coords = std::make_pair(width, 0);
                tile_2d_remainder tile_2d_remainder = handle_remainders(geometry,
                                                        starting_coords,
                                                        std::make_pair(tile_size, offset.second));

                add_remainder(tiling_2d_parameters_store, starting_coords, tile_2d_remainder);
            }

            // Case 2: x = 0 = const, y+= tile_size; the left most columns.
            if (offset.first and width == offset.first)
            {
                starting_coords = std::make_pair(0, length);
                tile_2d_remainder tile_2d_remainder = handle_remainders(geometry,
                                                      starting_coords,
                                                      std::make_pair(offset.first, tile_size));

                add_remainder(tiling_2d_parameters_store, starting_coords, tile_2d_remainder);
            }

            // Case 3: x = remainder = const, y+= tile size; the right most columns.
            if (remainder_width and width == last_width)
            {
                starting_coords = std::make_pair((last_width + tile_size), length);
                tile_2d_remainder tile_2d_remainder = handle_remainders(geometry,
                                                      starting_coords,
                                                      std::make_pair(remainder_width, tile_size));

                add_remainder(tiling_2d_parameters_store, starting_coords, tile_2d_remainder);
            }

            // Case 4: y = remainder = const, x += tile size; the highest rows.
            if (remainder_length and length == last_length)
            {
                starting_coords = std::make_pair(width, (last_length + tile_size));
                tile_2d_remainder tile_2d_remainder = handle_remainders(geometry,
                                                        starting_coords,
                                                        std::make_pair(tile_size, remainder_length));

                add_remainder(tiling_2d_parameters_store, starting_coords, tile_2d_remainder);
            }
        }
    }

    size_t total_geometry_area = geometry.width_ * geometry.length_;
    calculate_total_hits_and_misses(tiling_2d_parameters_store, total_geometry_area);

    return tiling_2d_parameters_store;
}


tiling_3d_parameters_store collect_tiling_parameters(const geometry_3d_data_store& geometry,
                                                     const coords_3d_t offset,
                                                     const size_t tile_size)
{
    tiling_3d_parameters_store tiling_3d_parameters_store = {};
    tiling_3d_parameters_store.offset_ = offset;
    tiling_3d_parameters_store.tile_size_ = tile_size;

    size_t max_width = geometry.width_ - tile_size;
    size_t max_length = geometry.length_ - tile_size;
    size_t max_height = geometry.height_ - tile_size;

    size_t offset_x = std::get<0>(offset);
    size_t offset_y = std::get<1>(offset);
    size_t offset_z = std::get<2>(offset);

    size_t remainder_width = (geometry.width_ - offset_x) % tile_size;
    size_t remainder_length = (geometry.length_ - offset_y) % tile_size;
    size_t remainder_height = (geometry.height_ - offset_z) % tile_size;

    size_t last_width = geometry.width_ - remainder_width - tile_size;
    size_t last_length = geometry.length_ - remainder_length - tile_size;
    size_t last_height = geometry.height_ - remainder_height - tile_size;

    size_t total_tile_area = tile_size * tile_size * tile_size;

    for (size_t height = offset_z; height <= max_height; height += tile_size)
    {
        for (size_t length = offset_y; length <= max_length; length += tile_size)
        {
            for (size_t width = offset_x; width <= max_width; width += tile_size)
            {
                coords_3d_t starting_coords;

                starting_coords = std::make_tuple(width, length, height);
                tile_3d tile = apply_tiling(geometry, starting_coords, tile_size);
                add_tile(tiling_3d_parameters_store, starting_coords, tile, total_tile_area);

                //Handle omitted rows and columns (4 cases) - 'tiling remainders':
                //(TODO: Consider extraction of below code to separe function.)

                // Case 1: y = 0 = const, x+= tile size; the lowest rows.
                if (offset_y and length == offset_y)
                {
                    starting_coords = std::make_tuple(width, 0, height);
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                            starting_coords,
                                                            std::make_tuple(tile_size,
                                                                   offset_y, tile_size));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

                // Case 2: x = 0 = const, y+= tile_size; the left most columns.
                if (offset_x and width == offset_x)
                {
                    starting_coords = std::make_tuple(0, length, height);
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                          starting_coords,
                                                          std::make_tuple(offset_x,
                                                              tile_size, tile_size));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

                // Case 3: z = 0 = const
                if (offset_z and height == offset_z)
                {
                    starting_coords = std::make_tuple(width, length, 0);
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                          starting_coords,
                                                          std::make_tuple(tile_size,
                                                              tile_size, offset_z));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

                // Case 4: x = remainder = const, y+= tile size; the right most columns.
                if (remainder_width and width == last_width)
                {
                    starting_coords = std::make_tuple((last_width + tile_size), length, height);
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                          starting_coords,
                                                          std::make_tuple(remainder_width,
                                                              tile_size, tile_size));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

                // Case 5: y = remainder = const, x += tile size; the highest rows.
                if (remainder_length and length == last_length)
                {
                    starting_coords = std::make_tuple(width, (last_length + tile_size), height);
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                            starting_coords,
                                                            std::make_tuple(tile_size,
                                                                remainder_length, tile_size));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

                // Case 6: z  = remainder = const, x += tile size; the highest rows.
                if (remainder_height and length == last_height)
                {
                    starting_coords = std::make_tuple(width, length, (last_height + tile_size));
                    tile_3d_remainder tile_3d_remainder = handle_remainders(geometry,
                                                            starting_coords,
                                                            std::make_tuple(tile_size,
                                                                tile_size, remainder_height));

                    add_remainder(tiling_3d_parameters_store, starting_coords, tile_3d_remainder);
                }

            }
        }
    }

    size_t total_geometry_area = geometry.width_ * geometry.length_ * geometry.height_;
    calculate_total_hits_and_misses(tiling_3d_parameters_store, total_geometry_area);

    return tiling_3d_parameters_store;
}

tile_2d apply_tiling(const geometry_2d_data_store& geometry,
                     const coords_2d_t starting_coords,
                     const size_t tile_size)
{
    tile_2d tile = {};
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

tile_3d apply_tiling(const geometry_3d_data_store& geometry,
                     const coords_3d_t starting_coords,
                     const size_t tile_size)
{
    tile_3d tile = {};
    tile.tile_size_ = tile_size;

        //std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t z = std::get<2>(starting_coords); z < std::get<2>(starting_coords) + tile_size; ++z)
    {
        for (size_t y = std::get<1>(starting_coords); y < std::get<1>(starting_coords) + tile_size; ++y)
        {
            for (size_t x = std::get<0>(starting_coords); x < std::get<0>(starting_coords) + tile_size; ++x)
            {
                if (geometry.bitset3d_[z][y].test(x))
                {
                    tile.number_of_hits_++;
                    tile.hit_coords_.push_back(std::make_tuple(x, y, z));
      //              std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
                }
            }
        }
    }

//    std::cout << std::endl;
    return tile;
}


tile_2d_remainder handle_remainders(const geometry_2d_data_store& geometry,
                                    const coords_2d_t starting_coords,
                                    const coords_2d_t distance)
{
    tile_2d_remainder tile_2d_remainder = {};
    tile_2d_remainder.dimensions_ = distance;

  //  std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + distance.second; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + distance.first; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                tile_2d_remainder.number_of_hits_++;
                tile_2d_remainder.hit_coords_.push_back(std::make_pair(x, y));
//                std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
            }
        }
    }

//    std::cout << std::endl;
    return tile_2d_remainder;
}

tile_3d_remainder handle_remainders(const geometry_3d_data_store& geometry,
                          const coords_3d_t starting_coords,
                          const coords_3d_t distance)
{
    tile_3d_remainder tile = {};
    tile.dimensions_ = distance;

//    std::cout << "HEIGHTxLENGTHxWIDTH " << std::get<2>(starting_coords) << "x" << std::get<1>(starting_coords) << "x" << std::get<0>(starting_coords) << "   ";
    for (size_t z = std::get<2>(starting_coords); z < std::get<2>(starting_coords) + std::get<2>(distance); ++z)
    {
        for (size_t y = std::get<1>(starting_coords); y < std::get<1>(starting_coords) + std::get<1>(distance); ++y)
        {
            for (size_t x = std::get<0>(starting_coords); x < std::get<0>(starting_coords) + std::get<0>(distance); ++x)
            {
                if (geometry.bitset3d_[z][y].test(x))
                {
                    tile.number_of_hits_++;
                    tile.hit_coords_.push_back(std::make_tuple(x, y, z));
      //              std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
                }
            }
        }
    }

//    std::cout << std::endl;
    return tile;
}
