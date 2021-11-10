#pragma once

#include "geometry_data_store.h"

#include <fstream>

struct bitset_builder : public boost::static_visitor<>
{
    bitset_builder(std::fstream& input_file);

    void operator()(geometry_2d_data_store& geometry);
    void operator()(geometry_3d_data_store& geometry);

private:
    std::fstream& input_file_;
};
