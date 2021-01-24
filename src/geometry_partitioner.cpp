#include "geometry_partitioner.h"

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            //collects parameters for one specific configuration (size, offset)
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

tiling_parameters_store collect_tiling_parameters(
                                            const geometry_2d_data_store& geometry,
                                            const std::pair<size_t, size_t> offset,
                                            const size_t tile_size)
{
    tiling_parameters_store tiling_parameters_store = {};
    tiling_parameters_store.offset_ = offset;

    size_t max_width = geometry.width_ - tile_size;
    size_t max_length = geometry.length_ - tile_size;

    size_t remainder_width = (geometry.width_ - offset.first) % tile_size;
    size_t remainder_length = (geometry.length_ - offset.second) % tile_size;

    size_t last_width = offset.first + ((geometry.width_ - offset.first) / tile_size) * tile_size - tile_size;
    size_t last_length = offset.second + ((geometry.length_ - offset.second) / tile_size) * tile_size - tile_size;

    for (size_t length = offset.second; length <= max_length; length += tile_size)
    {
        for (size_t width = offset.first; width <= max_width; width += tile_size)
        {
            tile tile = {};
            tile_remainder tile_remainder = {};

            starting_coords_2d_t starting_coords;

            // such case should be examined only if offset.second is set
            // for points skipped by offset: x y in {<0;0> ... <399, 0>}
            // it's like adding the lowest row (y == 0) to iteration loop
            // such "truncated tile" size is equal to offset
            if (offset.second != 0 and length == offset.second)
            {
                starting_coords = std::make_pair(width, 0);

                tile_remainder = apply_remainders_tiling(geometry,
                                                      starting_coords,
                                                      std::make_pair(tile_size, offset.second));

                if (tile_remainder.number_of_hits_)
                {
                    tiling_parameters_store.non_empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
//            tiling_parameters_store.total_hits_ += tile.number_of_hits_;
                }
                else
                {
                    tiling_parameters_store.empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
                }
            }

            // x == 0 - const, y+= tile_size It's for the left most column
            if (offset.first != 0 and width == offset.first)
            {
                starting_coords = std::make_pair(0, length);
                tile_remainder = apply_remainders_tiling(geometry,
                                                      starting_coords,
                                                      std::make_pair(offset.first, tile_size));

                if (tile_remainder.number_of_hits_)
                {
                    tiling_parameters_store.non_empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
  //          tiling_parameters_store.total_hits_ += tile.number_of_hits_;
                }
                else
                {
                    tiling_parameters_store.empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
                }

            }

            // normal tiles start
            starting_coords = std::make_pair(width, length);

            tile = apply_tiling(geometry, starting_coords, tile_size);

            if (tile.number_of_hits_)
            {
                // TODO: Remove me - use modifiers functions instead
                ++tiling_parameters_store.total_tiles_with_hits_;
                tiling_parameters_store.total_hits_ += tile.number_of_hits_;

                tiling_parameters_store.non_empty_tiles_.emplace(
                        std::make_pair(starting_coords, tile));

            }
            else
            {
                tiling_parameters_store.empty_tiles_.emplace(
                        std::make_pair(starting_coords, tile));
            }

            // normal tiles end

            // handle remainder - the right most columns
            // TODO: extract to function
            if (remainder_width and width == last_width)
            {
                starting_coords = std::make_pair((last_width + tile_size), length);

                tile_remainder = apply_remainders_tiling(geometry,
                                                      starting_coords,
                                                      std::make_pair(remainder_width,
                                                                     tile_size));

                if (tile_remainder.number_of_hits_)
                {
                    tiling_parameters_store.non_empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
         //   tiling_parameters_store.total_hits_ += tile.number_of_hits_;
                }
                else
                {
                    tiling_parameters_store.empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
                }
            }

            // handle remainder - the highest raws
            if (remainder_length and length == last_length)
            {
                starting_coords = std::make_pair(width, (last_length + tile_size));

                tile_remainder = apply_remainders_tiling(geometry, starting_coords,
                                    std::make_pair(tile_size, remainder_length));


                if (tile_remainder.number_of_hits_)
                {
        //    tiling_parameters_store.total_hits_ += tile.number_of_hits_;
                    tiling_parameters_store.non_empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
                }
                else
                {
                    tiling_parameters_store.empty_remainders_.emplace(
                            std::make_pair(starting_coords, tile_remainder));
                }
            }

        }
    }

    return tiling_parameters_store;
}


tile apply_tiling(const geometry_2d_data_store& geometry,
                  const std::pair<size_t, size_t> starting_coords,
                  const size_t tile_size)
{
    tile tile = {};
    tile.tile_size_ = tile_size;

    std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + tile_size; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + tile_size; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                tile.number_of_hits_++;
                tile.hit_coords_.push_back(std::make_pair(x, y));
                std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
            }
        }
    }

    std::cout << std::endl;
    return tile;
}

tile_remainder apply_remainders_tiling(const geometry_2d_data_store& geometry,
                                       const std::pair<size_t, size_t> starting_coords,
                                       const std::pair<size_t, size_t> distance)
{
    tile_remainder tile_remainder = {};
    tile_remainder.dimensions_ = distance;

    std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + distance.second; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + distance.first; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                tile_remainder.number_of_hits_++;
                tile_remainder.hit_coords_.push_back(std::make_pair(x, y));
                std::cout << y << "x" << x <<": " << geometry.bitset2d_[y][x] << " ";
            }
        }
    }

    std::cout << std::endl;
    return tile_remainder;
}
