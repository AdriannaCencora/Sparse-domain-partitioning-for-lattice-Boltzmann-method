#pragma once

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <boost/variant.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;
typedef boost::variant<bitset2d_t, bitset3d_t> bitset_variant_t;

struct geometry_data_store
{
    bool is_3d_;
    uint16_t width_;
    uint16_t length_;
    uint16_t height_;

    bitset_variant_t bitset_variant_;
};

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
