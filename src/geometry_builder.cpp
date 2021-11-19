#include "geometry_builder.h"
#include "geometry_data_store.h"

#include <fstream>

bitset_builder::bitset_builder(std::fstream& input_file)
    : input_file_(input_file)
{}

void bitset_builder::operator()(geometry_2d_data_store& geometry)
{
    int input_value;
    std::size_t num_of_hits = 0;  // debug variable
    for (std::size_t length = 0; length < geometry.length_; ++length)
    {
        for (std::size_t width = 0; width < geometry.width_; ++width)
        {
            input_file_ >> input_value;
            geometry.bitset2d_[length].set(width, input_value);
        }
        num_of_hits += geometry.bitset2d_[length].count();
    }
    std::cout << "DEBUG: TOTAL NUMBER OF '1': " << num_of_hits << std::endl;
}

void bitset_builder::operator()(geometry_3d_data_store& geometry)
{
    int input_value;
    std::size_t num_of_hits = 0; // debug variable
    for (std::size_t height = 0; height < geometry.height_; ++height)
    {
        for (std::size_t length = 0; length < geometry.length_; ++length)
        {
            for (std::size_t width = 0; width < geometry.width_; ++width)
            {
                input_file_ >> input_value;
                geometry.bitset3d_[height][length].set(width, input_value);
            }
        num_of_hits += geometry.bitset3d_[height][length].count();
        }
    }
    std::cout << "DEBUG: TOTAL NUMBER OF '1': " << num_of_hits << std::endl;
}
