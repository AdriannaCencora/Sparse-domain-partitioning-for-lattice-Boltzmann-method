#pragma once

#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;
//typedef boost::variant<bitset2d_t, bitset3d_t> bitset_variant_t;

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
    void operator()(geometry_2d_data_store el) const
    {
        std::cout << el.width_ << std::endl;
        std::cout << el.length_ << std::endl;
    }

    void operator()(geometry_3d_data_store el) const
    {
        std::cout << el.width_ << std::endl;
        std::cout << el.length_ << std::endl;
        std::cout << el.height_ << std::endl;
    }
};

//std::ostream& operator<<(std::ostream &out, const geometry_data_store_variant_t &type)
//{
//    out << type.width_;
//}

//    void resize()
//    {
//        bitset3d_ = decltype(bitset3d_)(height_, bitset2d_t(length_, boost::dynamic_bitset<>(width_)));
//    }



//    void resize_bitset3d()
//    {
//        bitset_variant_ = decltype(bitset_variant_)(height_, bitset2d_t(length_,
//                                                            boost::dynamic_bitset<>(width_)));
//    }
//
//    void resize_bitset2d()
//    {
//        bitset_variant_ = decltype(bitset_variant_)(length_, boost::dynamic_bitset<>(width_));
//    }


//struct bitset_visitor : public boost::static_visitor<>
//{
//    void operator()(bitset2d_t bitset_variant)
//    {
//        resize_bitset2d();
//    }
//
//    void operator()(bitset3d_t bitset_variant)
//    {
//        resize_bitset3d();
//    }
//}
