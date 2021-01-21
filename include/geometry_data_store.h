#pragma once

#include "tile.h"

#include <vector>
#include <iostream> //remove me
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;

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

    std::fstream& input_file_;
};

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(int tile_size)
        : tile_size_(tile_size)
    {}

    int counter = 0;

    void operator()(const geometry_2d_data_store& geometry)
    {
        for (int length = 0; length <= geometry.length_ - tile_size_; length += tile_size_)
        {
            for (int width = 0; width <= geometry.width_ - tile_size_; width += tile_size_)
            {
                int tmp_width = width;
                int tmp_length = length;
                int max_width = width + tile_size_ - 1;

                std::cout << "WIDTH x LENGTH " << width << "x" << length << "   ";

                for (int current_point_counter = 0; current_point_counter < (tile_size_ * tile_size_); ++current_point_counter)
                {
                    if (geometry.bitset2d_[tmp_length].test(tmp_width))
                    {
                        ++counter;
            //            std::cout << "[" << tmp_width << ", " << tmp_length << "] ";
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

                int last_processed_width = (geometry.width_ / tile_size_) * tile_size_ - tile_size_;
                int remainder = geometry.width_ % tile_size_;
                if ((width == last_processed_width) and remainder)
                {
                    max_width = max_width + remainder;
                    tmp_width = width + tile_size_;
                    tmp_length = length;

                    for (int current_point_counter = 0; current_point_counter < (remainder * tile_size_); ++current_point_counter)
                    {
                        if (geometry.bitset2d_[tmp_length].test(tmp_width))
                        {
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
                }
               std::cout << std::endl;
            } // width for
        } // length for

        std::cout << "counter = " << counter << std::endl;

    }

    void operator()(const geometry_3d_data_store& geometry)
    {
    }


    int tile_size_;
};
