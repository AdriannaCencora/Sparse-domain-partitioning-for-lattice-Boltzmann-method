#pragma once

#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>

typedef std::vector<boost::dynamic_bitset<>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;

struct geometry_data_store
{
    uint16_t width_;
    uint16_t length_;
    uint16_t height_;

    bitset3d_t bitset3d_;

    void resize()
    {
        bitset3d_ = decltype(bitset3d_)(height_, bitset2d_t(length_,
                                                            boost::dynamic_bitset<>(width_)));
    }
};
