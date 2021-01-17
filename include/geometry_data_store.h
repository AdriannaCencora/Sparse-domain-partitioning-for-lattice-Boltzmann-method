#include <vector>
#include <bitset>

namespace
{
    const unsigned size = 400; // TODO: Remove me
}
typedef std::vector<std::bitset<size>> bitset2d_t;
typedef std::vector<bitset2d_t> bitset3d_t;

struct geometry_data_store
{
    geometry_data_store(uint16_t width, uint16_t length, uint16_t height)
        : width_(width), length_(length), height_(height)
    {}

    uint16_t width_;
    uint16_t length_;
    uint16_t height_;
    bitset3d_t bitset3d_ = decltype(bitset3d_)(size, bitset2d_t(size));
};
