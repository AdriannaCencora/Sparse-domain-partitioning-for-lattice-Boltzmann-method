#pragma once

#include "app_context.h"

#include <string>

//struct bitset_builder : public boost::static_visitor<>
//{
//    bitset_builder(std::fstream& input_file) : input_file_(input_file) {}
//
//    void operator()(bitset3d_t bitset_variant);
//    void operator()(bitset2d_t bitset_variant);
//
//    std::fstream& input_file_;
//
//};

app_context process_data_entry(std::string filename);
