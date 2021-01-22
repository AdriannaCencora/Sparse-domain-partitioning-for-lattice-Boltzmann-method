#include "geometry_partitioner.h"

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{

    for (size_t offset_y = 0; offset_y < tile_size_; ++offset_y)
    {
        for (size_t offset_x = 0; offset_x < tile_size_; ++offset_x)
        {
            //collects parameters for one specific configuration (size, offset)
            all_tiling_parameters_store_.push_back(
                    collect_tiling_parameters_per_config(geometry,
                                                         std::make_pair(offset_x, offset_y),
                                                         tile_size_));
        }
    }

///////////////////
//    size_t counter = 0;
//    size_t remainder = geometry.width_ % tile_size_;
//    size_t last_length_to_be_processed = (geometry.length_ / tile_size_) * tile_size_;
//
//    for (size_t length = 0; length < geometry.length_; length += tile_size_)
//    {
//        for (size_t width = 0; width <= geometry.width_ - tile_size_; width += tile_size_)
//        {
//            single_tile_parameters single_tile_parameters = {tile_size_,
//                                                             tile_type::full_area};
//
//            size_t tmp_width = width;
//            size_t  tmp_length = length;
//            size_t  max_width = width + tile_size_ - 1;
//            size_t  tile_area = tile_size_ * tile_size_;
//          //  std::cout << "WIDTH x LENGTH " << width << "x" << length << "   ";
//
//            if ((length == last_length_to_be_processed) and remainder)
//            {
//                // len here equals 399 - first not processed value
//                tile_area = tile_size_ * remainder;
//            }
//
//            for (size_t current_point_counter = 0; current_point_counter < tile_area; ++current_point_counter)
//            {
//                // hit
//                if (geometry.bitset2d_[tmp_length].test(tmp_width))
//                {
//                    single_tile_parameters.number_of_hits_++;
//                    single_tile_parameters.hit_coords_.push_back(
//                            std::make_pair(tmp_width, tmp_length));
//                    ++counter;
//                }
//
//                if (tmp_width == max_width)
//                {
//                    tmp_width = width;
//                    ++tmp_length;
//                }
//                else
//                {
//                    ++tmp_width;
//                }
//            }
//
//            starting_coords_2d_t starting_coords = std::make_pair(width, length);
//            if (single_tile_parameters.number_of_hits_)
//            {
//                std::cout << "HITS: " << single_tile_parameters.number_of_hits_ << " ";
//                tiling_parameters_store_.non_empty_tiles_.emplace(
//                        std::make_pair(starting_coords, single_tile_parameters));
//            }
//            else
//            {
//                tiling_parameters_store_.empty_tiles_.emplace(
//                        std::make_pair(starting_coords, single_tile_parameters));
//            }
//
//            size_t last_processed_width = (geometry.width_ / tile_size_) * tile_size_ - tile_size_;
//            if ((width == last_processed_width) and remainder)
//            {
//
//                single_tile_parameters = {tile_size_, tile_type::partial_area};
//
//                max_width = max_width + remainder;
//                tmp_width = width + tile_size_;
//                tmp_length = length;
//                tile_area = remainder * tile_size_;
//
//                if (last_length_to_be_processed == length)
//                {
//                     tile_area = remainder * remainder;
//                }
//
//                for (size_t current_point_counter = 0; current_point_counter < tile_area; ++current_point_counter)
//                {
//                    if (geometry.bitset2d_[tmp_length].test(tmp_width))
//                    {
//                        single_tile_parameters.number_of_hits_++;
//                        single_tile_parameters.hit_coords_.push_back(
//                                std::make_pair(tmp_width, tmp_length));
//                        ++counter;
//                    }
//                    if (tmp_width == max_width)
//                    {
//                        tmp_width = width + tile_size_;
//                        ++tmp_length;
//                    }
//                    else
//                    {
//                        ++tmp_width;
//                    }
//                }
//
//                starting_coords_2d_t starting_coords = std::make_pair(width, length);
//                if (single_tile_parameters.number_of_hits_)
//                {
//                    tiling_parameters_store_.non_empty_tiles_.emplace(
//                            std::make_pair(starting_coords, single_tile_parameters));
//                }
//                else
//                {
//                    tiling_parameters_store_.non_empty_tiles_.emplace(
//                            std::make_pair(starting_coords, single_tile_parameters));
//                }
//
//
//            }
//           std::cout << std::endl;
//        } // width for
//    } // length for
//
//    std::cout << "counter = " << counter << std::endl;

}

void geometry_partitioner::operator()(const geometry_3d_data_store& geometry)
{
}

tiling_parameters_store collect_tiling_parameters_per_config(
                                            const geometry_2d_data_store& geometry,
                                            const std::pair<size_t, size_t> offset,
                                            const size_t tile_size)
{
    tiling_parameters_store tiling_parameters_store = {};
    tiling_parameters_store.offset_ = offset;

    size_t max_width = geometry.width_ - tile_size; // - offset.first;
    size_t max_length = geometry.length_ - tile_size; // - offset.second;

    for (size_t length = offset.second; length <= max_length; length += tile_size)
    {
        for (size_t width = offset.first; width <= max_width; width += tile_size)
        {
            starting_coords_2d_t starting_coords = std::make_pair(width, length);
            single_tile_parameters single_tile_parameters =
                collect_single_tile_parameters(geometry, starting_coords,  tile_size);

            tiling_parameters_store.total_hits_ += single_tile_parameters.number_of_hits_;

            if (single_tile_parameters.number_of_hits_)
            {
                ++tiling_parameters_store.total_tiles_with_hits_;
                tiling_parameters_store.non_empty_tiles_.emplace(
                        std::make_pair(starting_coords, single_tile_parameters));
            }
            else
            {
                tiling_parameters_store.empty_tiles_.emplace(
                        std::make_pair(starting_coords, single_tile_parameters));
            }
        }
    }
    return tiling_parameters_store;
}

single_tile_parameters collect_single_tile_parameters(
                                            const geometry_2d_data_store& geometry,
                                            const std::pair<size_t, size_t> starting_coords,
                                            const size_t tile_size)
{
    single_tile_parameters single_tile_parameters = {tile_size,
                                                     tile_type::full_area};

    std::cout << "LENGTHxWIDTH " << starting_coords.second << "x" << starting_coords.first << "   ";
    for (size_t y = starting_coords.second; y < starting_coords.second + tile_size; ++y)
    {
        for (size_t x = starting_coords.first; x < starting_coords.first + tile_size; ++x)
        {
            if (geometry.bitset2d_[y].test(x))
            {
                single_tile_parameters.number_of_hits_++;
                single_tile_parameters.hit_coords_.push_back(std::make_pair(x, y));
                std::cout << y << "x" << x <<": " << geometry.bitset2d_[x][y] << " ";
            }
        }
    }

    std::cout << std::endl;
    return single_tile_parameters;
}
