#pragma once

#include "tile.h"

#include <fstream>
#include <boost/variant.hpp>

void generate_data_output(tiling_parameters_store_variant_t& data_store);

struct file_writer : public boost::static_visitor<>
{
    file_writer(std::fstream& output_file)
        : output_file_(output_file)
    {}

    void operator()(const tiling_2d_parameters_store& data_store);
    void operator()(const tiling_3d_parameters_store& data_store);

private:
    std::fstream& output_file_;
};
