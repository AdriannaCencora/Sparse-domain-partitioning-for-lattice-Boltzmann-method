#pragma once

#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;

struct geometry_2d_data_store
{
    geometry_2d_data_store() : dimension_(2)
    {}

    std::size_t width_;
    std::size_t length_;

    std::size_t dimension_;

    bitset2d_t bitset2d_;
};

struct geometry_3d_data_store
{
    geometry_3d_data_store() : dimension_(3)
    {}

    std::size_t width_;
    std::size_t length_;
    std::size_t height_;

    std::size_t dimension_;

    bitset3d_t bitset3d_;
};

typedef boost::variant<geometry_2d_data_store,
                       geometry_3d_data_store> geometry_data_store_variant_t;

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
