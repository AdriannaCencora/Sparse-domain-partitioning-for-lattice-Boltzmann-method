#pragma once

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
    bitset_builder(std::fstream& input_file) : input_file_(input_file) {}

    void operator()(geometry_2d_data_store& geometry)
    {
        std::cout << "Debug: I am inside 2D" <<  std::endl;
        uint8_t input_value;

            for (int length = 0; length < geometry.length_; ++ length)
            {
                for (int width = 0; width < geometry.width_; ++width)
                {
                    input_file_ >> input_value;
                    geometry.bitset2d_[length].set(width, input_value);
                }
            }
    }

    void operator()(geometry_3d_data_store& geometry)
    {
        std::cout << "Debug: I am inside 3D" <<  std::endl;
        uint8_t input_value;

        for (int height = 0; height < geometry.height_; ++height)
        {
            for (int length = 0; length < geometry.length_; ++ length)
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
