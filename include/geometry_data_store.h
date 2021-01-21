#pragma once

#include "tile.h"

#include <vector>
#include <map>
#include <iostream> //remove me
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;

// TOCONSIDER: bitset comparator wrapper? gets starting_point + tile_size as parameter
// and run logical AND operation on bitset 'slice' and tile (some mask)  for example

struct geometry_2d_data_store
{
    uint16_t width_;
    uint16_t length_;

    bitset2d_t bitset2d_;
};

struct geometry_3d_data_store
{
    uint16_t width_;
    uint16_t length_;
    uint16_t height_;

    bitset3d_t bitset3d_;
};

typedef boost::variant<geometry_2d_data_store, geometry_3d_data_store> geometry_data_store_variant_t;

struct output_printer : public boost::static_visitor<>
{
    void operator()(geometry_2d_data_store& geometry) const
    {
        std::cout << geometry.width_ << std::endl;
        std::cout << geometry.length_ << std::endl;
    }

    void operator()(geometry_3d_data_store& geometry) const
    {
        std::cout << geometry.width_ << std::endl;
        std::cout << geometry.length_ << std::endl;
        std::cout << geometry.height_ << std::endl;
    }
};

struct bitset_builder : public boost::static_visitor<>
{
    bitset_builder(std::fstream& input_file)
        : input_file_(input_file)
    {}

    void operator()(geometry_2d_data_store& geometry)
    {
        std::cout << "Debug: I am inside 2D" <<  std::endl;
        int input_value;
        int num_of_hits = 0;
        for (int length = 0; length < geometry.length_; ++length)
            {
                for (int width = 0; width < geometry.width_; ++width)
                {
                    input_file_ >> input_value;
                    geometry.bitset2d_[length].set(width, input_value);
                }
                num_of_hits += geometry.bitset2d_[length].count();
            }
                std::cout << "COUNT: " << num_of_hits << std::endl;
    }

    void operator()(geometry_3d_data_store& geometry)
    {
        std::cout << "Debug: I am inside 3D" <<  std::endl;
        int input_value;

        for (int height = 0; height < geometry.height_; ++height)
        {
            for (int length = 0; length < geometry.length_; ++length)
            {
                for (int width = 0; width < geometry.width_; ++width)
                {
                    input_file_ >> input_value;
                    geometry.bitset3d_[height][length].set(width, input_value);
                }
            }
        }
    }

    private:
    std::fstream& input_file_;
};

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(uint16_t tile_size,
                         tiling_parameters_store& tiling_parameters_store)
        : tile_size_(tile_size), tiling_parameters_store_(tiling_parameters_store)
    {}

    int counter = 0;

    void operator()(const geometry_2d_data_store& geometry)
    {
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

    void operator()(const geometry_3d_data_store& geometry)
    {
    }

private:
    uint16_t tile_size_;
    tiling_parameters_store& tiling_parameters_store_;
};
