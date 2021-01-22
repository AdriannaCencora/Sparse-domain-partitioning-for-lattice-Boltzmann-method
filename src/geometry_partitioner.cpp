#include "geometry_partitioner.h"

void geometry_partitioner::operator()(const geometry_2d_data_store& geometry)
{
    int counter = 0;
    int remainder = geometry.width_ % tile_size_;
    int last_length_to_be_processed = (geometry.length_ / tile_size_) * tile_size_;

    for (int length = 0; length < geometry.length_; length += tile_size_)
    {
        for (int width = 0; width <= geometry.width_ - tile_size_; width += tile_size_)
        {
            single_tile_parameters single_tile_parameters = {tile_size_,
                                                             tile_type::full_area};

            int tmp_width = width;
            int tmp_length = length;
            int max_width = width + tile_size_ - 1;
            int tile_area = tile_size_ * tile_size_;
          //  std::cout << "WIDTH x LENGTH " << width << "x" << length << "   ";

            if ((length == last_length_to_be_processed) and remainder)
            {
                // len here equals 399 - first not processed value
                tile_area = tile_size_ * remainder;
            }

            for (int current_point_counter = 0; current_point_counter < tile_area; ++current_point_counter)
            {
                // hit
                if (geometry.bitset2d_[tmp_length].test(tmp_width))
                {
                    single_tile_parameters.number_of_hits_++;
                    single_tile_parameters.hit_coords_.push_back(
                            std::make_pair(tmp_width, tmp_length));
                    ++counter;
                }

                if (tmp_width == max_width)
                {
                    tmp_width = width;
                    ++tmp_length;
                }
                else
                {
                    ++tmp_width;
                }
            }

            starting_coords_2d_t starting_coords = std::make_pair(width, length);
            if (single_tile_parameters.number_of_hits_)
            {
                std::cout << "HITS: " << single_tile_parameters.number_of_hits_ << " ";
                tiling_parameters_store_.non_empty_tiles_.emplace(
                        std::make_pair(starting_coords, single_tile_parameters));
            }
            else
            {
                tiling_parameters_store_.empty_tiles_.emplace(
                        std::make_pair(starting_coords, single_tile_parameters));
            }

            int last_processed_width = (geometry.width_ / tile_size_) * tile_size_ - tile_size_;
            if ((width == last_processed_width) and remainder)
            {

                single_tile_parameters = {tile_size_, tile_type::partial_area};

                max_width = max_width + remainder;
                tmp_width = width + tile_size_;
                tmp_length = length;
                tile_area = remainder * tile_size_;

                if (last_length_to_be_processed == length)
                {
                     tile_area = remainder * remainder;
                }

                for (int current_point_counter = 0; current_point_counter < tile_area; ++current_point_counter)
                {
                    if (geometry.bitset2d_[tmp_length].test(tmp_width))
                    {
                        single_tile_parameters.number_of_hits_++;
                        single_tile_parameters.hit_coords_.push_back(
                                std::make_pair(tmp_width, tmp_length));
                        ++counter;
                    }
                    if (tmp_width == max_width)
                    {
                        tmp_width = width + tile_size_;
                        ++tmp_length;
                    }
                    else
                    {
                        ++tmp_width;
                    }
                }

                starting_coords_2d_t starting_coords = std::make_pair(width, length);
                if (single_tile_parameters.number_of_hits_)
                {
                    tiling_parameters_store_.non_empty_tiles_.emplace(
                            std::make_pair(starting_coords, single_tile_parameters));
                }
                else
                {
                    tiling_parameters_store_.non_empty_tiles_.emplace(
                            std::make_pair(starting_coords, single_tile_parameters));
                }


            }
           std::cout << std::endl;
        } // width for
    } // length for

    std::cout << "counter = " << counter << std::endl;

}

void geometry_partitioner::operator()(const geometry_3d_data_store& geometry)
{
}
